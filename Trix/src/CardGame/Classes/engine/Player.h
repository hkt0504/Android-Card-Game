#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "common.h"

class Hand;

class Player
{
public:
	Player();
	virtual ~Player();

	void setMode(bool complex);

	void clear();
	BidType thinkBid();
	bool noHaveBid();

	void setEmptySuit(Suit suit);
	bool isEmptySuit(Suit suit);

public:
	Hand *hand;
	bool played;
	int score;

	int bid;
	int duplicates;

	/** for eating card */
	int eatKQ;
	int eatDiamond;
	int eatLotosh;

private:
	bool complex;

	bool emptySuit[4];
};

#endif //_PLAYER_H_
