#ifndef _BUY_SCENE_H_
#define _BUY_SCENE_H_
#include "cocos2d.h"

USING_NS_CC;

class BuyScene : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    cocos2d::Size visibleSize;
    cocos2d::Point origin;
    cocos2d::Point center;
    cocos2d::Size bgSize;

    void menuBackCallback(cocos2d::Ref* pSender);
    void menuAdsCallback(cocos2d::Ref* pSender);
    void menuWinsCallback(cocos2d::Ref* pSender);
    void menuLosesCallback(cocos2d::Ref* pSender);

    void onGooglePurchase(int action);

    CREATE_FUNC(BuyScene);
};

#endif /* _BUY_SCENE_H_ */
