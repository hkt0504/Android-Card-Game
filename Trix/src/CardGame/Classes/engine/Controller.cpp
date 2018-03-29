#include "Controller.h"
#include "GameScene.h"
#include "Hand.h"
#include "Player.h"
#include "GameResult.h"
#include "CardDeck.h"
#include "MainTable.h"
#include "TrixTable.h"
#include "Level.h"
#include "KingLevel.h"
#include "QueenLevel.h"
#include "DiamondLevel.h"
#include "LotoshLevel.h"
#include "TrixLevel.h"
#include "ComplexLevel.h"

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

	partner = false;
	complex = false;
	level = NULL;
	table = NULL;
	startTurn = -1;
	bidCount = 0;
	showDue = false;

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

void Controller::setPlayMode(bool partner, bool complex)
{
	this->partner = partner;
	this->complex = complex;
	for (int i = 0; i < PLAYER_NUM; i++) {
		players[i]->setMode(complex);
	}
}

GameResult* Controller::getGameResult()
{
	return gameResult;
}

void Controller::getEatCard(EatCard eatCard[])
{
	for (int i = 0; i < 4; i++) {
		eatCard[i].score = players[i]->score;
		eatCard[i].eatKQ = players[i]->eatKQ;
		eatCard[i].eatD = players[i]->eatDiamond;
		eatCard[i].eatL = players[i]->eatLotosh;
	}
}

void Controller::step()
{
	//CCLog("Call Controller::step");

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
	case eGS_DUPLICATE:
		processDuplicate();
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
	bid = BT_NONE;

	for (int i = 0; i < PLAYER_NUM; i++) {
		players[i]->clear();
	}

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

	// if i have heart 7, first bid.
	if (startTurn == -1) {
		for (int i = 0; i < 4; i++) {
			if (players[i]->hand->findCard(HEART, CARD_7) != INVALID) {
				startTurn = i;
				break;
			}
		}
		gameResult->addKingdom(startTurn);
	}

	if (startTurn == 0) {
		gameScene->onWantBid(players[0]->bid);
	} else {
		// computer players' bids
		bid = players[startTurn]->thinkBid();

		createLevel();
		gameScene->onShowBid(startTurn, bid);

		state = (bid == BT_COMPLEX || bid == BT_KING || bid == BT_QUEEN || bid == BT_TRIX) ? eGS_DUPLICATE : eGS_PREPARE_PLAY;
	}
}

void Controller::setUserBid(BidType value)
{
	CCLog("Call Controller::setBid");

	bid = value;
	REMOVE_BIT(players[0]->bid, bid);

	createLevel();
	gameScene->onShowBid(0, bid);

	state = (bid == BT_COMPLEX || bid == BT_KING || bid == BT_QUEEN || bid == BT_TRIX) ? eGS_DUPLICATE : eGS_PREPARE_PLAY;
}

void Controller::processDuplicate()
{
	if (bid == BT_TRIX) {
		int duplicates = DUP_NONE;
		for (int i = 0; i < 4; i++) {
			if (level->thinkDuplicate(i, duplicates)) {
				players[i]->duplicates = duplicates;
				level->setDuplicate(i, duplicates);
			}
		}

		if (partner) {
			if ((players[0]->duplicates | players[2]->duplicates) == DUP_ALL_DUE ||
				(players[1]->duplicates | players[3]->duplicates) == DUP_ALL_DUE) {
				showDue = false;
			} else {
				showDue = true;
				for (int i = 0; i < 4; i ++) {
					if (players[i]->duplicates != DUP_NONE) {
						gameScene->onShowDuplicate(i, players[i]->duplicates);
					}
				}
			}
		}
		state = eGS_PREPARE_PLAY;
	} else {
		int duplicates = DUP_NONE;
		if (level->getDuplicateList(0, duplicates)) {
			gameScene->onWantDuplicate(duplicates);
		} else {
			thinkDuplicate();
		}
	}
}

