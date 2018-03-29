#include "ResultPopup.h"
#include "common.h"
#include "GameScene.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool ResultPopup::init()
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

	auto spr = Sprite::create("popup/result_popup_bg.png");
	spr->setPosition(center);
	this->addChild(spr, 2, 0);

	Size bgSize = spr->getContentSize();
	float x = origin.x + (visibleSize.width - bgSize.width) / 2.0;
	float y = origin.y + (visibleSize.height - bgSize.height) / 2.0;

	auto backItem = MenuItemImage::create("Buttons/btn_close.png", "Buttons/btn_close_d.png", CC_CALLBACK_1(ResultPopup::menuBackCallback, this));
	backItem->setPosition(Vec2(x + bgSize.width - backItem->getContentSize().width/2 - 20, y + bgSize.height - backItem->getContentSize().height/2 - 20));
	auto menuBack = Menu::create(backItem, NULL);
	menuBack->setPosition(Vec2::ZERO);
	this->addChild(menuBack, 2);

	CCLog(" Call ResultPopup::init()");
	return true;
}

void ResultPopup::viewResult(bool win, int count)
{
	char text[20] = {0, };
	if (win) {
		if (count < 0) {
			strcpy(text, "YOU WIN");
		} else {
			sprintf(text, "YOU WIN %d", count);
		}
	} else {
		if (count < 0) {
			strcpy(text, "YOU LOSE");
		} else {
			sprintf(text, "YOU LOSE %d", count);
		}
	}

	auto title = LabelTTF::create(text, "Felt", 50);
	title->setPosition(center);
	this->addChild(title, 3);
}

void ResultPopup::closeSelf()
{
	Menu *menuBack = (Menu*)gameScene->getChildByTag(BACK_ITEM_TAG);
	menuBack->setEnabled(true);
	setPosition(-visibleSize.width, -visibleSize.height);
}

void ResultPopup::menuBackCallback(Ref* pSender)
{
	closeSelf();
	gameScene->back();
}
