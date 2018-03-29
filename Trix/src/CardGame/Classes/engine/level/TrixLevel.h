#ifndef _TRIX_LEVEL_H_
#define _TRIX_LEVEL_H_

#include "Level.h"
#include "TrixTable.h"


class TrixLevel : public Level
{
public:
	TrixLevel();
	virtual ~TrixLevel();

	virtual void setTable(Table *table) { this->table = (TrixTable *) table; };
	virtual bool evaluateScore(int turn);

	virtual bool thinkDuplicate(int player, OUT int& duplicates);
	virtual void setDuplicate(int player, int duplicates);

	virtual Card thinkCard(Hand* hand, int turn);

	virtual Table* getTable() { return table; }

	void getEnabledCards(Hand* hand, Card cards[], int &count);

private:
	TrixTable *table;
	int ranking;
	int card2[4];
};

#endif //_TRIX_LEVEL_H_
