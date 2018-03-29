#include "ResultScene.h"
#include "common.h"
#include "GameScene.h"
#include "Controller.h"
#include "PlayerObject.h"
#include "GameResult.h"
#include "AppService.h"
#include "ui/UIScrollView.h"

USING_NS_CC;

#define FONT_SIZE			26

#define SCROLL_VIEW_TAG		10

#define LABEL_TOTAL1_TAG	21
#define LABEL_TOTAL2_TAG	22
#define LABEL_TOTAL3_TAG	23
#define LABEL_TOTAL4_TAG	24

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
	auto spr = Sprite::create("bid/result_bg.png");
	spr->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height / 2));
	this->addChild(spr, 1);

	bgSize = spr->getContentSize();
	bgOrigin = spr->getPosition();
	bgOrigin.x = 0;
	bgOrigin.y -= bgSize.height / 2;

	float x = origin.x + (visibleSize.width - bgSize.width) / 2.0;
	float y = origin.y + (visibleSize.height - bgSize.height) / 2.0;

	auto backItem = MenuItemImage::create("Buttons/btn_close.png", "Buttons/btn_close_d.png", CC_CALLBACK_1(ResultScene::menuBackCallback, this));
	backItem->setPosition(Vec2(x + bgSize.width - backItem->getContentSize().width/2 - 20, y + bgSize.height - backItem->getContentSize().height/2 - 20));
	auto menuBack = Menu::create(backItem, NULL);
	menuBack->setPosition(Vec2::ZERO);
	this->addChild(menuBack, 2);

	float orgX = 340, orgY = y + bgSize.height - 56;
	float marginX = 204;
	float fontSize = 40;

	bool isPartner = AppService::isPartner();

	// scrollview
	ui::ScrollView* scrollView = ui::ScrollView::create();
	scrollView->setTouchEnabled(true);
	scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	scrollView->setContentSize(Size(bgSize.width, bgSize.height - 180));
	float innerWidth = bgSize.width;
	float innerHeight = -bgSize.height;
	scrollView->setInnerContainerSize(Size(innerWidth, innerHeight));
	scrollView->setPosition(Vec2(visibleSize.width / 2 - bgSize.width / 2, origin.y + visibleSize.height / 2 + 80 - bgSize.height / 2));
	this->addChild(scrollView, 3, SCROLL_VIEW_TAG);

	// total field
	if (isPartner) {
		Vec2 pos = Vec2(163.5, (visibleSize.height - bgSize.height) / 2 + 20);
		Sprite *bg = Sprite::create("bid/result_total_item2.png");
		bg->setPosition(Vec2(center.x, (visibleSize.height - bgSize.height) / 2 + 20 + bg->getContentSize().height / 2));
		this->addChild(bg, 3);

		auto labelScore1 = LabelTTF::create("0", "Felt", FONT_SIZE);
		labelScore1->setPosition(Vec2(pos.x + 346, pos.y + 25));
		labelScore1->setColor(Color3B::WHITE);
		this->addChild(labelScore1, 4, LABEL_TOTAL1_TAG);

		auto labelScore2 = LabelTTF::create("0", "Felt", FONT_SIZE);
		labelScore2->setPosition(Vec2(pos.x + 752, pos.y + 25));
		labelScore2->setColor(Color3B::WHITE);
		this->addChild(labelScore2, 4, LABEL_TOTAL2_TAG);
	} else {
		Vec2 pos = Vec2(163.5, (visibleSize.height - bgSize.height) / 2 + 20);
		Sprite *bg = Sprite::create("bid/result_total_item.png");
		bg->setPosition(Vec2(center.x, (visibleSize.height - bgSize.height) / 2 + 20 + bg->getContentSize().height / 2));
		this->addChild(bg, 3);

		auto labelScore1 = LabelTTF::create("0", "Felt", FONT_SIZE);
		labelScore1->setPosition(Vec2(pos.x + 236, pos.y + 25));
		labelScore1->setColor(Color3B::WHITE);
		this->addChild(labelScore1, 4, LABEL_TOTAL1_TAG);

		auto labelScore2 = LabelTTF::create("0", "Felt", FONT_SIZE);
		labelScore2->setPosition(Vec2(pos.x + 440, pos.y + 25));
		labelScore2->setColor(Color3B::WHITE);
		this->addChild(labelScore2, 4, LABEL_TOTAL2_TAG);

		auto labelScore3 = LabelTTF::create("0", "Felt", FONT_SIZE);
		labelScore3->setPosition(Vec2(pos.x + 644, pos.y + 25));
		labelScore3->setColor(Color3B::WHITE);
		this->addChild(labelScore3, 4, LABEL_TOTAL3_TAG);

		auto labelScore4 = LabelTTF::create("0", "Felt", FONT_SIZE);
		labelScore4->setPosition(Vec2(pos.x + 852, pos.y + 25));
		labelScore4->setColor(Color3B::WHITE);
		this->addChild(labelScore4, 4, LABEL_TOTAL4_TAG);
	}

	return true;
}

