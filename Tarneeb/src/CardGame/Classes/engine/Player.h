#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "common.h"

class Hand;

class Player
{
public:
	Player();
	virtual ~Player();

	void clear();

	void setEmptySuit(Suit suit);
	bool isEmptySuit(Suit suit);
	void thinkBid(int maxBid);

private:
	Suit getHighestSuit(Suit suit1, Suit suit2);

public:
	Hand *hand;
	bool played;
	int score;

	int bid;
	Suit tarneeb;

	int tricks; // number of tricks taken by each player

private:
	bool emptySuit[4];
};

#endif //_PLAYER_H_
