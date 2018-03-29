#include "Game.h"
#include "Hand.h"
#include "GameScene.h"
#include "CardDeck.h"

USING_NS_CC;

Game::~Game()
{
	deck->release();

	tempHand->release();

	for (int i = 0; i < PLAYER_NUM; i++) {
		hands[i]->release();
	}
}

bool Game::init()
{
	if (!Node::init()) {
		return false;
	}

	deck = CardDeck::create();
	deck->retain();

	tempHand = Hand::create();
	tempHand->retain();

	for (int i = 0; i < PLAYER_NUM; i++) {
		hands[i] = Hand::create();
		hands[i]->retain();
	}

	initGame();

	return true;
}

GameResult Game::getGameResult()
{
	GameResult result;

	result.bid_user = bid[0] + bid[2];
	result.bid_bot = bid[1] + bid[3];
	result.trick_user = tricks[0] + tricks[2];
	result.trick_bot = tricks[1] + tricks[3];
	result.score_user = scores[0];
	result.score_bot = scores[1];
	result.bag_user = bags[0];
	result.bag_bot = bags[1];

	return result;
}

void Game::step()
{
	//CCLog("Call Game::step");

	if (idleCount > 0) {
		idleCount--;
		return;
	}

	int curState = state;
	state = eGS_IDLE;

	switch (curState) {
	case eGS_IDLE:
		break;
	case eGS_SHUFFLE:
		shuffle();
		idleCount = 3;
		state = eGS_BID;
		break;
	case eGS_BID:
		processBid();
		break;
	case eGS_PREPARE_PLAY:
		preparePlay();
		break;
	case eGS_PLAYCARD:
		playCard();
		break;
	case eGS_NEXT_TRICK:
		nextTrick();
		break;
	case eGS_END_ROUND:
		endRound();
		break;
	case eGS_NEW_ROUND:
		initRound();
		break;
	default:
		break;
	}
}

void Game::initGame()
{
	CCLog("Call Game::initGame");

	scores[0] = 0;
	scores[1] = 0;
	bags[0] = 0;
	bags[1] = 0;
	startTurn = -1;

	initRound();
}

void Game::newRound()
{
	CCLog("Call Game::newRound");
	state = eGS_NEW_ROUND;
}

void Game::initRound()
{
	trickCardCount = 0;
	closedCardCount = 0;
	bidCount = 0;
	turn = 0;

	//set startTurn
	if (startTurn == -1) {
		srand(time(0));
		startTurn = rand() % PLAYER_NUM;
	} else {
		startTurn ++;
		startTurn %= PLAYER_NUM;
	}

	doubleNil = false;

	for (int i = 0; i < PLAYER_NUM; i++) {
		tricks[i] = 0;
		bid[i] = 0;
		emptySuit[i][0] = false;
		emptySuit[i][1] = false;
		emptySuit[i][2] = false;
		emptySuit[i][3] = false;
		played[i] = false;
	}

	// setup the cards played
	for (int i = 0; i < CARD_NUM; i++) {
		playedCards[i].suit = BLANK;
		playedCards[i].value = 0;
	}

	suitLed = BLANK;
	spadeBroken = false;

	state = eGS_SHUFFLE;
}

void Game::shuffle()
{
	CCLog("Call Game::shuffle");

	deck->shuffle();
	for (int i = 0; i < PLAYER_NUM; i++) {
		hands[i]->setHand(deck, i);
		hands[i]->organizeHand();
	}

	for (int i = 0; i < PLAYER_NUM; i++) {
		gameScene->onSetHand(i, hands[i]);
	}
}

void Game::processBid()
{
	CCLog("Call Game::processBid");

	int bidTurn = (startTurn + bidCount) % PLAYER_NUM;
	if (bidTurn == 0) {
		gameScene->onWantBid();
	} else {
		// computer players' bids
		bid[bidTurn] = hands[bidTurn]->bid();
		gameScene->onShowBid(bidTurn, bid[bidTurn]);
		bidCount++;
		state = (bidCount >= PLAYER_NUM) ? eGS_PREPARE_PLAY : eGS_BID;
	}
}

