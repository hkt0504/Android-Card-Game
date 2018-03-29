#ifndef _GAME_RESULT_H_
#define _GAME_RESULT_H_

#include "cocos2d.h"
#include "common.h"

class RoundScore : public cocos2d::Node
{
public:
	virtual bool init();

	CREATE_FUNC(RoundScore);

	int score1;
	int score2;
};


class GameResult
{
public:
	GameResult();
	virtual ~GameResult();

	void clear();

	void addRoundScore(int score1, int score2);
	int getRoundCount();
	RoundScore *getRoundScore(int index);

	int getTotalScore(int player);

private:

	cocos2d::Vector<RoundScore*> scoreList;
	int total[2];
};

#endif //_GAME_RESULT_H_
