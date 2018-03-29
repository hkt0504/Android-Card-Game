#include "BidScene.h"
#include "common.h"
#include "GameScene.h"
#include "AppService.h"
#include "Controller.h"

USING_NS_CC;

#define KING_TAG    11
#define QUEEN_TAG   12
#define DIAMOND_TAG 13
#define LOTOSH_TAG  14
#define TRIX_TAG    15
#define COMPLEX_TAG 16

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

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object

	if (AppService::isComplex()) {
		initComplex();
	} else {
		initKingdom();
	}

	CCLog(" Call BidScene::init()");
	return true;
}

void BidScene::initKingdom()
{
	Sprite *spr = Sprite::create("bid/bid_bg.png");
	spr->setPosition(center);
	this->addChild(spr, 1, 0);

	float contentWidth = spr->getContentSize().width;
	float contentHeight = spr->getContentSize().height;

	float deltaX, deltaY;

	kingItem = MenuItemImage::create("bid/bid_king.png", "bid/bid_king_d.png", "bid/bid_king_d.png", CC_CALLBACK_1(BidScene::menuCallback, this, BT_KING));
	deltaX = center.x - 10 - kingItem->getContentSize().width * 2;
	deltaY = center.y + contentHeight / 2 - kingItem->getContentSize().height / 2 - 140;
	kingItem->setPosition(Vec2(deltaX, deltaY));
	auto menuKing = Menu::create(kingItem, NULL);
	menuKing->setPosition(Vec2::ZERO);
	this->addChild(menuKing, 2, KING_TAG);

	deltaX += kingItem->getContentSize().width + 10;

	queenItem = MenuItemImage::create("bid/bid_queen.png", "bid/bid_queen_d.png", "bid/bid_queen_d.png", CC_CALLBACK_1(BidScene::menuCallback, this, BT_QUEEN));
	queenItem->setPosition(Vec2(deltaX, deltaY));
	auto menuQueen = Menu::create(queenItem, NULL);
	menuQueen->setPosition(Vec2::ZERO);
	this->addChild(menuQueen, 2, QUEEN_TAG);

	deltaX += queenItem->getContentSize().width + 10;

	diamondItem = MenuItemImage::create("bid/bid_diamond.png", "bid/bid_diamond_d.png", "bid/bid_diamond_d.png", CC_CALLBACK_1(BidScene::menuCallback, this, BT_DIAMOND));
	diamondItem->setPosition(Vec2(deltaX, deltaY));
	auto menuDiamond = Menu::create(diamondItem, NULL);
	menuDiamond->setPosition(Vec2::ZERO);
	this->addChild(menuDiamond, 2, DIAMOND_TAG);

	deltaX += diamondItem->getContentSize().width + 10;

	lotoshItem = MenuItemImage::create("bid/bid_lotosh.png", "bid/bid_lotosh_d.png", "bid/bid_lotosh_d.png", CC_CALLBACK_1(BidScene::menuCallback, this, BT_LOTOSH));
	lotoshItem->setPosition(Vec2(deltaX, deltaY));
	auto menuLotosh = Menu::create(lotoshItem, NULL);
	menuLotosh->setPosition(Vec2::ZERO);
	this->addChild(menuLotosh, 2, LOTOSH_TAG);

	deltaX += lotoshItem->getContentSize().width + 10;

	trixItem = MenuItemImage::create("bid/bid_trix.png", "bid/bid_trix_d.png", "bid/bid_trix_d.png", CC_CALLBACK_1(BidScene::menuCallback, this, BT_TRIX));
	trixItem->setPosition(Vec2(deltaX, deltaY));
	auto menuTrix = Menu::create(trixItem, NULL);
	menuTrix->setPosition(Vec2::ZERO);
	this->addChild(menuTrix, 2, TRIX_TAG);
}

void BidScene::initComplex()
{
	Sprite *spr = Sprite::create("bid/bid_bg.png");
	spr->setPosition(center);
	this->addChild(spr, 1, 0);

	float contentWidth = spr->getContentSize().width;
	float contentHeight = spr->getContentSize().height;

	float deltaX, deltaY;

	complexItem = MenuItemImage::create("bid/bid_complex.png", "bid/bid_complex_d.png", "bid/bid_complex_d.png", CC_CALLBACK_1(BidScene::menuCallback, this, BT_COMPLEX));
	deltaX = center.x - complexItem->getContentSize().width / 2;
	deltaY = center.y + contentHeight / 2 - complexItem->getContentSize().height / 2 - 140;
	complexItem->setPosition(Vec2(deltaX, deltaY));
	auto menuComplex = Menu::create(complexItem, NULL);
	menuComplex->setPosition(Vec2::ZERO);
	this->addChild(menuComplex, 2, COMPLEX_TAG);

	deltaX += complexItem->getContentSize().width + 10;
	trixItem = MenuItemImage::create("bid/bid_trix.png", "bid/bid_trix_d.png", "bid/bid_trix_d.png", CC_CALLBACK_1(BidScene::menuCallback, this, BT_TRIX));
	trixItem->setPosition(Vec2(deltaX, deltaY));
	auto menuTrix = Menu::create(trixItem, NULL);
	menuTrix->setPosition(Vec2::ZERO);
	this->addChild(menuTrix, 2, TRIX_TAG);
}

void BidScene::menuCallback(Ref* pSender, int val)
{
	CCLog("Bid val (%d)", val);

	this->removeFromParent();
	gameScene->setEnabled(true);
	gameScene->controller->setUserBid((BidType) val);
}

void BidScene::setEnabledBids(int bid)
{
	if (AppService::isComplex()) {
		if ((bid & BT_TRIX) == 0) {
			Menu *menu = (Menu *) this->getChildByTag(TRIX_TAG);
			menu->setEnabled(false);
			trixItem->setEnabled(false);
		}

		if ((bid & BT_COMPLEX) == 0) {
			Menu *menu = (Menu *) this->getChildByTag(COMPLEX_TAG);
			menu->setEnabled(false);
			complexItem->setEnabled(false);
		}
	} else {
		if ((bid & BT_KING) == 0) {
			Menu *menu = (Menu *) this->getChildByTag(KING_TAG);
			menu->setEnabled(false);
			kingItem->setEnabled(false);
		}

		if ((bid & BT_QUEEN) == 0) {
			Menu *menu = (Menu *) this->getChildByTag(QUEEN_TAG);
			menu->setEnabled(false);
			queenItem->setEnabled(false);
		}

		if ((bid & BT_DIAMOND) == 0) {
			Menu *menu = (Menu *) this->getChildByTag(DIAMOND_TAG);
			menu->setEnabled(false);
			diamondItem->setEnabled(false);
		}

		if ((bid & BT_LOTOSH) == 0) {
			Menu *menu = (Menu *) this->getChildByTag(LOTOSH_TAG);
			menu->setEnabled(false);
			lotoshItem->setEnabled(false);
		}

		if ((bid & BT_TRIX) == 0) {
			Menu *menu = (Menu *) this->getChildByTag(TRIX_TAG);
			menu->setEnabled(false);
			trixItem->setEnabled(false);
		}
	}
}