void Game::setBid(int value, bool dnil/* = false*/)
{
	CCLog("Call Game::setBid");

	doubleNil = dnil;
	bid[0] = value;

	gameScene->onShowBid(0, bid[0]);

	bidCount++;
	state = (bidCount >= PLAYER_NUM) ? eGS_PREPARE_PLAY : eGS_BID;
}

void Game::preparePlay()
{
	gameScene->onFinishBid();
	turn = startTurn;
	state = eGS_PLAYCARD;
}

void Game::setCardPlayed(int playerIndex, int cardIndex)
{
	CCLog("Call Game::cardPlayed");

	if (turn == 0) {
		gameScene->onTouchEnable(false);
		playUserCard(cardIndex);
	}

	turn ++;
	turn %= PLAYER_NUM;

	// check complete trick
	if (played[0] && played[1] && played[2] && played[3]) {
		idleCount = 1;
		state = eGS_NEXT_TRICK;
	} else {
		if (turn == 0) {
			gameScene->onTouchEnable(getEnabledCardSuit(), true);
		} else {
			state = eGS_PLAYCARD;
		}
	}
}

void Game::playCard()
{
	CCLog("Call Game::playCard");

	if (turn == 0) {
		gameScene->onTouchEnable(getEnabledCardSuit(), true);
		return;
	}

	bool oldBroken = spadeBroken;
	Hand* hand = hands[turn];
	tempHand->setHand(hand);
	tempHand->applyRule(suitLed, spadeBroken);
	Card card = thinkCard(tempHand);

	if (card.suit == SPADE) {
		spadeBroken = true;
	}

	int cardIndex = hand->getCardIndex(card);
	if (cardIndex < 0) {
		CCLog("Call Game::playCard error detail card{%d, %d}", card.suit, card.value);
		for (int i = 0; i < PLAYER_CARD_NUM; i++) {
			CCLog("Call Game::playCard error hand information = {%d, %d}", hand->cardSuit(i), hand->cardValue(i));
		}
	} else {
		addCardPlayed(card.suit, card.value);
		hand->removeCard(card.suit, card.value);
		played[turn] = true;

		gameScene->onPlayCard(turn, cardIndex);
		if (spadeBroken && !oldBroken) {
			gameScene->onSpadeBroken();
		}
	}
}

int Game::getEnabledCardSuit()
{
	Hand* hand = hands[0];
	tempHand->setHand(hand);
	tempHand->applyRule(suitLed, spadeBroken);

	int enabledSuit = 0;
	for (int i = 0; i < PLAYER_CARD_NUM; i++) {
		Suit suit = tempHand->cardSuit(i);
		if (suit != BLANK) {
			enabledSuit |= suit;
		}
	}
	return enabledSuit;
}

void Game::playUserCard(int cardIndex)
{
	CCLog("Call Game::playUserCard");

	bool oldBroken = spadeBroken;

	// check if legal to play card
	Hand* hand = hands[0];
	Suit suit = hand->cardSuit(cardIndex);
	int value = hand->cardValue(cardIndex);

	if (suitLed == BLANK) { // set leading suit if leading
		suitLed = suit;
	}

	if (suit == SPADE) {
		spadeBroken = true;
	}

	addCardPlayed(suit, value);
	hands[0]->removeCard(suit, value);
	played[0] = true;

	if (spadeBroken && !oldBroken) {
		gameScene->onSpadeBroken();
	}
}

void Game::nextTrick()
{
	CCLog("Call Game::nextTrick");

	turn = evaluateTrickWin();
	tricks[turn]++; // increment number of tricks taken

	trickCardCount = 0;
	suitLed = BLANK;
	played[0] = false; // set that no one has played in the next trick
	played[1] = false;
	played[2] = false;
	played[3] = false;

	gameScene->removePlayedCards(turn);
	gameScene->onSetScore(turn, bid[turn], tricks[turn]);

	// update numbers of cards player
	closedCardCount += 4;

	if (closedCardCount == 52) {
		state = eGS_END_ROUND;
	} else {
		state = eGS_PLAYCARD;
	}
}

