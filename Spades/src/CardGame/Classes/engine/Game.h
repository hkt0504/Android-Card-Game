#ifndef _GAME_H_
#define _GAME_H_

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

class Game : public cocos2d::Node
{
public:
	virtual ~Game();
	virtual bool init();

	void initGame();
	void newRound();
	void setIdle() { state = eGS_IDLE; };
	void step();
	void setBid(int value, bool dnil = false);
	void setCardPlayed(int playerIndex, int cardIndex);

	void setGameScene(GameScene * scene) { gameScene = scene; };

	Hand *getHand(int i) {return hands[i];}
	GameResult getGameResult();

private:
	GameState state;
	int idleCount;

	GameScene* gameScene;

	CardDeck *deck;

	Hand *tempHand;
	Hand *hands[PLAYER_NUM];

	Card playedCards[CARD_NUM];

	// card count in current trick
	int trickCardCount;

	// card count while previous trick
	int closedCardCount;

	bool spadeBroken;
	Suit suitLed;

	bool doubleNil;
	int bid[PLAYER_NUM];
	int tricks[PLAYER_NUM]; // number of tricks taken by each player

	bool emptySuit[PLAYER_NUM][4];
	bool played[PLAYER_NUM]; // tracks who has already played in a trick

	int startTurn;
	int turn;
	int bidCount;

	// team score
	int scores[2];
	int bags[2];

	void initRound();
	void shuffle();
	void processBid();
	void preparePlay();
	void playCard();
	int getEnabledCardSuit();
	void playUserCard(int cardIndex);
	void nextTrick();
	void endRound();

	void addCardPlayed(Suit suit, int value);
	int evaluateTrickWin();
	int score(int pair);

	Card thinkCard(Hand *hand);
	Card thinkCardInSuit(Hand *hand, Suit suit);
	Card thinkCardOutSuit(Hand *hand, Suit suit);
	Card thinkCardFirst(Hand *hand);
	Card thinkLowestCardOutSuit(Hand *hand, Suit suit);

	int highestCardIndexInTrick(Suit suit);
	bool isHighestCard(Suit suit, int value);

public:
	CREATE_FUNC(Game);
};

#endif //_GAME_H_
