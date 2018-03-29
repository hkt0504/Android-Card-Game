#ifndef _LOTOSH_LEVEL_H_
#define _LOTOSH_LEVEL_H_

#include "Level.h"
#include "MainTable.h"

class LotoshLevel : public Level
{
public:
	LotoshLevel();
	virtual ~LotoshLevel();

	virtual void setTable(Table *table) { this->table = (MainTable *) table; };
	virtual bool evaluateScore(int turn);

protected:
	virtual Card thinkCardInSuit(Hand *hand, Suit suit, int turn);
	virtual Card thinkCardOutSuit(Hand *hand, Suit suit, int turn);
	virtual Card thinkCardFirst(Hand *hand, int turn);

	virtual Table* getTable() { return table; }

private:
	int nearestCard(Hand *hand, Suit suit, int highestOnTable);

private:
	int playedCount;
	MainTable *table;
};

#endif //_LOTOSH_LEVEL_H_