void ResultScene::viewResult(int mode)
{
	this->viewMode = mode;

	GameResult *result = gameScene->controller->getGameResult();

	int kingdomCount = result->getKingdomCount();
	int count = result->getRoundCount();

	ui::ScrollView* scrollView = (ui::ScrollView*)getChildByTag(SCROLL_VIEW_TAG);
	scrollView->removeAllChildren();
	float innerWidth = bgSize.width;
	float innerHeight = kingdomCount * 320;
	scrollView->setInnerContainerSize(Size(innerWidth, innerHeight));

	float ypos = (kingdomCount - 1) * 320 + 160;
	float scoreYPos = 0;

	if (ypos < bgSize.height - 350) {
		ypos = bgSize.height - 350;
	}

	int index = 0;
	int itemCount = AppService::isComplex() ? 2 : 5;
	for (int i = 0; i < kingdomCount; i++) {
		addKingdomField(ypos, result->getKingdom(i));

		scoreYPos = ypos;
		for (int j = 0; j < itemCount && index < count; j++, index++) {
			addScoreField(scoreYPos, result->getRoundScore(index));
			scoreYPos -= 36;
		}

		ypos -= 320;
	}

	setTotalField(result->getTotalScore(0), result->getTotalScore(1), result->getTotalScore(2), result->getTotalScore(3));
}

void ResultScene::menuBackCallback(Ref* pSender)
{
	setPosition(-visibleSize.width, -visibleSize.height);
	gameScene->setEnabled(true);

	switch (viewMode) {
	case ROUND:
		gameScene->newRound();
		break;
	default:
		break;
	}
}

void ResultScene::addKingdomField(float ypos, int player)
{
	CCLog("Call ResultScene::addKingdomField s");
	ui::ScrollView* scrollView = (ui::ScrollView*)getChildByTag(SCROLL_VIEW_TAG);
	if (!scrollView)
		return;

	Sprite *itemBg = NULL;
	if (AppService::isPartner()) {
		itemBg = Sprite::create("bid/result_item2.png");
	} else {
		itemBg = Sprite::create("bid/result_item.png");
	}
	itemBg->setPosition(Vec2(bgOrigin.x + bgSize.width / 2, ypos - 30));
	scrollView->addChild(itemBg, 4);

	char name[40];
	switch (player) {
	case 0:
		strcpy(name, "KINGDOM: YOU");
		break;
	case 1:
		sprintf(name, "KINGDOM: %s", PROFILE_NAMES_UP[gameScene->player[1]->avatarIndex]);
		break;
	case 2:
		sprintf(name, "KINGDOM: %s", PROFILE_NAMES_UP[gameScene->player[2]->avatarIndex]);
		break;
	case 3:
		sprintf(name, "KINGDOM: %s", PROFILE_NAMES_UP[gameScene->player[3]->avatarIndex]);
		break;
	default:
		strcpy(name, "KINGDOM");
		break;
	}

	auto labelName = LabelTTF::create(name, "Felt", 36);
	labelName->setPosition(Vec2(bgOrigin.x + bgSize.width / 2, ypos + itemBg->getContentSize().height / 2 + 10));
	labelName->setColor(Color3B::WHITE);
	scrollView->addChild(labelName, 4);

	Vec2 pos = Vec2(bgOrigin.x + 22, ypos + 64);
	/* Title Field */
	if (AppService::isPartner()) {
		auto name1 = LabelTTF::create("Us", "Felt", FONT_SIZE);
		name1->setPosition(pos.x + 346, pos.y);
		name1->setColor(Color3B::WHITE);
		name1->setHorizontalAlignment(TextHAlignment::LEFT);
		scrollView->addChild(name1, 10);

		auto name2 = LabelTTF::create("Them", "Felt", FONT_SIZE);
		name2->setPosition(pos.x + 752, pos.y);
		name2->setColor(Color3B::WHITE);
		name2->setHorizontalAlignment(TextHAlignment::LEFT);
		scrollView->addChild(name2, 10);
	} else {
		auto name0 = LabelTTF::create("You", "Felt", FONT_SIZE);
		name0->setPosition(pos.x + 236, pos.y);
		name0->setColor(Color3B::WHITE);
		name0->setHorizontalAlignment(TextHAlignment::LEFT);
		scrollView->addChild(name0, 10);

		int index = gameScene->player[1]->avatarIndex;
		auto name1 = LabelTTF::create(PROFILE_NAMES[index], "Felt", FONT_SIZE);
		name1->setPosition(pos.x + 440, pos.y);
		name1->setColor(Color3B::WHITE);
		name1->setHorizontalAlignment(TextHAlignment::LEFT);
		scrollView->addChild(name1, 10);

		index = gameScene->player[2]->avatarIndex;
		auto name2 = LabelTTF::create(PROFILE_NAMES[index], "Felt", FONT_SIZE);
		name2->setPosition(pos.x + 644, pos.y);
		name2->setColor(Color3B::WHITE);
		name2->setHorizontalAlignment(TextHAlignment::LEFT);
		scrollView->addChild(name2, 10);

		index = gameScene->player[3]->avatarIndex;
		auto name3 = LabelTTF::create(PROFILE_NAMES[index], "Felt", FONT_SIZE);
		name3->setPosition(pos.x + 852, pos.y);
		name3->setColor(Color3B::WHITE);
		name3->setHorizontalAlignment(TextHAlignment::LEFT);
		scrollView->addChild(name3, 10);
	}

	CCLog("Call ResultScene::addKingdomField e");
}

