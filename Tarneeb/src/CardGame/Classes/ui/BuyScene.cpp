#include "BuyScene.h"
#include "common.h"
#include "UserScene.h"
#include "PlayerObject.h"
#include "Google.h"
#include "Facebook.h"

USING_NS_CC;

#define LABEL_TITLE_SIZE	25

Scene* BuyScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = BuyScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool BuyScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}

	this->setKeypadEnabled(true);

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	center = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);

	CCLog(" Call BuyScene::init()");

	Sprite *spr = Sprite::create("bg1.jpg");
	spr->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height / 2);
	this->addChild(spr, 1);

	Sprite *titleBg = Sprite::create("other/buy_title.png");
	titleBg->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height - titleBg->getContentSize().height / 2 - 50);
	this->addChild(titleBg, 2);

	auto backItem = MenuItemImage::create("Buttons/btn_back.png", "Buttons/btn_back.png", CC_CALLBACK_1(BuyScene::menuBackCallback, this));
	backItem->setPosition(origin.x + backItem->getContentSize().width / 2 + 50, origin.y + visibleSize.height - backItem->getContentSize().height / 2 - 50);
	auto menuBack = Menu::create(backItem, NULL);
	menuBack->setPosition(Vec2::ZERO);
	this->addChild(menuBack, 2);

	auto adsItem = MenuItemImage::create("Buttons/btn_remove_ads.png", "Buttons/btn_remove_ads.png", CC_CALLBACK_1(BuyScene::menuAdsCallback, this));
	adsItem->setPosition(origin.x + visibleSize.width / 4.0 + 30, origin.y + visibleSize.height / 3.0);
	auto menuAds = Menu::create(adsItem, NULL);
	menuAds->setPosition(Vec2::ZERO);
	this->addChild(menuAds, 7);

	auto winsItem = MenuItemImage::create("Buttons/btn_buy_wins.png", "Buttons/btn_buy_wins.png", CC_CALLBACK_1(BuyScene::menuWinsCallback, this));
	winsItem->setPosition(origin.x + visibleSize.width / 2.0, origin.y + visibleSize.height / 3.0);
	auto menuWins = Menu::create(winsItem, NULL);
	menuWins->setPosition(Vec2::ZERO);
	this->addChild(menuWins, 8);

	auto losesItem = MenuItemImage::create("Buttons/btn_remove_loses.png", "Buttons/btn_remove_loses.png", CC_CALLBACK_1(BuyScene::menuLosesCallback, this));
	losesItem->setPosition(origin.x + visibleSize.width * 3.0 / 4.0 - 30, origin.y + visibleSize.height / 3.0);
	auto menuLoses = Menu::create(losesItem, NULL);
	menuLoses->setPosition(Vec2::ZERO);
	this->addChild(menuLoses, 9);

	return true;
}

void BuyScene::menuBackCallback(Ref* pSender)
{
	Scene *pScene = TransitionFade::create(0.5, UserScene::createScene());
	Director:: getInstance()->replaceScene(pScene);
}

void BuyScene::menuAdsCallback(Ref* pSender)
{
	Google::buyScene = this;
	Google::purchaseAds();
}

void BuyScene::menuWinsCallback(Ref* pSender)
{
	Google::buyScene = this;
	Google::purchaseWin();
}

void BuyScene::menuLosesCallback(Ref* pSender)
{
	Google::buyScene = this;
	Google::purchaseLose();
}

void BuyScene::onGooglePurchase(int action)
{
	CCLog("UserScene::onGooglePurchase");
}

void BuyScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		menuBackCallback(NULL);
	}
}
