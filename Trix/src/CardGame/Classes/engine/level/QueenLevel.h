#ifndef _QUEEN_LEVEL_H_
#define _QUEEN_LEVEL_H_

#include "Level.h"
#include "MainTable.h"

class QueenLevel : public Level
{
public:
	QueenLevel();
	virtual ~QueenLevel();

	virtual bool evaluateScore(int turn);

	virtual bool getDuplicateList(int player, OUT int& duplicates);
	virtual void setDuplicate(int player, int duplicates);
	virtual bool thinkDuplicate(int player, OUT int& duplicates);

	virtual void setTable(Table *table) { this->table = (MainTable *) table; };
	void setDuplicate(Suit suit, int player) { duplicate[suit_index(suit)] = player; }

protected:
	virtual Card thinkCardInSuit(Hand *hand, Suit suit, int turn);
	virtual Card thinkCardOutSuit(Hand *hand, Suit suit, int turn);
	virtual Card thinkCardFirst(Hand *hand, int turn);

	virtual Table* getTable() { return table; }

private:
	Card highestCardExcept(Hand *hand, Suit suit);
	int  nearestCard(Hand *hand, Suit suit, int highestOnTable);

private:
	int duplicate[4];
	int queenSuit;
	MainTable *table;
};

#endif //_QUEEN_LEVEL_H_
