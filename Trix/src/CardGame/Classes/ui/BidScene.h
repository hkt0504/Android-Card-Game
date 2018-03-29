#ifndef _BID_SCENE_H_
#define _BID_SCENE_H_
#include "cocos2d.h"

USING_NS_CC;
class GameScene;


class BidScene : public cocos2d::LayerColor
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    cocos2d::Size visibleSize;
    cocos2d::Point origin;
    cocos2d::Point center;
    GameScene *gameScene;

    void menuCallback(cocos2d::Ref* pSender, int val);
    void setEnabledBids(int bid);

    void initKingdom();
    void initComplex();

    MenuItemImage *kingItem;
    MenuItemImage *queenItem;
    MenuItemImage *diamondItem;
    MenuItemImage *lotoshItem;
    MenuItemImage *trixItem;
    MenuItemImage *complexItem;

    // implement the "static create()" method manually
    CREATE_FUNC(BidScene);
};

#endif /* _BID_SCENE_H_ */