void Game::endRound()
{
	CCLog("Call Game::endRound");
	// update scores at end of round
	scores[0] += score(0);
	scores[1] += score(1);

	if (scores[0] >= MAX_SCORE || scores[1] >= MAX_SCORE) {
		if (scores[0] > scores[1]) { // win
			gameScene->onGameFinished(true);
		} else if (scores[0] < scores[1]) { // lose
			gameScene->onGameFinished(false);
		} else { // tie
			gameScene->onRoundFinished();
		}
	} else {
		gameScene->onRoundFinished();
	}
}

// Adds played card to next available position in playedCards array
void Game::addCardPlayed(Suit suit, int value)
{
	int index = closedCardCount + trickCardCount;
	playedCards[index].suit = suit;
	playedCards[index].value = value;
	trickCardCount ++;
}

// Return turn of who win the trick
int Game::evaluateTrickWin()
{
	CCLog("Call Game::evaluateTrickWin");

	Suit suitLed = playedCards[closedCardCount].suit;
	int highest_index = 0;
	int highest_value = calc_card_value(playedCards[highest_index + closedCardCount]);
	int current_value;
	int winner;

	for (int i = 1; i < 4; i++) {
		if (playedCards[i + closedCardCount].suit == suitLed || playedCards[i + closedCardCount].suit == SPADE) {
			current_value = calc_card_value(playedCards[i + closedCardCount]);
			if (current_value > highest_value) {
				highest_value = current_value;
				highest_index = i;
			}
		}
	}

	winner = turn + highest_index;
	winner %= PLAYER_NUM;
	return winner;
}

// Returns the score from the end of a round
int Game::score(int pair)
{
	int score = 0;
	int bids = bid[pair] + bid[pair + 2];
	int trick = tricks[pair] + tricks[pair + 2];

	// Nil bids
	if (bid[pair] == 0) {
		if (tricks[pair] == 0) {
			score += ((doubleNil && pair == 0) ? 200 : 100);
		} else {
			score -= ((doubleNil && pair == 0) ? 200 : 100);
		}
	}

	if (bid[pair + 2] == 0) {
		if (tricks[pair + 2] == 0) {
			score += 100;
		} else {
			score -= 100;
		}
	}

	// exact number of tricks
	if (bids == trick) {
		score += (10 * bids);
	} else if (bids < trick) {
		int bag = (trick - bids);

		// too many tricks
		score += (10 * bids + bag);

		// bags overflow 10
		bags[pair] += bag;
		if (bags[pair] >= 10) {
			score -= 100;
			bags[pair] -= 10;
		}
	} if ( bids > trick) {
		// not enough tricks
		score -= (10 * bids);
	}

	return score;
}

Card Game::thinkCard(Hand *hand)
{
	CCLog("Call Game::thinkCard");

	switch (suitLed) {
	case SPADE:
	case DIAMOND:
	case CLOVER:
	case HEART:
		if (hand->hasSuit(suitLed)) {
			return thinkCardInSuit(hand, suitLed);
		} else {
			return thinkCardOutSuit(hand, suitLed);
		}
		break;

	default:
		// where suit is BLANK
		return thinkCardFirst(hand);
	}
}

