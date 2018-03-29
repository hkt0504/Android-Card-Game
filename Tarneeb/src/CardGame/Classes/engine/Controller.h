#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "cocos2d.h"
#include "common.h"

enum GameState {
	eGS_IDLE = 0,
	eGS_NEW_ROUND,
	eGS_SHUFFLE,
	eGS_BID,
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
	void setBid(int value);
	void setTarneeb(Suit suit);
	void setCardPlayed(int playerIndex, int cardIndex);

	void setGameScene(GameScene * scene) { gameScene = scene; };

	Hand *getHand(int i);
	GameResult* getGameResult();

	CREATE_FUNC(Controller);

private:
	void initRound();
	void shuffle();
	void processBid();
	void afterBid();
	void preparePlay();
	void playCard();
	int getEnabledCardSuit();
	void playUserCard(int cardIndex);
	void nextTrick();
	void endRound();

	void score();

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

	int bidValue;
	Suit tarneeb;
	int bidder;
	int bidCount;
	int passCount;

	int turn;

};

#endif //_GAME_H_
