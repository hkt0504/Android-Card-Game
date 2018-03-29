#ifndef _MAIN_TABLE_H_
#define _MAIN_TABLE_H_

#include "Table.h"

class Hand;

/** management of played cards for Diamond, King, Lotosh, Queen, Complex */
class MainTable : public Table
{
public:
	MainTable();
	virtual ~MainTable() {};

	virtual void addCardPlayed(Suit suit, int value);
	virtual void nextTrick();
	virtual int evaluateTrickWin(int turn);
	virtual bool isAllPlayed() { return closedCardCount == CARD_NUM; };
	virtual bool isLastPlayerInTrick() { return trickCardCount == 3; };
	virtual bool isFinishedSuit(Hand *hand, Suit suit);

	int highestCardInTrick(Suit suit, OUT int& player);
	int highestCardInTrick(Suit suit);

	bool findCardInTrick(Suit suit, int value);
	bool findSuitInTrick(Suit suit);
	bool findCardInTrick(int value);

	bool isHighestCard(Suit suit, int value);
	bool isLowestCard(Suit suit, int value);

	bool isTeamHighest(Suit suit);

public:
	Card playedCards[CARD_NUM];

	// card count while previous trick
	int closedCardCount;

	// card count in current trick
	int trickCardCount;

	// played suit count
	int playedSuitCount[4];
};
#endif //_MAIN_TABLE_H_
