#ifndef _HAND_H_
#define _HAND_H_

#include "common.h"

class CardDeck;

class Hand
{
	Card cards[PLAYER_CARD_NUM+1];                            // defines a hand (1 space in end for overshooting index)
public:
	Hand() {}
	virtual ~Hand() {};

	void setHand(const Hand *hand);

	Card card(int i) { return cards[i]; }
	Suit cardSuit(int i);
	int cardValue(int i);
	int cardIndex(int i);

	int getCardIndex(Card card);

	void setHand(CardDeck *deck, int index);
	void organizeHand();

	void applyRule(Suit suitLed);

	int findCard(Suit suit, int value);
	int countSuit(Suit suit);
	bool hasSuit(Suit suit);

	// compare
	bool highestCardOfAll(Suit* suit, int* value);

	int highestCard(Suit suit);
	int lowestCard(Suit suit);
	int nearestCard(Suit suit, int value);
	int highCard(Suit suit, int value);
	int lowCard(Suit suit, int value);
	int lowCardExcept(Suit suit, int value, int except);
	int highCardExcept(Suit suit, int value, int except);
	int highestCardExcept(Suit suit, int value);

	void removeCard(Suit suit, int value);
	bool empty();

	int countLow(int value);
	int countHigh(int value);
	int countEqual(int value);
};

#endif //_HAND_H_
