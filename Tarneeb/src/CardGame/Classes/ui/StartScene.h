#ifndef __STARTSCENE_SCENE_H__
#define __STARTSCENE_SCENE_H__

#include "cocos2d.h"

class StartScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    int heartIndex;
    int loadPercent;

    void onStart(float time);

    // implement the "static create()" method manually
    CREATE_FUNC(StartScene);
};

#endif // __STARTSCENE_SCENE_H__
