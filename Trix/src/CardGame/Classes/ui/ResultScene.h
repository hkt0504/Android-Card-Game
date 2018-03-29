#ifndef _RESULT_SCENE_H_
#define _RESULT_SCENE_H_
#include "cocos2d.h"

USING_NS_CC;
class GameScene;
class RoundScore;

class ResultScene : public cocos2d::LayerColor
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static Scene* createScene(GameScene *parent);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    cocos2d::Size visibleSize;
    cocos2d::Point origin;
    cocos2d::Point center;
    cocos2d::Size bgSize;
    cocos2d::Vec2 bgOrigin;

    enum {ANYWHERE = 0, ROUND};
    int viewMode;

    GameScene *gameScene;

    void viewResult(int mode);
    void menuBackCallback(cocos2d::Ref* pSender);

    CREATE_FUNC(ResultScene);

private:
    void addKingdomField(float ypos, int player);
    void addScoreField(float ypos, RoundScore *score);
    void setTotalField(int total1, int total2, int total3, int total4);
};

#endif /* _RESULT_SCENE_H_ */