Card Game::thinkCardInSuit(Hand *hand, Suit suit)
{
	Card card;
	card.suit = suit;

	int highestIndex = highestCardIndexInTrick(suit);
	if (highestIndex == -1) {
		// never called, only for no exception
		card.value = hand->highestCard(suit);
		return card;
	}

	// if self is nil, play highest card small than highest card
	if (bid[turn] == 0) {
		if (playedCards[highestIndex].suit == SPADE && suit != SPADE) {
			card.value = hand->highestCard(suit);
			return card;
		} else {
			card.value = hand->nearestCard(suit, playedCards[highestIndex].value);
			return card;
		}
	}

	// if already played spade, play lowest card
	if (playedCards[highestIndex].suit == SPADE && suit != SPADE) {
		card.value = hand->lowestCard(suit);
		return card;
	}

	// if team is nil, play card large than highest card
	int team = (turn + 2) % 4;
	if (bid[team] == 0 && tricks[team] == 0) {
		// if last play, play high card
		if (trickCardCount == 3) {
			int highValue = hand->highCard(suit, playedCards[highestIndex].value);
			if (highValue > 0) {
				card.value = highValue;
				return card;
			}
		}

		int value = hand->highestCard(suit);
		if (value < playedCards[highestIndex].value) {
			card.value = hand->lowestCard(suit);
		} else {
			card.value = value;
		}
		return card;
	}

	// if team is win, play lowest card
	if (highestIndex == closedCardCount + trickCardCount - 2) {
		card.value = hand->lowestCard(suit);
		return card;
	}

	// if other team is nil, play lowest card
	int other = (turn + 1) % 4;
	if (bid[other] == 0 && tricks[other] == 0 && !played[other]) {
		card.value = hand->lowestCard(suit);
		return card;
	}

	// if other team is nil, play lowest card
	other = (turn + 3) % 4;
	if (bid[other] == 0 && tricks[other] == 0 && !played[other]) {
		card.value = hand->lowestCard(suit);
		return card;
	}

	// if last play, play high card
	if (trickCardCount == 3) {
		int highValue = hand->highCard(suit, playedCards[highestIndex].value);
		if (highValue > 0) {
			card.value = highValue;
			return card;
		}
	}

	// if have highest card, play it
	if (isHighestCard(suit, hand->highestCard(suit))) {
		card.value = hand->highestCard(suit);
		return card;
	} else {
		card.value = hand->lowestCard(suit);
		return card;
	}
}

Card Game::thinkCardOutSuit(Hand *hand, Suit suit)
{
	Card card;
	emptySuit[turn][suit_index(suit)] = true;

	// if led is spade, play lowest card
	if (suit == SPADE) {
		return thinkLowestCardOutSuit(hand, suit);
	}

	enum::Suit suits[4] = {HEART, CLOVER, DIAMOND, SPADE};

	// if self is nil, play highest card
	if (bid[turn] == 0) {
		int highValue = 0;
		for (int i = 0; i < 3; i++) {
			if (suits[i] != suit && hand->hasSuit(suits[i])) {
				int value = hand->highestCard(suits[i]);
				if (value > highValue) {
					highValue = value;
					card.suit = suits[i];
					card.value = value;
				}
			}
		}

		if (highValue > 0) {
			return card;
		}
	}

	bool hasSpade = hand->hasSuit(SPADE);

	int highestIndex = highestCardIndexInTrick(suit);
	if (highestIndex == -1) {
		// never called, only for no exception
		return thinkLowestCardOutSuit(hand, suit);
	}

	// if team is nil, play highest spade
	int team = (turn + 2) % 4;
	if (bid[team] == 0 && tricks[team] == 0 && hasSpade) {
		// is highest played, is lowest card
		if (playedCards[highestIndex].suit == SPADE) {
			int highValue = hand->highCard(SPADE, playedCards[highestIndex].value);
			if (highValue > 0) {
				card.suit = SPADE;
				card.value = highValue;
				return card;
			} else {
				return thinkLowestCardOutSuit(hand, suit);
			}
		}

		card.suit = SPADE;
		card.value = hand->lowestCard(SPADE);
		return card;
	}

	// if team win, play lowest card
	if (highestIndex == closedCardCount + trickCardCount - 2) {
		return thinkLowestCardOutSuit(hand, suit);
	}

	// if other team is nil, play lowest card
	int other = (turn + 1) % 4;
	if ((bid[other] == 0 && tricks[other] == 0) && !played[other]) {
		return thinkLowestCardOutSuit(hand, suit);
	}

	// if other team is nil, play lowest card
	other = (turn + 3) % 4;
	if (bid[other] == 0 && tricks[other] == 0 && !played[other]) {
		return thinkLowestCardOutSuit(hand, suit);
	}

	// if has spade, play lowest spade
	if (hasSpade) {
		bool canSpade = true;
		for (int i = 0; i < 4; i++) {
			if (i != turn && !played[i] && emptySuit[i][suit_index(suit)] && !emptySuit[i][suit_index(SPADE)]) {
				canSpade = false;
				break;
			}
		}

		if (canSpade) {
			if (playedCards[highestIndex].suit == SPADE) {
				// if no have high spade, play lowest other card
				int highValue = hand->highCard(SPADE, playedCards[highestIndex].value);
				if (highValue > 0) {
					card.suit = SPADE;
					card.value = highValue;
					return card;
				}
			} else {
				card.suit = SPADE;
				card.value = hand->lowestCard(SPADE);
				return card;
			}
		}
	}

	return thinkLowestCardOutSuit(hand, suit);
}

