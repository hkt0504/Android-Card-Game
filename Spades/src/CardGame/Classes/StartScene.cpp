#include "StartScene.h"
#include "common.h"
#include "MenuScene.h"
#include "ui/UILoadingBar.h"

USING_NS_CC;

#define PROGRESS_BAR_TAG	10

Scene* StartScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = StartScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StartScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    cocos2d::Sprite *sprite2 = Sprite::create("bg2.jpg");
    Size imgSize = sprite2->getContentSize();

    //Director::getInstance()->setContentScaleFactor(g_screenScale);
    Director::getInstance()->getOpenGLView()->setDesignResolutionSize(imgSize.width, imgSize.height, ResolutionPolicy::EXACT_FIT);
    visibleSize = Director::getInstance()->getWinSizeInPixels();
    origin = Director::getInstance()->getVisibleOrigin();

    sprite2->setPosition(Vec2(origin.x + visibleSize.width/2.0, origin.y + visibleSize.height / 2.0));
    cocos2d::log("visibleSize(%f, %f), imgSize(%f, %f), org(%f, %f)", visibleSize.width, visibleSize.height, imgSize.width, imgSize.height, origin.x, origin.y);

    this->addChild(sprite2, 0);

    Sprite *fuelBarBorder = Sprite::create("banners/progress_bg.png");
    fuelBarBorder->setPosition(ccp(visibleSize.width / 2,origin.y + 170));
    this->addChild(fuelBarBorder,1);

    ui::LoadingBar *loadBar = ui::LoadingBar::create("banners/progress_status.png", 0);
    loadBar->setPosition(ccp(visibleSize.width / 2,origin.y + 170));
    addChild(loadBar, 2, PROGRESS_BAR_TAG);
    loadPercent = 0;
    /*
    ProgressTimer *fuelBar = CCProgressTimer::create(CCSprite::createWithSpriteFrameName ("banners/progress_status.png" ));
    // Set this progress bar object as kCCProgressTimerTypeBar (%)
    fuelBar->setType(CCProgressTimerType::BAR);
    // Set anchor point in 0,0 and add it as a child to our border sprite
    fuelBar->setAnchorPoint(ccp(0,0));
    fuelBar->setBarChangeRate(ccp(1,0)); // To make width 100% always
    fuelBar->setTag(1);                  // Tag our object for easy access
    fuelBarBorder->addChild(fuelBar,50); // Add it inside the border sprite

    fuelBar->setPercentage(80);
    */

	//Sprite *spr2 = Sprite::create("effects/shining.png");
	//spr2->setPosition(Point(origin.x + visibleSize.width / 2.0, origin.y + visibleSize.height - spr2->getContentSize().height / 2.0));
	//this->addChild(spr2, 1);
//	RotateTo *action_1 = RotateTo::create(3.0f, 0, 60);
//	RepeatForever *action_2 = RepeatForever::create(action_1);
//	spr->runAction(action_2);

	//Sprite *spr = Sprite::create("hearts_logo.png");
	//spr->setPosition(Point(origin.x + visibleSize.width / 2.0, origin.y + visibleSize.height - spr2->getContentSize().height / 2.0));
	//this->addChild(spr, 2);
	schedule(schedule_selector(StartScene::onStart), 0.1);

    return true;
}

void StartScene::onStart(float time)
{
	if(loadPercent == 100)
	{
		unscheduleAllSelectors();
		Scene *pScene = TransitionFade::create(0.5, MenuScene::createScene());
		Director:: getInstance()->replaceScene(pScene);
		return;
	}
	loadPercent += 10;
	ui::LoadingBar *loadBar = (ui::LoadingBar*)getChildByTag(PROGRESS_BAR_TAG);
	if(loadBar)
		loadBar->setPercent(loadPercent);
}
