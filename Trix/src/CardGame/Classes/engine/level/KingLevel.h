#ifndef _KING_LEVEL_H_
#define _KING_LEVEL_H_

#include "Level.h"
#include "MainTable.h"

class KingLevel : public Level
{
public:
	KingLevel();
	virtual ~KingLevel();

	virtual bool evaluateScore(int turn);

	virtual bool getDuplicateList(int player, OUT int& duplicates);
	virtual void setDuplicate(int player, int duplicates);
	virtual bool thinkDuplicate(int player, OUT int& duplicates);

	virtual void setTable(Table *table) { this->table = (MainTable *) table; };
	void setDuplicate(int player) { duplicate = player; }

protected:
	virtual Card thinkCardInSuit(Hand *hand, Suit suit, int turn);
	virtual Card thinkCardOutSuit(Hand *hand, Suit suit, int turn);
	virtual Card thinkCardFirst(Hand *hand, int turn);

	virtual Table* getTable() { return table; }

private:
	Card highestCardExcept(Hand *hand, Suit suit);
	int  nearestCard(Hand *hand, Suit suit, int highestOnTable);
	bool lowestCardOfAllExcept(Hand *hand, int exceptSuits, OUT Card *card);

private:
	int duplicate;
	MainTable *table;
};

#endif //_KING_LEVEL_H_
