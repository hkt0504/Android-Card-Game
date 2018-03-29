#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "common.h"
#include "cocos2d.h"

class Hand;
class Table;
class Player;

class Level
{
public:
	Level();
	virtual ~Level();

	virtual Card thinkCard(Hand* hand, int turn);

	/** return is finished */
	virtual bool evaluateScore(int turn) { return false; };

	/** for duplicate */
	virtual bool getDuplicateList(int player, OUT int& duplicates);
	virtual void setDuplicate(int player, int duplicates) {};
	virtual bool thinkDuplicate(int player, OUT int& duplicates);

	virtual void setTable(Table *table) {};
	void setPlayers(Player *players[]);
	void setPartner(bool partner) { this->partner = partner; };

protected:
	virtual Card thinkCardInSuit(Hand *hand, Suit suit, int turn);
	virtual Card thinkCardOutSuit(Hand *hand, Suit suit, int turn);
	virtual Card thinkCardFirst(Hand *hand, int turn);

	virtual Table* getTable() { return NULL; }

	bool lowestCardOfAll(Hand *hand, OUT Card *card);

	bool isTeamHighest();

public:
	Player *players[PLAYER_NUM];
	Suit suitLed;

protected:
	bool partner;
};

#endif //_LEVEL_H_
