#include "ResultScene.h"
#include "common.h"
#include "GameScene.h"
#include "Controller.h"
#include "PlayerObject.h"
#include "GameResult.h"
#include "ui/UIScrollView.h"

USING_NS_CC;

#define SCROLL_VIEW_TAG		10

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
	if (!LayerColor::initWithColor(ccc4(0, 0, 0, 0))) {
		return false;
	}

	viewMode = ANYWHERE;
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	center = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);

	Sprite *spr = Sprite::create("bid/result_board.png");
	spr->setPosition(center.x, origin.y + visibleSize.height / 2 - 20);
	this->addChild(spr, 1);

	bgSize = spr->getContentSize();

	float x = origin.x + (visibleSize.width - bgSize.width) / 2.0;
	float y = origin.y + (visibleSize.height - bgSize.height) / 2.0;

	auto backItem = MenuItemImage::create("Buttons/btn_close.png", "Buttons/btn_close.png", CC_CALLBACK_1(ResultScene::menuBackCallback, this));
	backItem->setPosition(Vec2(x + bgSize.width - backItem->getContentSize().width/2 - 10, y + bgSize.height - backItem->getContentSize().height/2 - 30));
	auto menuBack = Menu::create(backItem, NULL);
	menuBack->setPosition(Vec2::ZERO);
	this->addChild(menuBack, 5);

	// scrollview
	ui::ScrollView* scrollView = ui::ScrollView::create();
	scrollView->setTouchEnabled(true);
	scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	scrollView->setContentSize(Size(bgSize.width - 20, 192));
	float innerWidth = bgSize.width;
	float innerHeight = bgSize.height;
	scrollView->setInnerContainerSize(Size(innerWidth, innerHeight));
	scrollView->setPosition(Vec2(visibleSize.width / 2 - bgSize.width / 2 + 10, origin.y + visibleSize.height / 2 - 160));
	this->addChild(scrollView, 3, SCROLL_VIEW_TAG);

	return true;
}

void ResultScene::viewResult(int mode)
{
	this->viewMode = mode;
	float x = origin.x + (visibleSize.width - bgSize.width) / 2.0;
	float y = origin.y + (visibleSize.height - bgSize.height) / 2.0;

	GameResult *result = gameScene->controller->getGameResult();
	int count = result->getRoundCount();

	ui::ScrollView* scrollView = (ui::ScrollView*)getChildByTag(SCROLL_VIEW_TAG);
	scrollView->removeAllChildren();
	float innerWidth = bgSize.width;
	float innerHeight = count * 96;
	scrollView->setInnerContainerSize(Size(innerWidth, innerHeight));

	float ypos = count * 96 - 48;
	float scoreYPos = 0;

	if (ypos < 144) {
		ypos = 144;
	}

	int index = 0;
	for (int i = 0; i < count; i++) {
		addScoreField(ypos, result->getRoundScore(i));
		ypos -= 96;
	}

	int total1 = result->getTotalScore(0);
	int total2 = result->getTotalScore(1);
	char text[100];
	float fontSize = 40;

	/* First Score Field */
	sprintf(text, "%d", total1);
	LabelTTF *totalScore1 = LabelTTF::create(text, "Felt", fontSize);
	totalScore1->setPosition(Vec2(x + 296, y + 152));
	totalScore1->setColor(Color3B::WHITE);
	this->addChild(totalScore1, 10, 1);

	/* Second Score Field */
	sprintf(text, "%d", total2);
	LabelTTF *totalScore2 = LabelTTF::create(text, "Felt", fontSize);
	totalScore2->setPosition(Vec2(x + 640, y + 152));
	totalScore2->setColor(Color3B::WHITE);
	this->addChild(totalScore2, 10, 2);
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

void ResultScene::addScoreField(float ypos, RoundScore *score)
{
	ui::ScrollView* scrollView = (ui::ScrollView*)getChildByTag(SCROLL_VIEW_TAG);
	if (!scrollView)
		return;

	Sprite *itemBg = Sprite::create("bid/result_item.png");
	itemBg->setPosition(bgSize.width / 2 - 10, ypos);
	scrollView->addChild(itemBg, 4);

	char text[100];
	float fontSize = 40;

	/* First Score Field */
	sprintf(text, "%d", score->score1);
	LabelTTF *trick1 = LabelTTF::create(text, "Felt", fontSize);
	trick1->setPosition(Vec2(290, ypos));
	trick1->setColor(Color3B::WHITE);
	scrollView->addChild(trick1, 10, 1);

	/* Second Score Field */
	sprintf(text, "%d", score->score2);
	LabelTTF *trick3 = LabelTTF::create(text, "Felt", fontSize);
	trick3->setPosition(Vec2(620, ypos));
	trick3->setColor(Color3B::WHITE);
	scrollView->addChild(trick3, 10, 2);

}