Card Game::thinkCardFirst(Hand *hand)
{
	Card card;
	enum::Suit suits[4] = {HEART, CLOVER, DIAMOND, SPADE};

	// if self is nil, play lowest card
	if (bid[turn] == 0) {
		int lowValue = 1000;
		for (int i = 0; i < 3; i++) {
			if (hand->hasSuit(suits[i])) {
				int value = hand->lowestCard(suits[i]);
				if (value < lowValue) {
					lowValue = value;
					card.suit = suits[i];
					card.value = value;
				}
			}
		}

		if (lowValue < 1000) {
			suitLed = card.suit;
			return card;
		}
	}

	// if team is nil, play highest
	int team = (turn + 2) % 4;
	if (bid[team] == 0 && tricks[team] == 0) {

		bool canSpade = spadeBroken && hand->hasSuit(SPADE);
		int suitCount = canSpade ? 4 : 3;

		// if team have only spade, must play highest spade
		if (canSpade && emptySuit[team][0] && emptySuit[team][1] && emptySuit[team][2]) {
			suitLed = SPADE;
			card.suit = SPADE;
			card.value = hand->highestCard(SPADE);
			return card;
		}

		// if have highest card, play it
		for (int i = 0; i < suitCount; i++) {
			if (hand->hasSuit(suits[i])) {
				int value = hand->highestCard(suits[i]);
				if (isHighestCard(suits[i], value)) {
					suitLed = suits[i];
					card.suit = suits[i];
					card.value = value;
					return card;
				}
			}
		}

		// if haven't highest, play void team suit
		for (int i = 0; i < suitCount; i++) {
			Suit suit = get_suit(i);
			if (emptySuit[team][i] && hand->hasSuit(suit)) {
				suitLed = suit;
				card.suit = suit;
				card.value = hand->highestCard(suit);
				return card;
			}
		}

		// if have only me & team
		int highValue = 0;
		for (int i = 0; i < suitCount; i++) {
			Suit suit = get_suit(i);
			if ((!emptySuit[(turn + 1) % 4][i] || !emptySuit[(turn + 3) % 4][i]) && hand->hasSuit(suit)) {
				int value = hand->highestCard(suits[i]);
				if (value > highValue) {
					highValue = value;
					card.suit = suits[i];
					card.value = value;
				}
			}
		}

		if (highValue > 0) {
			suitLed = card.suit;
			return card;
		}

		// try highest
		highValue = 0;
		for (int i = 0; i < suitCount; i++) {
			if (hand->hasSuit(suits[i])) {
				int value = hand->highestCard(suits[i]);
				if (value > highValue) {
					highValue = value;
					card.suit = suits[i];
					card.value = value;
				}
			}
		}

		if (highValue > 0) {
			suitLed = card.suit;
			return card;
		}

		// if have only spade
		if (hand->hasSuit(SPADE)) {
			suitLed = SPADE;
			card.suit = SPADE;
			card.value = hand->highestCard(SPADE);
			return card;
		}

		// never called
		CCLog("Game::thinkCardFirst error when nil");
	}

	// if other team is nil, play lowest card
	int player1 = (turn + 1) % 4, player2 = (turn + 3) % 4;
	if ((bid[player1] == 0 && tricks[player1] == 0) || (bid[player2] == 0 && tricks[player2] == 0)) {
		int lowValue = 1000;
		for (int i = 0; i < 3; i++) {
			if (hand->hasSuit(suits[i])) {
				int value = hand->lowestCard(suits[i]);
				if (value < lowValue) {
					lowValue = value;
					card.suit = suits[i];
					card.value = value;
				}
			}
		}

		if (lowValue < 1000) {
			suitLed = card.suit;
			return card;
		}
	}

	// if have highest card, play highest card
	for (int i = 0; i < 3; i++) {
		if (hand->hasSuit(suits[i])) {
			int value = hand->highestCard(suits[i]);
			if (isHighestCard(suits[i], value)) {
				// if anyone be expected to spade, save highest card
				bool enableHighest = true;
				for (int j = 0; j < 4; j++) {
					if (j != turn && emptySuit[j][i] && !emptySuit[3]) {
						enableHighest = false;
						break;
					}
				}

				if (enableHighest) {
					card.suit = suits[i];
					card.value = value;
					suitLed = suits[i];
					return card;
				}
			}
		}
	}

	// Play highest spade if broken and other suit can be trumped and if have highest spade in the game at this point
	if (spadeBroken && hand->hasSuit(SPADE) && isHighestCard(SPADE, hand->highestCard(SPADE))) {
		card.suit = SPADE;
		card.value = hand->highestCard(SPADE);
		suitLed = SPADE;
		return card;
	}

	// If no sure wins, lead lowest cards in order
	for (int i = 0; i < 3; i++) {
		if (hand->hasSuit(suits[i])) {
			card.suit = suits[i];
			card.value = hand->lowestCard(suits[i]);
			suitLed = suits[i];
			return card;
		}
	}

	// If void everything else, lead spade suit
	if (hand->hasSuit(SPADE)) {
		card.suit = SPADE;
		card.value = hand->lowestCard(SPADE);
		suitLed = SPADE;
		return card;
	}

	// never called
	CCLog("Game::thinkCardFirst error");
	return card;
}

