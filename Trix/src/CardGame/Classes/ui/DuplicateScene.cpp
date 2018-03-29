#include "DuplicateScene.h"
#include "common.h"
#include "GameScene.h"
#include "Controller.h"

USING_NS_CC;

#define KING_TAG    11
#define QUEEN_TAG   12
#define DIAMOND_TAG 13
#define LOTOSH_TAG  14
#define TRIX_TAG    15

// on "init" you need to initialize your instance
bool DuplicateScene::init()
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

	Sprite *spr = Sprite::create("bid/dup_bg.png");
	spr->setPosition(center);
	this->addChild(spr, 1, 0);

	Size bgSize = spr->getContentSize();
	float x = origin.x + (visibleSize.width - bgSize.width) / 2.0;
	float y = origin.y + (visibleSize.height - bgSize.height) / 2.0;

	auto backItem = MenuItemImage::create("Buttons/btn_close.png", "Buttons/btn_close_d.png", CC_CALLBACK_1(DuplicateScene::menuBackCallback, this));
	backItem->setPosition(Vec2(x + bgSize.width - backItem->getContentSize().width/2 - 20, y + bgSize.height - backItem->getContentSize().height/2 - 20));
	auto menuBack = Menu::create(backItem, NULL);
	menuBack->setPosition(Vec2::ZERO);
	this->addChild(menuBack, 2);

	duplicates = DUP_NONE;

	CCLog(" Call DuplicateScene::init()");
	return true;
}

void DuplicateScene::setDuplicate(int duplicates)
{
	int flags[5] = {DUP_K, DUP_H_Q, DUP_C_Q, DUP_D_Q, DUP_S_Q};
	bool dups[5] = {false, };
	int count = 0;
	for (int i = 0; i < 5; i++) {
		if (HAS_BIT(duplicates, flags[i])) {
			dups[i] = true;
			count++;
		}
	}

	float gap = (750 - 150 * count) / count;
	float x = - (150 + gap) * (count - 1) / 2;
	float y = -30;

	if (dups[0]) {
		auto item0 = MenuItemImage::create("cards/h13.png", "cards/h13.png", NULL, NULL);
		auto item1 = MenuItemImage::create("bid/h13_s.png", "bid/h13_s.png", NULL, NULL);
		auto wholeItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(DuplicateScene::menuDupCallback, this, DUP_K), item0, item1, NULL);
		wholeItem->setPosition(Vec2(x, y));
		auto menu = Menu::create(wholeItem, NULL);
		this->addChild(menu, 1);
		x += 150 + gap;
	}

	if (dups[1]) {
		auto item0 = MenuItemImage::create("cards/h12.png", "cards/h12.png", NULL, NULL);
		auto item1 = MenuItemImage::create("bid/h12_s.png", "bid/h12_s.png", NULL, NULL);
		auto wholeItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(DuplicateScene::menuDupCallback, this, DUP_H_Q), item0, item1, NULL);
		wholeItem->setPosition(Vec2(x, y));
		auto menu = Menu::create(wholeItem, NULL);
		this->addChild(menu, 1);
		x += 150 + gap;
	}

	if (dups[2]) {
		auto item0 = MenuItemImage::create("cards/c12.png", "cards/c12.png", NULL, NULL);
		auto item1 = MenuItemImage::create("bid/c12_s.png", "bid/c12_s.png", NULL, NULL);
		auto wholeItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(DuplicateScene::menuDupCallback, this, DUP_C_Q), item0, item1, NULL);
		wholeItem->setPosition(Vec2(x, y));
		auto menu = Menu::create(wholeItem, NULL);
		this->addChild(menu, 1);
		x += 150 + gap;
	}

	if (dups[3]) {
		auto item0 = MenuItemImage::create("cards/d12.png", "cards/d12.png", NULL, NULL);
		auto item1 = MenuItemImage::create("bid/d12_s.png", "bid/d12_s.png", NULL, NULL);
		auto wholeItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(DuplicateScene::menuDupCallback, this, DUP_D_Q), item0, item1, NULL);
		wholeItem->setPosition(Vec2(x, y));
		auto menu = Menu::create(wholeItem, NULL);
		this->addChild(menu, 1);
		x += 150 + gap;
	}

	if (dups[4]) {
		auto item0 = MenuItemImage::create("cards/s12.png", "cards/s12.png", NULL, NULL);
		auto item1 = MenuItemImage::create("bid/s12_s.png", "bid/s12_s.png", NULL, NULL);
		auto wholeItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(DuplicateScene::menuDupCallback, this, DUP_S_Q), item0, item1, NULL);
		wholeItem->setPosition(Vec2(x, y));
		auto menu = Menu::create(wholeItem, NULL);
		this->addChild(menu, 1);
	}
}

void DuplicateScene::menuBackCallback(cocos2d::Ref* pSender)
{
	this->removeFromParent();
	gameScene->setEnabled(true);
	gameScene->controller->setUserDuplicate(duplicates);
}

void DuplicateScene::menuDupCallback(cocos2d::Ref* pSender, int val)
{
	auto toggleItem = dynamic_cast<MenuItemToggle*>(pSender);
	int selected = toggleItem->getSelectedIndex();
	if (selected == 0 ) {
		REMOVE_BIT(duplicates, val);
	} else if (selected == 1) {
		SET_BIT(duplicates, val);
	}
}
