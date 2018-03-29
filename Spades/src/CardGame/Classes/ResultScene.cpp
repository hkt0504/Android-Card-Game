#include "ResultScene.h"
#include "common.h"
#include "GameScene.h"
#include "engine/Game.h"
#include "PlayerObject.h"

USING_NS_CC;

#define LABEL_TITLE_SIZE	25

Scene* ResultScene::createScene(GameScene *parent)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = ResultScene::create();
	layer->gameScene = parent;
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool ResultScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!LayerColor::initWithColor(ccc4(0, 0, 0, 0)))
	{
		return false;
	}

	viewMode = ANYWHERE;
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	center = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);

	CCLog(" Call ResultScene::init()");
	Sprite *spr = Sprite::create("result_board.png");
	spr->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height / 2 - 20));
	this->addChild(spr, 1);

	bgSize = spr->getContentSize();

	float x = origin.x + (visibleSize.width - bgSize.width) / 2.0;
	float y = origin.y + (visibleSize.height - bgSize.height) / 2.0;

	auto backItem = MenuItemImage::create("Buttons/btn_close.png", "Buttons/btn_close_d.png", CC_CALLBACK_1(ResultScene::menuBackCallback, this));
	backItem->setPosition(Vec2(x + bgSize.width - backItem->getContentSize().width/2 - 10, y + bgSize.height - backItem->getContentSize().height/2 - 30));
	auto menuBack = Menu::create(backItem, NULL);
	menuBack->setPosition(Vec2::ZERO);
	this->addChild(menuBack, 2);

	char text[100];

	float orgX = 670, orgY = 410;
	float marginY = 64, marginX = 190;
	float fontSize = 40;

	/* First Score Field */
	sprintf(text, "%d", 0);
	LabelTTF *trick1 = LabelTTF::create(text, "Felt", fontSize);
	trick1->setPosition(Vec2(orgX, orgY));
	this->addChild(trick1, 10, 1);
	trick1->setColor(Color3B::WHITE);

	sprintf(text, "%d", 0);
	LabelTTF *trick2 = LabelTTF::create(text, "Felt", fontSize);
	trick2->setPosition(Vec2(orgX, orgY - marginY));
	this->addChild(trick2, 10, 2);
	trick2->setColor(Color3B::WHITE);

	sprintf(text, "%d", 0);
	LabelTTF *score1 = LabelTTF::create(text, "Felt", fontSize);
	score1->setPosition(Vec2(orgX, orgY - marginY * 2));
	this->addChild(score1, 10, 3);
	score1->setColor(Color3B::WHITE);

	sprintf(text, "%d", 0);
	LabelTTF *bag1 = LabelTTF::create(text, "Felt", fontSize);
	bag1->setPosition(Vec2(orgX, orgY - marginY * 3));
	this->addChild(bag1, 10, 4);
	bag1->setColor(Color3B::WHITE);

	/* Second Score Field */
	sprintf(text, "%d", 0);
	LabelTTF *trick3 = LabelTTF::create(text, "Felt", fontSize);
	trick3->setPosition(Vec2(orgX + marginX, orgY));
	this->addChild(trick3, 10, 5);
	trick3->setColor(Color3B::WHITE);

	sprintf(text, "%d", 0);
	LabelTTF *trick4 = LabelTTF::create(text, "Felt", fontSize);
	trick4->setPosition(Vec2(orgX + marginX, orgY - marginY));
	this->addChild(trick4, 10, 6);
	trick4->setColor(Color3B::WHITE);

	sprintf(text, "%d", 0);
	LabelTTF *score2 = LabelTTF::create(text, "Felt", fontSize);
	score2->setPosition(Vec2(orgX + marginX, orgY - marginY * 2));
	this->addChild(score2, 10, 7);
	score2->setColor(Color3B::WHITE);

	sprintf(text, "%d", 0);
	LabelTTF *bag2 = LabelTTF::create(text, "Felt", fontSize);
	bag2->setPosition(Vec2(orgX + marginX, orgY - marginY * 3));
	this->addChild(bag2, 10, 8);
	bag2->setColor(Color3B::WHITE);

	return true;

}

void ResultScene::viewResult(int mode)
{
	this->viewMode = mode;
	float x = origin.x + (visibleSize.width - bgSize.width) / 2.0;
	float y = origin.y + (visibleSize.height - bgSize.height) / 2.0;

	char text[100];
	GameResult result = gameScene->game->getGameResult();

	/* First Score Field */
	sprintf(text, "%d", result.bid_user);
	LabelTTF *trick1 = (LabelTTF *)getChildByTag(1);
	trick1->setString(text);

	sprintf(text, "%d", result.trick_user);
	LabelTTF *trick2 = (LabelTTF *)getChildByTag(2);
	trick2->setString(text);

	sprintf(text, "%d", result.bag_user);
	LabelTTF *score1 = (LabelTTF *)getChildByTag(3);
	score1->setString(text);

	sprintf(text, "%d", result.score_user);
	LabelTTF *bag1 = (LabelTTF *)getChildByTag(4);
	bag1->setString(text);

	/* Second Score Field */
	sprintf(text, "%d", result.bid_bot);
	LabelTTF *trick3 = (LabelTTF *)getChildByTag(5);
	trick3->setString(text);

	sprintf(text, "%d", result.trick_bot);
	LabelTTF *trick4 = (LabelTTF *)getChildByTag(6);
	trick4->setString(text);

	sprintf(text, "%d", result.bag_bot);
	LabelTTF *score2 = (LabelTTF *)getChildByTag(7);
	score2->setString(text);

	sprintf(text, "%d", result.score_bot);
	LabelTTF *bag2 = (LabelTTF *)getChildByTag(8);
	bag2->setString(text);

}

void ResultScene::menuBackCallback(Ref* pSender)
{
	setPosition(-visibleSize.width, -visibleSize.height);
	gameScene->setEnabled(true);

	switch (viewMode) {
	case ROUND:
		gameScene->newRound();
		break;
	case GAMEOVER:
		gameScene->back();
		break;
	default:
		break;
	}
}
