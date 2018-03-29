#ifndef __MODE_SCENE_H__
#define __MODE_SCENE_H__

#include "cocos2d.h"
#include "common.h"

class ModeScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene(int index);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    int gameIndex;

    cocos2d::MenuItemToggle *soundItem;

    void menuNotarneebCallback(cocos2d::Ref* pSender);
    void menuTarneebCallback(cocos2d::Ref* pSender);

    void onSound(bool mute);

    // implement the "static create()" method manually
    CREATE_FUNC(ModeScene);
};

#endif // __MODE_SCENE_H__
