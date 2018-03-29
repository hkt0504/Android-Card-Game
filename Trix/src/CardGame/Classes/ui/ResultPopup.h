#ifndef _RESULT_POPUP_H_
#define _RESULT_POPUP_H_
#include "cocos2d.h"

USING_NS_CC;
class GameScene;

#define BACK_ITEM_TAG		3
#define RESULT_ITEM_TAG		4

class ResultPopup : public cocos2d::LayerColor
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    cocos2d::Size visibleSize;
    cocos2d::Point origin;
    cocos2d::Point center;
    GameScene *gameScene;

    void viewResult(bool win, int count);
    void menuBackCallback(cocos2d::Ref* pSender);
    void closeSelf();

    // implement the "static create()" method manually
    CREATE_FUNC(ResultPopup);
};

#endif /* _RESULT_POPUP_H_ */
