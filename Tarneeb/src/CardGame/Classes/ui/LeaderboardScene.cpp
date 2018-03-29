#include "LeaderboardScene.h"
#include "UserScene.h"
#include "common.h"
#include "Facebook.h"
#include "AppService.h"
#include "ui/UIScrollView.h"

USING_NS_CC;

#define SCROLL_VIEW_TAG		10

Scene* LeaderboardScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = LeaderboardScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool LeaderboardScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() ) {
		return false;
	}

	this->setKeypadEnabled(true);

	visibleSize = Director::getInstance()->getWinSizeInPixels();
	origin = Director::getInstance()->getVisibleOrigin();
	Vec2 center = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);

	cocos2d::Sprite *spriteBg = Sprite::create("bg1.jpg");
	Size imgSize = spriteBg->getContentSize();
	spriteBg->setPosition(center);
	this->addChild(spriteBg, 0);

	auto moreGameItem = MenuItemImage::create("Buttons/more_game.png", "Buttons/more_game.png",
			CC_CALLBACK_1(LeaderboardScene::menuMoreGame, this));
	moreGameItem->setPosition(Vec2(center.x - moreGameItem->getContentSize().width / 2 - 20,
			origin.y + visibleSize.height - moreGameItem->getContentSize().height / 2 - 50));
	auto menuMoreGame = Menu::create(moreGameItem, NULL);
	menuMoreGame->setPosition(Vec2::ZERO);
	this->addChild(menuMoreGame, 1);

	auto rateGameItem = MenuItemImage::create("Buttons/rate_game.png", "Buttons/rate_game.png",
			CC_CALLBACK_1(LeaderboardScene::menuRateGame, this));
	rateGameItem->setPosition(Vec2(center.x + rateGameItem->getContentSize().width / 2 + 20,
			origin.y + visibleSize.height - rateGameItem->getContentSize().height / 2 - 50));
	auto menuRateGame = Menu::create(rateGameItem, NULL);
	menuRateGame->setPosition(Vec2::ZERO);
	this->addChild(menuRateGame, 1);

	auto backItem = MenuItemImage::create("Buttons/btn_back.png", "Buttons/btn_back.png",
			CC_CALLBACK_1(LeaderboardScene::menuBackCallback, this));
	backItem->setPosition(Vec2(origin.x + backItem->getContentSize().width / 2 + 50,
			origin.y + visibleSize.height - backItem->getContentSize().height / 2 - 50));
	auto menuBack = Menu::create(backItem, NULL);
	menuBack->setPosition(Vec2::ZERO);
	this->addChild(menuBack, 1);

	Sprite *titleBg = Sprite::create("other/top_players.png");
	titleBg->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - rateGameItem->getContentSize().height - titleBg->getContentSize().height / 2 - 65));
	this->addChild(titleBg, 2);

	Sprite *boardBg = Sprite::create("other/leaderboard.png");
	boardBg->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 90);
	this->addChild(boardBg, 2);

	boardSize = boardBg->getContentSize();
	boardOrigin = boardBg->getPosition();
	boardOrigin.x = 0;
	boardOrigin.y -= boardSize.height / 2;

	ui::ScrollView* scrollView = ui::ScrollView::create();

	scrollView->setTouchEnabled(true);
	scrollView->setDirection(ui::ScrollView::Direction::VERTICAL);
	scrollView->setContentSize(Size(boardSize.width, boardSize.height - 70));
	float innerWidth = boardSize.width;
	float innerHeight = -boardSize.height;

	scrollView->setInnerContainerSize(Size(innerWidth, innerHeight));

	scrollView->setPosition(Vec2(visibleSize.width / 2 - boardSize.width / 2, origin.y + visibleSize.height / 2 - 90 - boardSize.height / 2));
	this->addChild(scrollView, 3, SCROLL_VIEW_TAG);

	loadScores();

	return true;
}

void LeaderboardScene::menuBackCallback(Ref* pSender)
{
	Scene *pScene = TransitionFade::create(0.5, UserScene::createScene());
	Director:: getInstance()->replaceScene(pScene);
}

