#ifndef _DIAMOND_LEVEL_H_
#define _DIAMOND_LEVEL_H_

#include "Level.h"
#include "MainTable.h"


class DiamondLevel : public Level
{
public:
	DiamondLevel();
	virtual ~DiamondLevel();

	virtual void setTable(Table *table) { this->table = (MainTable *) table; };
	virtual bool evaluateScore(int turn);

protected:
	virtual Card thinkCardInSuit(Hand *hand, Suit suit, int turn);
	virtual Card thinkCardOutSuit(Hand *hand, Suit suit, int turn);
	virtual Card thinkCardFirst(Hand *hand, int turn);

	virtual Table* getTable() { return table; }

private:
	Card highestCardExcept(Hand *hand, Suit suit);
	int nearestCard(Hand *hand, Suit suit, int highestOnTable);

private:
	int playedDiamonds;
	MainTable *table;
};

#endif //_DIAMOND_LEVEL_H_
