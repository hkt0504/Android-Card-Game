#ifndef _TRIX_TABLE_H_
#define _TRIX_TABLE_H_

#include "Table.h"

/** management of played cards for Trix */
class TrixTable : public Table
{
public:
	TrixTable();
	virtual ~TrixTable() {};

	virtual void addCardPlayed(Suit suit, int value);
	virtual bool isAllPlayed() { return cardCount == CARD_NUM; };

	int minCard(Suit suit);
	int maxCard(Suit suit);

public:
	Card playedCards[CARD_NUM];

	int cardCount;
};

#endif //_TABLE_H_
