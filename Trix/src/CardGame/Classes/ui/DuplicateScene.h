#ifndef _DUPLICATE_SCENE_H_
#define _DUPLICATE_SCENE_H_

#include "cocos2d.h"

USING_NS_CC;
class GameScene;

#define BACK_ITEM_TAG		3
#define BID_LAYER_SCALE		0.8

class DuplicateScene : public cocos2d::LayerColor
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    cocos2d::Size visibleSize;
    cocos2d::Point origin;
    cocos2d::Point center;
    GameScene *gameScene;
    int duplicates;

    void setDuplicate(int duplicates);
    void menuBackCallback(cocos2d::Ref* pSender);
    void menuDupCallback(cocos2d::Ref* pSender, int val);

    // implement the "static create()" method manually
    CREATE_FUNC(DuplicateScene);
};

#endif /* _DUPLICATE_SCENE_H_ */
