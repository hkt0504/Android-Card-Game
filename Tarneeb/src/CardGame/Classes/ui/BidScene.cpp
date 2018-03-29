#include "BidScene.h"
#include "common.h"
#include "GameScene.h"
#include "Controller.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool BidScene::init()
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
	spr->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height / 2);
	this->addChild(spr, 2, 0);

	contentSize = spr->getContentSize();

	CCLog(" Call BidScene::init()");
	return true;
}

void BidScene::setMin(int value)
{
	float contentWidth = contentSize.width;
	float contentHeight = contentSize.height;
	float marginX = contentWidth / 5.0;
	float marginY = contentHeight / 4.0;
	float deltaX = origin.x + (visibleSize.width - contentWidth) / 2 + marginX;
	float deltaY = origin.y + (visibleSize.height + contentHeight) / 2 - marginY * 2;
	char text[30];
	float x, y;

	int index = 0;
	if (value < 7) value = 7;
	for (int i = value; i < 15; i++, index++) {

		x = origin.x + deltaX + (index % 4)* marginX;
		y = origin.y + deltaY - (index / 4) * marginY;

		LabelTTF *label = NULL;

		if (i == 14) {
			strcpy(text, "bid/btn_bid_pass.png");
		} else {
			sprintf(text, "bid/btn_bid%d.png", i - 6);
		}

		MenuItemImage *menuItem = MenuItemImage::create(text, text, CC_CALLBACK_1(BidScene::menuCallback, this, i));
		menuItem->setPosition(x, y);
		Menu *menu = Menu::create(menuItem, NULL);
		menu->setPosition(Vec2::ZERO);

		this->addChild(menu, 100 + i, i);
	}
}

void BidScene::menuCallback(Ref* pSender, int val)
{
	CCLog("Bid val (%d)", val);

	if (val < 14) {
		this->removeFromParent();
		gameScene->setEnabled(true);
		gameScene->controller->setBid(val);
	} else { //pass
		this->removeFromParent();
		gameScene->setEnabled(true);
		gameScene->controller->setBid(0);
	}
}

