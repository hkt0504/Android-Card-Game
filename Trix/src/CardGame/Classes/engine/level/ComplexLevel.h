#ifndef _COMPLEX_LEVEL_H_
#define _COMPLEX_LEVEL_H_

#include "Level.h"
#include "MainTable.h"

class ComplexLevel : public Level
{
public:
	ComplexLevel();
	virtual ~ComplexLevel();

	virtual bool evaluateScore(int turn);

	virtual bool getDuplicateList(int player, OUT int& duplicates);
	virtual void setDuplicate(int player, int duplicates);
	virtual bool thinkDuplicate(int player, OUT int& duplicates);

	virtual void setTable(Table *table) { this->table = (MainTable *) table; };
	void setDuplicateQ(Suit suit, int player) { duplicateQ[suit_index(suit)] = player; }
	void setDuplicateK(int player) { duplicateK = player; }

protected:
	virtual Card thinkCardInSuit(Hand *hand, Suit suit, int turn);
	virtual Card thinkCardOutSuit(Hand *hand, Suit suit, int turn);
	virtual Card thinkCardFirst(Hand *hand, int turn);

	virtual Table* getTable() { return table; }

private:
	int highestCardInSuit(Hand *hand, Suit suit);
	Card highestCardOutSuit(Hand *hand, Suit suit);
	Card lowestCardExcept(Hand *hand, int turn, int exceptSuit);
	int  nearestCardWithTeam(Hand *hand, Suit suit, int highestOnTable);
	int  nearestCard(Hand *hand, Suit suit, int highestOnTable);
	int  highCard(Hand *hand, Suit suit, int highestOnTable);

private:
	int duplicateQ[4];
	int duplicateK;
	int playedCount;

	bool kingPlayed;
	int queenPlayed;

	MainTable *table;

};

#endif //_COMPLEX_LEVEL_H_
