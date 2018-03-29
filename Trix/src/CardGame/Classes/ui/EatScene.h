#ifndef _EAT_SCENE_H_
#define _EAT_SCENE_H_

#include "cocos2d.h"
#include "common.h"

USING_NS_CC;

class GameScene;

class EatScene : public cocos2d::LayerColor
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    cocos2d::Size visibleSize;
    cocos2d::Point origin;
    cocos2d::Point center;
    GameScene *gameScene;

    void showCards();
    void menuBackCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(EatScene);

private:
    void drawAvatar(const char *name, char *photo, int score, float x, float y);
    void setOnePlayer(EatCard eat, float x, float y, float deltaX, float deltaY);
};

#endif /* _EAT_SCENE_H_ */
