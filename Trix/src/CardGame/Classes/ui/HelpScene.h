#ifndef __HELPSCENE_SCENE_H__
#define __HELPSCENE_SCENE_H__

#include "cocos2d.h"

class HelpScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // a selector callback
    void menuBackCallback(cocos2d::Ref* pSender);

    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;

    // implement the "static create()" method manually
    CREATE_FUNC(HelpScene);
};

#endif // __HELPSCENE_SCENE_H__