void LeaderboardScene::menuMoreGame(Ref* pSender)
{
	AppService::openMoreGame();
}

void LeaderboardScene::menuRateGame(Ref* pSender)
{
	AppService::openRateGame();
}

void LeaderboardScene::onFacebook(GameScore scores[], int count)
{
	int index = 0;
	for (int i = 0; i < count; i++) {
		if (scores[i].name) {
			addScoreField((count - index), scores[i]);
			index++;
		}
	}

	ui::ScrollView* scrollView = (ui::ScrollView*)getChildByTag(SCROLL_VIEW_TAG);
	if(!scrollView)
		return;

	float innerWidth = boardSize.width;
	float innerHeight = (count + 1) * 80;

	scrollView->setInnerContainerSize(Size(innerWidth, innerHeight));
}

void LeaderboardScene::loadScores()
{
#ifdef FACEBOOK
	Facebook::leaderboardScene = this;
	Facebook::getScores();
#else
	GameScore scores[10];
	scores[0].name = "Test User Test1";
	scores[0].win = 10;
	scores[0].lose = 10;

	scores[1].name = "Test User Test2";
	scores[1].win = 10;
	scores[1].lose = 10;

	scores[2].name = "Test User Test3";
	scores[2].win = 10;
	scores[2].lose = 10;

	scores[3].name = "Test User Test4";
	scores[3].win = 10;
	scores[3].lose = 10;

	scores[4].name = "Test User Test5";
	scores[4].win = 10;
	scores[4].lose = 10;

	scores[5].name = "Test User Test6";
	scores[5].win = 10;
	scores[5].lose = 10;

	scores[6].name = "Test User Test7";
	scores[6].win = 10;
	scores[6].lose = 10;

	scores[7].name = "Test User Test8";
	scores[7].win = 10;
	scores[7].lose = 10;

	scores[8].name = "Test User Test9";
	scores[8].win = 10;
	scores[8].lose = 10;

	scores[9].name = "Test User Test10";
	scores[9].win = 10;
	scores[9].lose = 10;

	onFacebook(scores, 10);
#endif //FACEBOOK
}

void LeaderboardScene::addScoreField(int index, GameScore score)
{
	ui::ScrollView* scrollView = (ui::ScrollView*)getChildByTag(SCROLL_VIEW_TAG);
	if(!scrollView)
		return;

	CCLog("LeaderboardScene::addScoreField() %s, %d, %d, %d", score.name, score.win, score.lose, score.win * 10 - score.lose * 7);

	Vec2 pos = Vec2(boardOrigin.x + 22, index * 80);
	Sprite *bg = Sprite::create("other/leaderboard_item.png");
	bg->setAnchorPoint(Vec2::ZERO);
	bg->setPosition(pos);
	scrollView->addChild(bg, 3);

	LabelTTF *labelName = LabelTTF::create(score.name, "Felt", 30);
	labelName->setPosition(Vec2(pos.x + 120, pos.y + 30));
	labelName->setColor(Color3B::WHITE);
	scrollView->addChild(labelName, 4);

	char text[100] = {0,};
	sprintf(text, "%d", score.win);

	LabelTTF *labelWin = LabelTTF::create(text, "Felt", 30);
	labelWin->setPosition(Vec2(pos.x + 355, pos.y + 30));
	labelWin->setColor(Color3B::BLACK);
	scrollView->addChild(labelWin, 4);

	sprintf(text, "%d", score.lose);
	LabelTTF *labelLose = LabelTTF::create(text, "Felt", 30);
	labelLose->setPosition(Vec2(pos.x + 600, pos.y + 30));
	labelLose->setColor(Color3B::BLACK);
	scrollView->addChild(labelLose, 4);

	sprintf(text, "%d", score.score());
	LabelTTF *labelScore = LabelTTF::create(text, "Felt", 30);
	labelScore->setPosition(Vec2(pos.x + 840, pos.y + 30));
	labelScore->setColor(Color3B::BLACK);
	scrollView->addChild(labelScore, 4);
}

void LeaderboardScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		menuBackCallback(NULL);
	}
}
