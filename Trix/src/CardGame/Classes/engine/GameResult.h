#ifndef _GAME_RESULT_H_
#define _GAME_RESULT_H_

#include "cocos2d.h"
#include "common.h"

class RoundScore : public cocos2d::Node
{
public:
	virtual bool init();
	char* getTypeName();

	CREATE_FUNC(RoundScore);

	BidType type;
	int score1;
	int score2;
	int score3;
	int score4;
};


class GameResult
{
public:
	GameResult();
	virtual ~GameResult();

	void clear();

	void addKingdom(int player);
	int getKingdomCount();
	int getKingdom(int index);

	void addRoundScore(BidType type, int score1, int score2, int score3, int score4);
	int getRoundCount();
	RoundScore *getRoundScore(int index);

	int getTotalScore(int player);

private:
	int kingdom[PLAYER_NUM];
	int kingdomCount;

	cocos2d::Vector<RoundScore*> scoreList;
	int total[PLAYER_NUM];
};

#endif //_GAME_RESULT_H_
