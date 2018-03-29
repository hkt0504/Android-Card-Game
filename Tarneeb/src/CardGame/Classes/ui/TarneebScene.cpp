#include "TarneebScene.h"
#include "common.h"
#include "GameScene.h"
#include "Controller.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool TarneebScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!LayerColor::initWithColor(ccc4(0, 0, 0, 0))) {
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

	Sprite *spr = Sprite::create("bid/pop_bid.png");
	spr->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height / 2));
	this->addChild(spr, 2, 0);

	float contentWidth = spr->getContentSize().width;
	float contentHeight = spr->getContentSize().height;
	float marginX = contentWidth / 5.0;
	float x = origin.x + (visibleSize.width - contentWidth) / 2 + marginX;
	float y = center.y - 20;

	MenuItemImage *menuItem0 = MenuItemImage::create("bid/btn_bid_spade.png", "bid/btn_bid_spade.png", CC_CALLBACK_1(TarneebScene::menuCallback, this, 0));
	menuItem0->setPosition(x, y);
	auto menu0 = Menu::create(menuItem0, NULL);
	menu0->setPosition(Vec2::ZERO);
	this->addChild(menu0, 100, 0);

	x += marginX;
	MenuItemImage *menuItem1 = MenuItemImage::create("bid/btn_bid_clover.png", "bid/btn_bid_clover.png", CC_CALLBACK_1(TarneebScene::menuCallback, this, 1));
	menuItem1->setPosition(x, y);
	auto menu1 = Menu::create(menuItem1, NULL);
	menu1->setPosition(Vec2::ZERO);
	this->addChild(menu1, 100, 1);

	x += marginX;
	MenuItemImage *menuItem2 = MenuItemImage::create("bid/btn_bid_heart.png", "bid/btn_bid_heart.png", CC_CALLBACK_1(TarneebScene::menuCallback, this, 2));
	menuItem2->setPosition(x, y);
	auto menu2 = Menu::create(menuItem2, NULL);
	menu2->setPosition(Vec2::ZERO);
	this->addChild(menu2, 100, 2);

	x += marginX;
	MenuItemImage *menuItem3 = MenuItemImage::create("bid/btn_bid_diamond.png", "bid/btn_bid_diamond.png", CC_CALLBACK_1(TarneebScene::menuCallback, this, 3));
	menuItem3->setPosition(x, y);
	auto menu3 = Menu::create(menuItem3, NULL);
	menu3->setPosition(Vec2::ZERO);
	this->addChild(menu3, 100, 3);

	CCLog(" Call TarneebScene::init()");
	return true;
}

void TarneebScene::menuCallback(Ref* pSender, int val)
{
	CCLog("Tarneeb val (%d)", val);

	Suit suits[4] = { SPADE, CLOVER, HEART, DIAMOND };
	this->removeFromParent();
	gameScene->setEnabled(true);
	gameScene->controller->setTarneeb(suits[val]);
}

