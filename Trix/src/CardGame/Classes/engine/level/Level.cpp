#include "Level.h"
#include "Hand.h"
#include "Table.h"
#include "Player.h"

USING_NS_CC;

Level::Level()
{
	suitLed = BLANK;
	partner = false;
}

Level::~Level()
{
}

void Level::setPlayers(Player *players[])
{
	for (int i = 0; i < PLAYER_NUM; i++) {
		this->players[i] = players[i];
	}
}

bool Level::getDuplicateList(int player, OUT int& duplicates)
{
	duplicates = DUP_NONE;
	return false;
}

bool Level::thinkDuplicate(int player, OUT int& duplicates)
{
	duplicates = DUP_NONE;
	return false;
}

Card Level::thinkCard(Hand* hand, int turn)
{
	CCLog("Call Level::thinkCard start");

	Card card;

	switch (suitLed) {
	case SPADE:
	case DIAMOND:
	case CLOVER:
	case HEART:
		if (hand->hasSuit(suitLed)) {
			card = thinkCardInSuit(hand, suitLed, turn);
		} else {
			players[turn]->setEmptySuit(suitLed);
			card = thinkCardOutSuit(hand, suitLed, turn);
		}
		break;

	default:
		// where suit is BLANK
		card = thinkCardFirst(hand, turn);
		suitLed = card.suit;
		break;
	}

	CCLog("Call Level::thinkCard end");
	return card;
}

Card Level::thinkCardInSuit(Hand *hand, Suit suit, int turn)
{
	CCLog("Call Level::thinkCardInSuit");

	Card card;
	return card;
}

Card Level::thinkCardOutSuit(Hand *hand, Suit suit, int turn)
{
	CCLog("Call Level::thinkCardOutSuit");

	Card card;
	return card;
}

Card Level::thinkCardFirst(Hand *hand, int turn)
{
	CCLog("Call Level::thinkCardFirst");

	Card card;
	return card;
}

// Returns lowest card regardless of suit in hand
bool Level::lowestCardOfAll(Hand *hand, Card *card)
{
	card->suit = BLANK;
	card->value = CARD_MAX;

	for (int i = 0; i < PLAYER_CARD_NUM; i++) {
		Suit suit = hand->cardSuit(i);
		int value = hand->cardValue(i);
		if (suit != BLANK && !getTable()->isFinishedSuit(hand, suit) && value < card->value) {
			card->set(suit, value);
		}
	}

	if (card->suit != BLANK) {
		return true;
	}

	// i have only finished suit
	for (int i = 0; i < CARD_NUM; i++) {
		Suit suit = hand->cardSuit(i);
		int value = hand->cardValue(i);
		if (suit != BLANK) {
			card->set(suit, value);
			return true;
		}
	}

	return false;
}
