#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "cocos2d.h"
#include "common.h"

enum GameState {
	eGS_IDLE = 0,
	eGS_NEW_ROUND,
	eGS_SHUFFLE,
	eGS_BID,
	eGS_DUPLICATE,
	eGS_PREPARE_PLAY,
	eGS_PLAYCARD,
	eGS_NEXT_TRICK,
	eGS_END_ROUND,
};

class Hand;
class GameScene;
class CardDeck;
class Player;
class Table;
class Level;
class GameResult;

class Controller : public cocos2d::Node
{
public:
	virtual ~Controller();
	virtual bool init();

	void newRound();
	void setIdle() { state = eGS_IDLE; };
	void step();
	void setUserBid(BidType value);
	void setUserDuplicate(int duplicates);
	void setCardPlayed(int playerIndex, int cardIndex);

	BidType getBid() { return bid; };
	void setPlayMode(bool partner, bool complex);
	void setGameScene(GameScene * scene) { gameScene = scene; };
	GameResult* getGameResult();
	void getEatCard(EatCard eatCard[]);

	CREATE_FUNC(Controller);

private:
	void initRound();
	void shuffle();
	void processBid();
	void processDuplicate();
	void thinkDuplicate();
	void preparePlay();
	void playCard();
	void prepareUserCard();
	void playUserCard(int cardIndex);
	void nextTurn();
	void nextTrick();
	void endRound();

	void createLevel();

private:
	GameState state;
	int idleCount;

	GameScene* gameScene;

	CardDeck *deck;
	Level *level;
	Hand *tempHand;

	Table *table;
	Player *players[PLAYER_NUM];

	GameResult *gameResult;

	BidType bid;
	bool partner;
	bool complex;
	bool showDue;

	int turn;
	int startTurn;
	int bidCount;

};

#endif //_CONTROLLER_H_