void ResultScene::addScoreField(float ypos, RoundScore *score)
{
	ui::ScrollView* scrollView = (ui::ScrollView*)getChildByTag(SCROLL_VIEW_TAG);
	if(!scrollView)
		return;

	if (AppService::isPartner()) {
		Vec2 pos = Vec2(bgOrigin.x + 22, ypos);

		auto labelType = LabelTTF::create(score->getTypeName(), "Felt", 28);
		labelType->setPosition(Vec2(pos.x + 70, pos.y + 30));
		labelType->setColor(Color3B::WHITE);
		scrollView->addChild(labelType, 5);

		char text[100] = {0,};
		sprintf(text, "%d", score->score1 + score->score3);

		auto labelScore1 = LabelTTF::create(text, "Felt", 28);
		labelScore1->setPosition(Vec2(pos.x + 346, pos.y + 30));
		labelScore1->setColor(Color3B::WHITE);
		scrollView->addChild(labelScore1, 5);

		sprintf(text, "%d", score->score2 + score->score4);
		auto labelScore2 = LabelTTF::create(text, "Felt", 28);
		labelScore2->setPosition(Vec2(pos.x + 752, pos.y + 30));
		labelScore2->setColor(Color3B::WHITE);
		scrollView->addChild(labelScore2, 5);
	} else {
		Vec2 pos = Vec2(bgOrigin.x + 22, ypos);

		auto labelType = LabelTTF::create(score->getTypeName(), "Felt", 28);
		labelType->setPosition(Vec2(pos.x + 70, pos.y + 30));
		labelType->setColor(Color3B::WHITE);
		scrollView->addChild(labelType, 5);

		char text[100] = {0,};
		sprintf(text, "%d", score->score1);

		auto labelScore1 = LabelTTF::create(text, "Felt", 28);
		labelScore1->setPosition(Vec2(pos.x + 236, pos.y + 30));
		labelScore1->setColor(Color3B::WHITE);
		scrollView->addChild(labelScore1, 5);

		sprintf(text, "%d", score->score2);
		auto labelScore2 = LabelTTF::create(text, "Felt", 28);
		labelScore2->setPosition(Vec2(pos.x + 440, pos.y + 30));
		labelScore2->setColor(Color3B::WHITE);
		scrollView->addChild(labelScore2, 5);

		sprintf(text, "%d", score->score3);
		auto labelScore3 = LabelTTF::create(text, "Felt", 28);
		labelScore3->setPosition(Vec2(pos.x + 644, pos.y + 30));
		labelScore3->setColor(Color3B::WHITE);
		scrollView->addChild(labelScore3, 5);

		sprintf(text, "%d", score->score4);
		auto labelScore4 = LabelTTF::create(text, "Felt", 28);
		labelScore4->setPosition(Vec2(pos.x + 852, pos.y + 30));
		labelScore4->setColor(Color3B::WHITE);
		scrollView->addChild(labelScore4, 5);
	}
}

void ResultScene::setTotalField(int total1, int total2, int total3, int total4)
{
	if (AppService::isPartner()) {
		char text[100] = {0,};
		sprintf(text, "%d", total1 + total3);
		auto labelTotal1 = (LabelTTF *) this->getChildByTag(LABEL_TOTAL1_TAG);
		labelTotal1->setString(text);

		sprintf(text, "%d", total2 + total4);
		auto labelTotal2 = (LabelTTF *) this->getChildByTag(LABEL_TOTAL2_TAG);
		labelTotal2->setString(text);
	} else {
		char text[100] = {0,};
		sprintf(text, "%d", total1);
		auto labelTotal1 = (LabelTTF *) this->getChildByTag(LABEL_TOTAL1_TAG);
		labelTotal1->setString(text);

		sprintf(text, "%d", total2);
		auto labelTotal2 = (LabelTTF *) this->getChildByTag(LABEL_TOTAL2_TAG);
		labelTotal2->setString(text);

		sprintf(text, "%d", total3);
		auto labelTotal3 = (LabelTTF *) this->getChildByTag(LABEL_TOTAL3_TAG);
		labelTotal3->setString(text);

		sprintf(text, "%d", total4);
		auto labelTotal4 = (LabelTTF *) this->getChildByTag(LABEL_TOTAL4_TAG);
		labelTotal4->setString(text);
	}
}
