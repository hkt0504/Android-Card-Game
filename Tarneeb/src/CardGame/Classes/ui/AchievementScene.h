#ifndef __ACHIEVEMENT_SCENE_H__
#define __ACHIEVEMENT_SCENE_H__

#include "cocos2d.h"

class AchievementScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(int win);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    void setWin(int win);

    // a selector callback
    void menuBackCallback(cocos2d::Ref* pSender);

    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

    cocos2d::Size boardSize;
    cocos2d::Vec2 boardOrigin;

    // implement the "static create()" method manually
    CREATE_FUNC(AchievementScene);
};

#endif // __ACHIEVEMENT_SCENE_H__
