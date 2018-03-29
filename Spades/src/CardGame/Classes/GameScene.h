#ifndef __GAMESCENE_SCENE_H__
#define __GAMESCENE_SCENE_H__

#include "cocos2d.h"
#include "common.h"

class CardSprite;
class PlayerObject;
class Game;
class Hand;
using namespace CocosDenshion;

class GameScene : public cocos2d::Layer
{
public:
	GameScene();
	virtual ~GameScene();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(int index);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void onTouched(cocos2d::Point point);

    void removePlayedCards(int winIndex);
    void loadAllCards();
    // a selector callback
    void menuBackCallback(cocos2d::Ref* pSender);
    void menuResultCallback(Ref* pSender);

    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    void back();

    void prepareGame();

    cocos2d::Vector<CardSprite*> cardList;
    cocos2d::Size visibleSize;
    cocos2d::Point origin;
    cocos2d::Point center;

    PlayerObject *player[PLAYER_NUM];
    CardSprite *playedCards[4];

    Game *game;
    int playedIndex;
    int gameIndex;

    //call For GameLogic
    void onStep(float time);
    void onSetHand(int index, Hand* hand);
    void onWantBid();
    void onShowBid(int index, int bid);
    void onFinishBid();
    void onTouchEnable(bool enable);
    void onTouchEnable(int suitType, bool enable);
    void onPlayed(int playerIndex, CardSprite* card);
    void onPlayCard(int playerIndex, int cardIndex);
    void onSpadeBroken();
    void onSetScore(int playerIndex, int bid, int score);
    void onRoundFinished();
    void onGameFinished(bool win);

    void setUserBid(int value, bool dNil);
    void newRound();
    void showUserCards();
    void setEnabled(bool enable);
    bool enabled;

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
};

#endif // __GAMESCENE_SCENE_H__
