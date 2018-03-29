#ifndef __MENUSCENE_SCENE_H__
#define __MENUSCENE_SCENE_H__

#include "cocos2d.h"

class MenuScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    void particleAnimation();
    void createParticleAction();
    void endParticleAction();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void menuPlayCallback(cocos2d::Ref* pSender);
    void menuHelpCallback(cocos2d::Ref* pSender);
    void menuFaceBookCallback(cocos2d::Ref* pSender);
    void menuMoreCallback(cocos2d::Ref* pSender);
    void menuRateCallback(cocos2d::Ref* pSender);
    void menuSoundCallback(cocos2d::Ref* pSender);

    void onSound(bool mute);
    void onFaceBook(int action);

    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    int particleIndex;

    cocos2d::MenuItemToggle *soundItem;

    // implement the "static create()" method manually
    CREATE_FUNC(MenuScene);
};

#endif // __MENUSCENE_SCENE_H__
