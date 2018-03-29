#ifndef __LEADERBOARD_SCENE_H__
#define __LEADERBOARD_SCENE_H__

#include "cocos2d.h"
#include "common.h"

class LeaderboardScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // a selector callback
    void menuBackCallback(cocos2d::Ref* pSender);
    void menuMoreGame(Ref* pSender);
    void menuRateGame(Ref* pSender);

    void onFacebook(GameScore scores[], int count);

    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    cocos2d::Size boardSize;
    cocos2d::Vec2 boardOrigin;

    // implement the "static create()" method manually
    CREATE_FUNC(LeaderboardScene);

private:
    void loadScores();
    void addScoreField(float ypos, GameScore score);
};

#endif // __LEADERBOARD_SCENE_H__
