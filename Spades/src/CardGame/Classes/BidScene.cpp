#include "BidScene.h"
#include "common.h"
#include "GameScene.h"
#include <GLES2/gl2.h>

USING_NS_CC;

// on "init" you need to initialize your instance
bool BidScene::init()
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
	doubleNil = true;

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object

	Sprite *spr = Sprite::create("pop_bid.png");
	spr->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height / 2));
	this->addChild(spr, 2, 0);

	float contentWidth = spr->getContentSize().width;
	float contentHeight = spr->getContentSize().height;
	float marginX = contentWidth / 5.0 - 2;
	float marginY = contentHeight / 4.0 - 9;
	float deltaX = origin.x + (visibleSize.width - contentWidth) / 2 + marginX / 2 + 22;
	float deltaY = origin.y + (visibleSize.height + contentHeight) / 2 - marginY - 49;
	char text[4];
	float x, y;
	for (int i = 0; i < 15; i++) {
		sprintf(text, "%d", i + 1);
		x = origin.x + deltaX + (i % 5)* marginX;
		y = origin.y + deltaY - (i / 5) * marginY;
		x -= (i % 5) * 10;

		LabelTTF *label = NULL;
		if (i == 13) {
			label = LabelTTF::create("Double Nil", "Felt", 24);
			label->setColor(Color3B::WHITE);
			label->setHorizontalAlignment(TextHAlignment::CENTER);
			nilLabel = label;
		} else if (i == 14) {
			label = LabelTTF::create("Show", "Felt", 24);
			label->setColor(Color3B::WHITE);
		} else {
			label = LabelTTF::create(text, "Felt", 36);
		}

		MenuItemLabel *menuItem = MenuItemLabel::create(label, CC_CALLBACK_1(BidScene::menuCallback, this, i + 1));
		float old_width = menuItem->getContentSize().width;
		menuItem->setContentSize(Size(100, menuItem->getContentSize().height));
		label->setPosition(label->getPosition().x + (50 - label->getContentSize().width / 2), 0);
		menuItem->setPosition(Vec2(x, y));
		auto menu = Menu::create(menuItem, NULL);
		menu->setPosition(Vec2::ZERO);
		if(i < 13)
		{
			menu->setColor(ccc3(180, 180, 180));
			menu->setEnabled(false);
		}

		this->addChild(menu, 100 + i, i + 1);
	}

	CCLog(" Call BidScene::init()");
	return true;
}

void BidScene::menuCallback(Ref* pSender, int val)
{
	CCLog("Bid val (%d)", val);

	if( val < 14)
	{
		this->removeFromParent();
		gameScene->setEnabled(true);
		gameScene->setUserBid(val, false);
		gameScene->showUserCards();
	}
	else if (val == 14) /* Double Nil or Nil */
	{
		this->removeFromParent();
		gameScene->setEnabled(true);
		gameScene->setUserBid(0, doubleNil);
		gameScene->showUserCards();
	}
	else if (val == 15) /* Show Cards */
	{
		setEnabledBids(true);
		changeNilState(false);
		setEnabledBtn(15, false);
		gameScene->showUserCards();
	}
}

void BidScene::setEnabledBids(bool enable)
{
	for (int i = 1; i <14; i++)
		setEnabledBtn(i, enable);
}

void BidScene::changeNilState(bool dNil)
{
	doubleNil = dNil;
	if (dNil) {
		nilLabel->setString("Double Nil");
	} else {
		nilLabel->setString("Nil");
	}
	nilLabel->setPosition(nilLabel->getPosition().x + (50 - nilLabel->getContentSize().width / 2), 0);
}

void BidScene::setEnabledBtn(int index, bool enable)
{
	Menu *menu = (Menu*)getChildByTag(index);
	menu->setEnabled(enable);
	if(enable)
		menu->setColor(Color3B::WHITE);
	else
		menu->setColor(ccc3(180, 180, 180));
}
