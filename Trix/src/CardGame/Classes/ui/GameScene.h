#ifndef __GAMESCENE_SCENE_H__
#define __GAMESCENE_SCENE_H__

#include "cocos2d.h"
#include "common.h"

class CardSprite;
class PlayerObject;
class Controller;
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
    void menuResultCallback(cocos2d::Ref* pSender);
    void menuEatCallback(cocos2d::Ref* pSender);

    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    void back();
    bool isTrixBid();

    void prepareGame();

    cocos2d::Vector<CardSprite*> cardList;
    cocos2d::Size visibleSize;
    cocos2d::Point origin;
    cocos2d::Point center;

    PlayerObject *player[PLAYER_NUM];
    CardSprite *playedCards[4];

    Controller *controller;
    int playedIndex;
    int gameIndex;
    bool enabled;

    //call For GameLogic
    void onStep(float time);
    void onSetHand(int index, Hand *hand);
    void onWantBid(int enabledBid);
    void onShowBid(int playerIndex, BidType bid);
    void onWantDuplicate(int duplicates);
    void onShowDuplicate(int playerIndex, int duplicates);
    void onPlayTurn(int playerIndex, bool pass = false);
    void onShowScore(int playerIndex, int score, int move);
    void onTouchEnable(bool enable);
    void onTouchEnable(int suitType, bool enable);
    void onTouchEnable(Card enableds[], int count);
    void onPlayed(int playerIndex, CardSprite* card);
    void onPlayCard(int playerIndex, int cardIndex);
    void onRoundFinished();
    void onGameFinished(bool win);

    void newRound();
    void setEnabled(bool enable);

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
};

#endif // __GAMESCENE_SCENE_H__
