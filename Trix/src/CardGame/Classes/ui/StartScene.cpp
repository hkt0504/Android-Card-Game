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

	auto spriteBg = Sprite::create("bg1.jpg");
	Size imgSize = spriteBg->getContentSize();

	visibleSize = Director::getInstance()->getWinSizeInPixels();
	origin = Director::getInstance()->getVisibleOrigin();

	spriteBg->setScale(SCR_W / imgSize.width, SCR_H / imgSize.height);
	spriteBg->setPosition(Vec2(origin.x + visibleSize.width/2.0, origin.y + visibleSize.height / 2.0));
	cocos2d::log("visibleSize(%f, %f), imgSize(%f, %f), org(%f, %f)", visibleSize.width, visibleSize.height, imgSize.width, imgSize.height, origin.x, origin.y);
	this->addChild(spriteBg, 0);

	auto sprLogo = Sprite::create("logo1.png");
	sprLogo->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 60));
	this->addChild(sprLogo, 1);

	auto sprLoading = Sprite::create("banners/loading.png");
	sprLoading->setPosition(origin.x + visibleSize.width / 2, origin.y + 250);
	this->addChild(sprLoading, 1);

	auto sprBarBg = Sprite::create("banners/progress_bg.png");
	sprBarBg->setPosition(visibleSize.width / 2, origin.y + 170);
	this->addChild(sprBarBg,2);

	ui::LoadingBar *loadBar = ui::LoadingBar::create("banners/progress_status.png", 0);
	loadBar->setPosition(Vec2(visibleSize.width / 2, origin.y + 170));
	addChild(loadBar, 3, PROGRESS_BAR_TAG);
	loadPercent = 0;

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
