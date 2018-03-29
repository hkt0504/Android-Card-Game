#include "Controller.h"
#include "GameScene.h"
#include "Hand.h"
#include "Player.h"
#include "GameResult.h"
#include "CardDeck.h"
#include "Table.h"
#include "Level.h"

USING_NS_CC;

bool Controller::init()
{
	if (!Node::init()) {
		return false;
	}

	for (int i = 0; i < PLAYER_NUM; i++) {
		players[i] = new Player();
	}

	deck = new CardDeck();
	tempHand = new Hand();
	gameResult = new GameResult();

	table = new Table();
	level = new Level();
	level->setTable(table);
	level->setPlayers(players);

	bidder = -1;

	initRound();

	return true;
}

Controller::~Controller()
{
	SAFE_DELETE(level);
	SAFE_DELETE(deck);
	SAFE_DELETE(tempHand);
	SAFE_DELETE(table);
	SAFE_DELETE(gameResult);

	for (int i = 0; i < PLAYER_NUM; i++) {
		SAFE_DELETE(players[i]);
	}
}

GameResult* Controller::getGameResult()
{
	return gameResult;
}

Hand *Controller::getHand(int i)
{
	return players[i]->hand;
}

void Controller::step()
{
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

void Controller::newRound()
{
	CCLog("Call Controller::newRound");
	state = eGS_NEW_ROUND;
}

void Controller::initRound()
{
	turn = 0;

	bidValue = 0;
	tarneeb = SPADE;
	bidCount = 0;
	passCount = 0;

	//set bidder
	if (bidder == -1) {
		srand(time(0));
		bidder = rand() % PLAYER_NUM;
	} else {
		bidder ++;
		bidder %= PLAYER_NUM;
	}

	for (int i = 0; i < PLAYER_NUM; i++) {
		players[i]->clear();
	}

	// setup the cards played
	table->clear();

	level->suitLed = BLANK;

	state = eGS_SHUFFLE;
}

void Controller::shuffle()
{
	CCLog("Call Controller::shuffle");

	deck->shuffle();
	for (int i = 0; i < PLAYER_NUM; i++) {
		players[i]->hand->setHand(deck, i);
		players[i]->hand->organizeHand();
	}

	for (int i = 0; i < PLAYER_NUM; i++) {
		gameScene->onSetHand(i, players[i]->hand);
	}
}

void Controller::processBid()
{
	CCLog("Call Controller::processBid");

	if (bidCount >= PLAYER_NUM) {
		if (players[bidder]->bid != 0 && players[bidder]->bid < bidValue) {
			if (bidder == 0) {
				gameScene->onWantBid(bidValue == 0 ? 7 : bidValue + 1);
			} else {
				players[bidder]->thinkBid(bidValue);
				afterBid();
			}
		} else {
			passCount ++;
			if (passCount >= 3) {
				for (int i = 0; i < PLAYER_NUM; i++) {
					if (players[i]->bid != 0) {
						bidder = i;
						if (bidder == 0) {
							gameScene->onWantTarneeb();
						} else {
							state = eGS_PREPARE_PLAY;
						}
						return;
					}
				}
				gameScene->newRound();
			} else {
				afterBid();
			}
		}
	} else {
		if (bidder == 0) {
			gameScene->onWantBid(bidValue == 0 ? 7 : bidValue + 1);
		} else {
			players[bidder]->thinkBid(bidValue);
			afterBid();
		}
	}
}

void Controller::afterBid() {
	gameScene->onShowBid(bidder, players[bidder]->bid);

	if (players[bidder]->bid == 13) {
		bidValue = 13;
		if (bidder == 0) {
			gameScene->onWantTarneeb();
		} else {
			tarneeb = players[bidder]->tarneeb;
			state = eGS_PREPARE_PLAY;
		}
	} else {
		if (players[bidder]->bid > bidValue) {
			bidValue = players[bidder]->bid;
			tarneeb = players[bidder]->tarneeb;
		}

		bidCount ++;
		bidder++;
		bidder %= PLAYER_NUM;
		state = eGS_BID;
	}
}

void Controller::setBid(int value)
{
	CCLog("Call Controller::setBid");

	players[0]->bid = value;
	bidder = 0;
	afterBid();
}

void Controller::setTarneeb(Suit suit)
{
	CCLog("Call Controller::setTarneeb");

	tarneeb = suit;
	players[0]->tarneeb = suit;
	state = eGS_PREPARE_PLAY;
}

void Controller::preparePlay()
{
	gameScene->onFinishBid();
	gameScene->onShowTarneeb(tarneeb);
	turn = bidder;
	state = eGS_PLAYCARD;
}

void Controller::setCardPlayed(int playerIndex, int cardIndex)
{
	CCLog("Call Controller::setCardPlayed");

	if (turn == 0) {
		gameScene->onTouchEnable(false);
		playUserCard(cardIndex);
	}

	turn ++;
	turn %= PLAYER_NUM;

	// check complete trick
	if (players[0]->played && players[1]->played && players[2]->played && players[3]->played) {
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

void Controller::playCard()
{
	CCLog("Call Controller::playCard");

	if (turn == 0) {
		gameScene->onTouchEnable(getEnabledCardSuit(), true);
		return;
	}

	Hand* hand = players[turn]->hand;
	tempHand->setHand(hand);
	tempHand->applyRule(level->suitLed);
	Card card = level->thinkCard(tempHand, turn, tarneeb);

	int cardIndex = hand->getCardIndex(card);
	if (cardIndex < 0) {
		CCLog("Call Controller::playCard error detail card{%d, %d}", card.suit, card.value);
		for (int i = 0; i < PLAYER_CARD_NUM; i++) {
			CCLog("Call Controller::playCard error hand information = {%d, %d}", hand->cardSuit(i), hand->cardValue(i));
		}
	} else {
		table->addCardPlayed(card.suit, card.value);
		hand->removeCard(card.suit, card.value);
		players[turn]->played = true;

		gameScene->onPlayCard(turn, cardIndex);
	}
}

int Controller::getEnabledCardSuit()
{
	Hand* hand = players[0]->hand;
	tempHand->setHand(hand);
	tempHand->applyRule(level->suitLed);

	int enabledSuit = 0;
	for (int i = 0; i < PLAYER_CARD_NUM; i++) {
		Suit suit = tempHand->cardSuit(i);
		if (suit != BLANK) {
			enabledSuit |= suit;
		}
	}
	return enabledSuit;
}

void Controller::playUserCard(int cardIndex)
{
	CCLog("Call Controller::playUserCard");

	// check if legal to play card
	Hand* hand = players[0]->hand;
	Suit suit = hand->cardSuit(cardIndex);
	int value = hand->cardValue(cardIndex);

	if (level->suitLed == BLANK) { // set leading suit if leading
		level->suitLed = suit;
	}

	table->addCardPlayed(suit, value);
	hand->removeCard(suit, value);
	players[0]->played = true;
}

void Controller::nextTrick()
{
	CCLog("Call Controller::nextTrick");

	turn = table->evaluateTrickWin(turn, tarneeb);
	players[turn]->tricks++; // increment number of tricks taken

	level->suitLed = BLANK;
	players[0]->played = false; // set that no one has played in the next trick
	players[1]->played = false;
	players[2]->played = false;
	players[3]->played = false;

	table->nextTrick();

	gameScene->removePlayedCards(turn);
	gameScene->onSetScore(turn, players[turn]->bid, players[turn]->tricks);

	if (table->isAllPlayed()) {
		state = eGS_END_ROUND;
	} else {
		state = eGS_PLAYCARD;
	}
}

void Controller::endRound()
{
	CCLog("Call Controller::endRound");

	// update scores at end of round
	score();

	int score1 = gameResult->getTotalScore(0);
	int score2 = gameResult->getTotalScore(1);

	if (score1 >= MAX_SCORE || score2 >= MAX_SCORE) {
		if (score1 > score2) { // win
			gameScene->onGameFinished(true);
		} else if (score1 < score2) { // lose
			gameScene->onGameFinished(false);
		} else { // tie
			gameScene->onRoundFinished();
		}
	} else {
		gameScene->onRoundFinished();
	}
}

// Returns the score from the end of a round
void Controller::score()
{
	int score1 = 0;
	int score2 = 0;
	int bidderTeam = players[bidder]->tricks + players[(bidder + 2) % PLAYER_NUM]->tricks;
	int bid = bidValue;

	if (bidderTeam >= bid) {
		if (bidder == 0 || bidder ==2) {
			score1 = bidderTeam;
			score2 = 0;
		} else {
			score1 = 0;
			score2 = bidderTeam;
		}
	} else {
		if (bidder == 0 || bidder ==2) {
			score1 = -bid;
			score2 = 13 - bidderTeam;
		} else {
			score1 = 13 - bidderTeam;
			score2 = -bid;
		}
	}

	gameResult->addRoundScore(score1, score2);
}
