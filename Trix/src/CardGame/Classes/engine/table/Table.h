#ifndef _TABLE_H_
#define _TABLE_H_

#include "common.h"

class Hand;

/** management of played cards */
class Table
{
public:
	Table() {};
	virtual ~Table() {};

	virtual void addCardPlayed(Suit suit, int value) {};
	virtual void nextTrick() {};
	virtual bool isAllPlayed() { return true; };

	virtual int evaluateTrickWin(int turn) { return 0; };
	virtual bool isLastPlayerInTrick() { return false; };
	virtual bool isFinishedSuit(Hand *hand, Suit suit) { return false; };

};

#endif //_TABLE_H_
