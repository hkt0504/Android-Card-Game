#include "GameResult.h"


bool RoundScore::init()
{
	if (!Node::init()) {
		return false;
	}

	score1 = 0;
	score2 = 0;

	return true;
}

//////////////////////////////////
// GameResult

GameResult::GameResult()
{
	for (int i = 0; i < 2; i++) {
		total[i] = 0;
	}
}

void GameResult::clear()
{
	for (int i = 0; i < 2; i++) {
		total[i] = 0;
	}

	int size = scoreList.size();
	for (int i = size - 1; i >= 0; i--) {
		scoreList.at(i)->release();
	}
	scoreList.clear();
}

GameResult::~GameResult()
{
	int size = scoreList.size();
	for (int i = size - 1; i >= 0; i--) {
		scoreList.at(i)->release();
	}
}

void GameResult::addRoundScore(int score1, int score2)
{
	RoundScore *score = RoundScore::create();
	score->score1 = score1;
	score->score2 = score2;
	score->retain();

	scoreList.insert(scoreList.size(), score);

	total[0] += score1;
	total[1] += score2;
}

int GameResult::getRoundCount()
{
	return scoreList.size();
}

RoundScore* GameResult::getRoundScore(int index)
{
	return scoreList.at(index);
}

int GameResult::getTotalScore(int player)
{
	return total[player];
}
