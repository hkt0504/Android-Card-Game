#ifndef __USERSCENE_SCENE_H__
#define __USERSCENE_SCENE_H__

#include "cocos2d.h"
#include "common.h"

class UserScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // a selector callback
    void menuBackCallback(cocos2d::Ref* pSender);
    void menuPlayNow(Ref* pSender);
    void menuShareGame(Ref* pSender);
    void menuInviteFriend(Ref* pSender);
    void menuBuyFeatures(Ref* pSender);
    void menuAchievement(Ref* pSender);
    void menuLeaderboard(Ref* pSender);

    void setUserAvatar(const char *imgURL);
    void setUserName(const  char *name);
    void setUserLevel(int level);
    void setUserWinCount(int count);
    void setUserLoseCount(int count);

    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

    // implement the "static create()" method manually
    CREATE_FUNC(UserScene);
};

#endif // __USERSCENE_SCENE_H__