void Controller::setUserDuplicate(int duplicates)
{
	players[0]->duplicates = duplicates;
	level->setDuplicate(0, duplicates);
	gameScene->onShowDuplicate(0, duplicates);
	thinkDuplicate();
}

void Controller::thinkDuplicate()
{
	int duplicates = DUP_NONE;
	if (level->thinkDuplicate(1, duplicates)) {
		players[1]->duplicates = duplicates;
		level->setDuplicate(1, duplicates);
		gameScene->onShowDuplicate(1, duplicates);
	}

	if (level->thinkDuplicate(2, duplicates)) {
		players[2]->duplicates = duplicates;
		level->setDuplicate(2, duplicates);
		gameScene->onShowDuplicate(2, duplicates);
	}

	if (level->thinkDuplicate(3, duplicates)) {
		players[3]->duplicates = duplicates;
		level->setDuplicate(3, duplicates);
		gameScene->onShowDuplicate(3, duplicates);
	}

	state = eGS_PREPARE_PLAY;
}

void Controller::preparePlay()
{
	turn = startTurn;
	state = eGS_PLAYCARD;
}

void Controller::setCardPlayed(int playerIndex, int cardIndex)
{
	CCLog("Call Controller::setCardPlayed");

	if (turn == 0) {
		gameScene->onTouchEnable(false);
		playUserCard(cardIndex);
	}

	if (bid == BT_TRIX) {
		int duplicates = players[turn]->duplicates;
		int score = players[turn]->score;
		bool finish = level->evaluateScore(turn);

		int move = players[turn]->score - score;
		if (move != 0) {
			gameScene->onShowScore(turn, players[turn]->score, move);
		}

		if (partner && showDue && duplicates != players[turn]->duplicates) {
			gameScene->onShowDuplicate(turn, players[turn]->duplicates);
		}

		players[turn]->played = false;

		if (finish || table->isAllPlayed()) {
			state = eGS_END_ROUND;
			return;
		}
	}

	nextTurn();
}

void Controller::nextTurn()
{
	CCLog("Call Controller::nextTurn");

	turn ++;
	turn %= PLAYER_NUM;

	if (bid == BT_TRIX) {
		if (players[turn]->hand->empty()) {
			players[turn]->played = true;
			nextTurn();
		} else {
			if (turn == 0) {
				prepareUserCard();
			} else {
				state = eGS_PLAYCARD;
			}
		}
	} else {
		// check complete trick
		if (players[0]->played && players[1]->played && players[2]->played && players[3]->played) {
			idleCount = 1;
			state = eGS_NEXT_TRICK;
		} else {
			if (turn == 0) {
				prepareUserCard();
			} else {
				state = eGS_PLAYCARD;
			}
		}
	}
}

void Controller::playCard()
{
	CCLog("Call Controller::playCard s");

	if (turn == 0) {
		prepareUserCard();
	} else {
		Hand* hand = players[turn]->hand;
		tempHand->setHand(hand);

		if (bid == BT_TRIX) {
			Card card = level->thinkCard(tempHand, turn);
			if (card.suit == BLANK) {
				players[turn]->played = true;
				gameScene->onPlayTurn(turn, true);
				nextTurn();
			} else {
				int cardIndex = hand->getCardIndex(card);
				if (cardIndex < 0) {
					CCLog("Call Controller::playCard error detail card{%d, %d}", card.suit, card.value);
				} else {
					table->addCardPlayed(card.suit, card.value);
					hand->removeCard(card.suit, card.value);
					players[turn]->played = true;

					gameScene->onPlayCard(turn, cardIndex);
					gameScene->onPlayTurn(turn);
				}
			}
		} else {
			tempHand->applyRule(level->suitLed);
			Card card = level->thinkCard(tempHand, turn);

			int cardIndex = hand->getCardIndex(card);
			if (cardIndex < 0) {
				CCLog("Call Controller::playCard error detail card{%d, %d}", card.suit, card.value);
			} else {
				table->addCardPlayed(card.suit, card.value);
				hand->removeCard(card.suit, card.value);
				players[turn]->played = true;

				gameScene->onPlayCard(turn, cardIndex);
				gameScene->onPlayTurn(turn);
			}
		}
	}

	CCLog("Call Controller::playCard e");
}

