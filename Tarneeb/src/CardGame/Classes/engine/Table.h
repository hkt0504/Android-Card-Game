#ifndef _TABLE_H_
#define _TABLE_H_

#include "common.h"

class Hand;

class Table
{
public:
	Table();
	virtual ~Table();

public:
	void clear();

	void addCardPlayed(Suit suit, int value);
	Card getTrickCard(int index);

	int currentOfTrick() {return trickCardCount;}

	int evaluateTrickWin(int turn, Suit tarneeb);

	void nextTrick();
	bool isAllPlayed() { return (closedCardCount == CARD_NUM); }

	int highestCardIndexInTrick(Suit suit);
	bool isHighestCard(Suit suit, int value);

	bool isFinishedSuit(Hand *hand, Suit suit);

private:
	Card playedCards[CARD_NUM];

	// card count in current trick
	int trickCardCount;

	// card count while previous trick
	int closedCardCount;

	// played suit count
	int playedSuitCount[4];
};

#endif //_TABLE_H_
