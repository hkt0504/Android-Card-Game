#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "common.h"

class Hand;
class Table;
class Player;

class Level
{
public:
	Level();
	virtual ~Level();

public:
	void setTable(Table *table) { this->table = table; };
	void setPlayers(Player *players[]);

	Card thinkCard(Hand *hand, int turn, Suit tarneeb);

private:
	Card thinkFirstCard(Hand *hand, int turn, Suit tarneeb);
	Card thinkCardInSuit(Hand *hand, int turn, Suit tarneeb);
	Card thinkCardOutSuit(Hand *hand, int turn, Suit tarneeb);

	Card thinkHighestOrLowestCard(Hand *hand, Suit tarneeb);
	bool isOtherTeamHasSuit(Suit suit, int turn);

public:
	Suit suitLed;

private:
	Table *table;
	Player *players[PLAYER_NUM];
};

#endif //_LEVEL_H_
