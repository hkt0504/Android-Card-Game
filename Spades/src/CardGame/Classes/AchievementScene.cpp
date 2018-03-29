#include "AchievementScene.h"
#include "UserScene.h"
#include "common.h"

USING_NS_CC;

Scene* AchievementScene::createScene(int win)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = AchievementScene::create();
	layer->setWin(win);

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool AchievementScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}

	this->setKeypadEnabled(true);

	visibleSize = Director::getInstance()->getWinSizeInPixels();
	origin = Director::getInstance()->getVisibleOrigin();

	cocos2d::Sprite *spriteBg = Sprite::create("bg4.jpg");
	Size imgSize = spriteBg->getContentSize();
	spriteBg->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height / 2));
	this->addChild(spriteBg, 0);

	auto backItem = MenuItemImage::create("Buttons/btn_back.png", "Buttons/btn_back_d.png", CC_CALLBACK_1(AchievementScene::menuBackCallback, this));
	backItem->setPosition(Vec2(origin.x + backItem->getContentSize().width / 2 + BACK_BTN_MARGIN,
			origin.y + visibleSize.height - backItem->getContentSize().height / 2 - BACK_BTN_MARGIN));
	auto menuBack = Menu::create(backItem, NULL);
	menuBack->setPosition(Vec2::ZERO);
	this->addChild(menuBack, 1);

	Sprite *titleBg = Sprite::create("other/achiv_title.png");
	titleBg->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - titleBg->getContentSize().height / 2 - 50));
	this->addChild(titleBg, 2);

	Sprite *boardBg = Sprite::create("other/achiv_board.png");
	boardBg->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 60);
	this->addChild(boardBg, 2);

	boardSize = boardBg->getContentSize();
	boardOrigin = boardBg->getPosition();
	boardOrigin.x -= boardSize.width / 2;
	boardOrigin.y -= boardSize.height / 2;

	return true;
}

void AchievementScene::setWin(int win)
{
	Sprite *junior = Sprite::create((win >= JUNIOR) ? "other/achiv_junior.png" : "other/achiv_junior_d.png");
	junior->setAnchorPoint(Vec2::ZERO);
	junior->setPosition(boardOrigin.x + 33, boardOrigin.y + 207);
	addChild(junior, 3);

	Sprite *senior = Sprite::create((win >= SENIOR) ? "other/achiv_senior.png" : "other/achiv_senior_d.png");
	senior->setAnchorPoint(Vec2::ZERO);
	senior->setPosition(boardOrigin.x + 575, boardOrigin.y + 207);
	addChild(senior, 3);

	Sprite *excellent = Sprite::create((win >= EXCELLENT) ? "other/achiv_excellent.png" : "other/achiv_excellent_d.png");
	excellent->setAnchorPoint(Vec2::ZERO);
	excellent->setPosition(boardOrigin.x + 33, boardOrigin.y + 117);
	addChild(excellent, 3);

	Sprite *talent = Sprite::create((win >= TALENT) ? "other/achiv_talent.png" : "other/achiv_talent_d.png");
	talent->setAnchorPoint(Vec2::ZERO);
	talent->setPosition(boardOrigin.x + 575, boardOrigin.y + 117);
	addChild(talent, 3);

	Sprite *genius = Sprite::create((win >= GENIUS) ? "other/achiv_genius.png" : "other/achiv_genius_d.png");
	genius->setAnchorPoint(Vec2::ZERO);
	genius->setPosition(boardOrigin.x + 33, boardOrigin.y + 30);
	addChild(genius, 3);
}

void AchievementScene::menuBackCallback(Ref* pSender)
{
	Scene *pScene = TransitionFade::create(0.5, UserScene::createScene());
	Director:: getInstance()->replaceScene(pScene);
}

void AchievementScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		menuBackCallback(NULL);
	}
}