Card Game::thinkLowestCardOutSuit(Hand *hand, Suit suit)
{
	enum::Suit suits[4] = {HEART, CLOVER, DIAMOND, SPADE};
	Card card;

	int suitCount = spadeBroken ? 4 : 3;
	int lowValue = 1000;
	for (int i = 0; i < suitCount; i++) {
		if (suit != suits[i] && hand->hasSuit(suits[i])) {
			int value = hand->lowestCard(suits[i]);
			if (value < lowValue) {
				lowValue = value;
				card.suit = suits[i];
				card.value = value;
			}
		}
	}

	if (lowValue < 1000) {
		return card;
	}

	if (hand->hasSuit(SPADE)) {
		card.suit = SPADE;
		card.value = hand->lowestCard(SPADE);
		return card;
	}

	// never called
	CCLog("Game::thinkLowestCardOutSuit error");
	return card;
}

int Game::highestCardIndexInTrick(Suit suit)
{
	int highIndex = -1;
	int highValue = 0;
	for (int i = 0; i < trickCardCount; i++) {
		if (playedCards[i + closedCardCount].suit == suit || playedCards[i + closedCardCount].suit == SPADE) {
			int value = calc_card_value(playedCards[i + closedCardCount]);
			if (value > highValue) {
				highValue = value;
				highIndex = i + closedCardCount;
			}
		}
	}

	return highIndex;
}

// Returns true if all the cards in a certain suit above a certain value have
// been played. Else, returns false.
bool Game::isHighestCard(Suit asuit, int avalue)
{
	bool flag = false;

	for (int i = 14; i > avalue; i--) {
		flag = false;
		for (int j = 0; j < closedCardCount; j++) {
			if (playedCards[j].suit == asuit && playedCards[j].value == i) {
				flag = true;
			}
		}

		if (flag == false) {
			return false;
		}
	}
	return true;
}
