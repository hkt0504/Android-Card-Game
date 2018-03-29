#ifndef _CARD_DECK_H_
#define _CARD_DECK_H_

#include "common.h"

class CardDeck
{
	Card cards[CARD_NUM];                              // defines a deck

public:
	CardDeck();
	virtual ~CardDeck() {};

	void shuffle();

	Suit cardSuit(int i);
	int cardValue(int i);
};

#endif //_CARD_DECK_H_
