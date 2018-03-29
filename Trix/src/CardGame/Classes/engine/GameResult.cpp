#include "GameResult.h"


bool RoundScore::init()
{
	if (!Node::init()) {
		return false;
	}

	return true;
}

char* RoundScore::getTypeName()
{
	switch (type) {
	case BT_KING:
		return "K";
	case BT_QUEEN:
		return "Q";
	case BT_DIAMOND:
		return "D";
	case BT_LOTOSH:
		return "L";
	case BT_TRIX:
		return "T";
	case BT_COMPLEX:
		return "C";
	default:
		return "";
	}
}

//////////////////////////////////
// GameResult

GameResult::GameResult()
{
	for (int i = 0; i < PLAYER_NUM; i++) {
		total[i] = 0;
		kingdom[i] = 0;
	}

	kingdomCount = 0;
}

void GameResult::clear()
{
	for (int i = 0; i < PLAYER_NUM; i++) {
		total[i] = 0;
		kingdom[i] = 0;
	}

	kingdomCount = 0;

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

void GameResult::addKingdom(int player)
{
	kingdom[kingdomCount] = player;
	kingdomCount ++;
}

int GameResult::getKingdomCount()
{
	return kingdomCount;
}

int GameResult::getKingdom(int index)
{
	return kingdom[index];
}

void GameResult::addRoundScore(BidType type, int score1, int score2, int score3, int score4)
{
	RoundScore *score = RoundScore::create();
	score->type = type;
	score->score1 = score1;
	score->score2 = score2;
	score->score3 = score3;
	score->score4 = score4;
	score->retain();

	scoreList.insert(scoreList.size(), score);

	total[0] += score1;
	total[1] += score2;
	total[2] += score3;
	total[3] += score4;
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
