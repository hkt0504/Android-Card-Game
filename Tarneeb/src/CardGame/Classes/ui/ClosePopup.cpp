#include "ClosePopup.h"
#include "common.h"
#include "GameScene.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool ClosePopup::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !LayerColor::initWithColor(ccc4(0, 0, 0, 0)))
	{
		return false;
	}

	BlendFunc blend;
	blend.src = GL_SRC_ALPHA;
	blend.dst = GL_ONE_MINUS_SRC_ALPHA;

	setBlendFunc(blend);
	setContentSize(visibleSize);

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	center = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);

	auto backLayer = LayerColor::create(ccc4(0,0,0,50), visibleSize.width, visibleSize.height);
	backLayer->setAnchorPoint(Vec2::ZERO);
	backLayer->setPosition(Vec2::ZERO);
	this->addChild(backLayer, 1);

	auto spr = Sprite::create("popup/close_confirm_bg.png");
	spr->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	this->addChild(spr, 2, 0);

	Size bgSize = spr->getContentSize();
	auto yesItem = MenuItemImage::create("popup/confirm_btn_yes.png", "popup/confirm_btn_yes.png",
			CC_CALLBACK_1(ClosePopup::menuYesCallback, this));

	float x = origin.x + (visibleSize.width - bgSize.width) / 2.0;
	float y = origin.y + (visibleSize.height - bgSize.height) / 2.0;
	yesItem->setPosition(x + bgSize.width / 4.0 + 50, y + bgSize.height / 2.0 - 50);
	auto menuYes = Menu::create(yesItem, NULL);
	menuYes->setPosition(Vec2::ZERO);
	this->addChild(menuYes, 7);

	auto noItem = MenuItemImage::create("popup/confirm_btn_no.png", "popup/confirm_btn_no.png",
			CC_CALLBACK_1(ClosePopup::menuNoCallback, this));
	noItem->setPosition(x + bgSize.width * 3.0 / 4.0 - 50, y + bgSize.height / 2.0 - 50);
	auto menuNo = Menu::create(noItem, NULL);
	menuNo->setPosition(Vec2::ZERO);
	this->addChild(menuNo, 8);

	CCLog(" Call ClosePopup::init()");
	return true;
}

void ClosePopup::closeSelf()
{
	Menu *menuBack = (Menu*)gameScene->getChildByTag(BACK_ITEM_TAG);
	menuBack->setEnabled(true);
	setPosition(-visibleSize.width, -visibleSize.height);
}

void ClosePopup::menuYesCallback(Ref* pSender)
{
	closeSelf();
	gameScene->back();
}

void ClosePopup::menuNoCallback(Ref* pSender)
{
	gameScene->setEnabled(true);
	closeSelf();
}