void Controller::prepareUserCard()
{
	CCLog("Call Controller::prepareUserCard s");
	if (bid == BT_TRIX) {
		Hand* hand = players[0]->hand;

		Card cards[12];
		int count = 0;
		((TrixLevel *)level)->getEnabledCards(hand, cards, count);

		CCLog("Call Controller::prepareUserCard count %d", count);
		if (count == 0) {
			// next player
			players[turn]->played = true;
			gameScene->onPlayTurn(0, true);
			nextTurn();
		} else {
			gameScene->onTouchEnable(cards, count);
			gameScene->onPlayTurn(0);
		}
	} else {
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

		gameScene->onTouchEnable(enabledSuit, true);
		gameScene->onPlayTurn(0);
	}
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
	CCLog("Call Controller::nextTrick s");

	gameScene->onPlayTurn(INVALID);

	bool finish = false;
	int scores[4];
	int duplicates[4];
	for (int i = 0; i < PLAYER_NUM; i++) {
		scores[i] = players[i]->score;
		duplicates[i] = players[i]->duplicates;
	}

	finish = level->evaluateScore(turn);
	turn = table->evaluateTrickWin(turn);
	level->suitLed = BLANK;

	table->nextTrick();

	gameScene->removePlayedCards(turn);
	for (int i = 0; i < 4; i++) {
		int move = players[i]->score - scores[i];
		if (move != 0) {
			gameScene->onShowScore(i, players[i]->score, move);
		}

		if (duplicates[i] != players[i]->duplicates) {
			gameScene->onShowDuplicate(i, players[i]->duplicates);
		}
		players[i]->played = false;
	}

	if (finish || table->isAllPlayed()) {
		state = eGS_END_ROUND;
	} else {
		state = eGS_PLAYCARD;
	}

	CCLog("Call Controller::nextTrick e");
}

void Controller::endRound()
{
	CCLog("Call Controller::endRound s");

	gameResult->addRoundScore(bid, players[0]->score, players[1]->score, players[2]->score, players[3]->score);
	bool noHaveBid = players[startTurn]->noHaveBid();
	if (noHaveBid) {
		startTurn ++;
		startTurn %= PLAYER_NUM;
		bidCount ++;
	}

	if (bidCount >= PLAYER_NUM) {
		bool win = false;
		int score0 = gameResult->getTotalScore(0);
		int score1 = gameResult->getTotalScore(1);
		int score2 = gameResult->getTotalScore(2);
		int score3 = gameResult->getTotalScore(3);

		if (partner) {
			win = (score0 + score2 > score1 + score3);
		} else {
			win = (score0 >= score1 && score0 >= score2 && score0 >= score3);
		}

		// game finished
		if (win) { // win
			gameScene->onGameFinished(true);
		} else { // lose
			gameScene->onGameFinished(false);
		}
	} else {
		gameScene->onRoundFinished();
		if (noHaveBid) {
			gameResult->addKingdom(startTurn);
		}
	}

	CCLog("Call Controller::endRound e");
}

void Controller::createLevel()
{
	SAFE_DELETE(level);
	SAFE_DELETE(table);

	switch (bid) {
	case BT_KING:
		table = new MainTable();
		level = new KingLevel();
		break;
	case BT_QUEEN:
		table = new MainTable();
		level = new QueenLevel();
		break;
	case BT_DIAMOND:
		table = new MainTable();
		level = new DiamondLevel();
		break;
	case BT_LOTOSH:
		table = new MainTable();
		level = new LotoshLevel();
		break;
	case BT_TRIX:
		table = new TrixTable();
		level = new TrixLevel();
		break;
	case BT_COMPLEX:
		table = new MainTable();
		level = new ComplexLevel();
		break;
	default: // error case
		break;
	}

	level->setTable(table);
	level->setPlayers(players);
	level->setPartner(partner);
}
