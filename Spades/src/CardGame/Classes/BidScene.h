#ifndef _BID_SCENE_H_
#define _BID_SCENE_H_
#include "cocos2d.h"

USING_NS_CC;
class GameScene;

#define BACK_ITEM_TAG		3
#define BID_LAYER_SCALE		0.8

class BidScene : public cocos2d::LayerColor
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    cocos2d::Size visibleSize;
    cocos2d::Point origin;
    cocos2d::Point center;
    bool doubleNil;
    LabelTTF *nilLabel;
    GameScene *gameScene;

    void menuCallback(cocos2d::Ref* pSender, int val);
    void setEnabledBids(bool enable);
    void changeNilState(bool dNil);
    void setEnabledBtn(int index, bool enable);

    // implement the "static create()" method manually
    CREATE_FUNC(BidScene);
};

#endif /* _BID_SCENE_H_ */
