#include "common.h"
#include "GameScene.h"
#include "MenuScene.h"
#include "CardSprite.h"
#include "PlayerObject.h"
#include "CardDeck.h"
#include "PlayerObject.h"
#include "BidScene.h"
#include "ClosePopup.h"
#include "Game.h"
#include "Hand.h"
#include "ResultScene.h"
#include "UserScene.h"
#include "Facebook.h"

USING_NS_CC;

#define CLOSE_SCENE_TAG		20
#define RESULT_SCENE_TAG	21
#define NIL_SCENE_TAG		22
#define TABLE_BOARD_TAG		2

GameScene::GameScene()
{
	game = Game::create();
	game->retain();

	playedIndex = 0;
}

GameScene::~GameScene()
{
	game->release();
}

Scene* GameScene::createScene(int index)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameScene::create();
	layer->gameIndex = index;

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	this->setKeypadEnabled(true);

	enabled = true;
	game->setGameScene(this);
	playedIndex = 0;

	visibleSize = Director::getInstance()->getWinSizeInPixels();
	origin = Director::getInstance()->getVisibleOrigin();
	center = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object

	MenuItemImage *backItem = MenuItemImage::create("Buttons/btn_back.png", "Buttons/btn_back_d.png",
													CC_CALLBACK_1(GameScene::menuBackCallback, this));
	backItem->setPosition(Vec2(origin.x + visibleSize.width - backItem->getContentSize().width / 2 - BACK_BTN_MARGIN,
			origin.y + visibleSize.height - backItem->getContentSize().height / 2 - BACK_BTN_MARGIN));
	auto menuBack = Menu::create(backItem, NULL);
	menuBack->setPosition(Vec2::ZERO);
	this->addChild(menuBack, 1, BACK_ITEM_TAG);

	MenuItemImage *resultItem = MenuItemImage::create("Buttons/right_page_icon.png", "Buttons/right_page_icon.png", CC_CALLBACK_1(GameScene::menuResultCallback, this));
	resultItem->setPosition(Vec2(origin.x + resultItem->getContentSize().width / 2 + BACK_BTN_MARGIN,
			origin.y + visibleSize.height - resultItem->getContentSize().height / 2 - BACK_BTN_MARGIN));
	auto menuResult = Menu::create(resultItem, NULL);
	menuResult->setPosition(Vec2::ZERO);
	this->addChild(menuResult, 2, RESULT_ITEM_TAG);

	Sprite *spr = Sprite::create("play_table.png");
	//spr->setAnchorPoint(Point::ZERO);
	spr->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height / 2 - 10));
	this->addChild(spr, 3, TABLE_BOARD_TAG);
	spr->setVisible(false);
	// add a label shows "Hello World"
	// create and initialize a label

	cocos2d::Sprite *sprite2 = Sprite::create("bg1.jpg");
	Size imgSize = sprite2->getContentSize();
	sprite2->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height / 2));

	//cocos2d::log("visibleSize(%f, %f), imgSize(%f, %f)", visibleSize.width, visibleSize.height, imgSize.width, imgSize.height);

	this->addChild(sprite2, 0);

	ClosePopup *scene = ClosePopup::create();
	scene->gameScene = this;

	//this->setTouchEnabled(false);
	//_eventDispatcher->setEnabled(false);
	this->addChild(scene, 2001, CLOSE_SCENE_TAG);
	scene->setPosition(-visibleSize.width, -visibleSize.height);

	ResultScene *scene2 = ResultScene::create();
	scene2->gameScene = this;

	//this->setTouchEnabled(false);
	//_eventDispatcher->setEnabled(false);
	this->addChild(scene2, 2002, RESULT_SCENE_TAG);
	scene2->setPosition(-visibleSize.width, -visibleSize.height);

	//scene->setVisible(false);

	loadAllCards();
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/pop.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/shuffle.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/win.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/spade.mp3");

	prepareGame();

	EventListenerTouchOneByOne *listener =  EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch* touch, Event* event){
		// event->getCurrentTarget() returns the *listener's* sceneGraphPriority node.
		GameScene *target = static_cast<GameScene*>(event->getCurrentTarget());

		//Get the position of the current point relative to the button
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);

		//Check the click area
		if (rect.containsPoint(locationInNode))
		{
			target->onTouched(locationInNode);
			return true;
		}
		return false;
	};

	listener->setSwallowTouches(true);

	_eventDispatcher->addEventListenerWithFixedPriority(listener, 10);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	/* start game! */
	schedule(schedule_selector(GameScene::onStep), GAME_SPEED);

	return true;
}

void GameScene::menuBackCallback(Ref* pSender)
{
	CCLog("Call GameScene::menuBackCallback");
	setEnabled(false);

	ClosePopup *scene = (ClosePopup*)getChildByTag(CLOSE_SCENE_TAG);
	scene->gameScene = this;

	//this->setTouchEnabled(false);
	//_eventDispatcher->setEnabled(false);
	scene->setPosition(0, 0);
}

void GameScene::menuResultCallback(Ref* pSender)
{
	CCLog("Call menuResultCallback");
	setEnabled(false);

	ResultScene *scene = (ResultScene*)getChildByTag(RESULT_SCENE_TAG);
	scene->gameScene = this;
	scene->viewResult(ResultScene::ANYWHERE);

	//this->setTouchEnabled(false);
	//_eventDispatcher->setEnabled(false);
	scene->setPosition(0, 0);
}

void GameScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK && enabled) {
		menuBackCallback(NULL);
	}
}

void GameScene::setEnabled(bool enable)
{
	if(enable)
		resume();
	else
		pause();
	enabled = enable;
	Menu *menuBack = (Menu*)getChildByTag(BACK_ITEM_TAG);
	menuBack->setEnabled(enable);

	Menu *menuResult = (Menu*)getChildByTag(RESULT_ITEM_TAG);
	menuResult->setEnabled(enable);

	player[0]->setEnabled(enable);
}

void GameScene::back()
{
	game->setIdle();
	unscheduleAllSelectors();
	if(gameIndex == GAME_FROME_GUEST)
	{
		Scene *pScene = TransitionFade::create(0.5, MenuScene::createScene());
		Director:: getInstance()->replaceScene(pScene);
	}
	else if(gameIndex == GAME_FROME_FACEBOOK)
	{
		Scene *pScene = TransitionFade::create(0.5, UserScene::createScene());
		Director:: getInstance()->replaceScene(pScene);
	}
}

void GameScene::onTouched(Point point)
{
	log("touch began... x = %f, y = %f", point.x, point.y);
	if(enabled)
		player[0]->onTouchedTable();
}

void GameScene::onPlayed(int playerIndex, CardSprite* card)
{
	playedCards[playerIndex] = card;
	game->setCardPlayed(playerIndex, card->cardIndex);
}

void GameScene::removePlayedCards(int winIndex)
{
	CCLog("Call GameScene::removePlayedCards");
	for(int i = 0; i < 4; i++)
	{
		if(!playedCards[i])
			continue;
		Point movePos;
		switch (winIndex)
		{
		case 0:
			movePos = Vec2(0, -visibleSize.height);
			break;
		case 1:
			movePos = Vec2(-visibleSize.width, 0);
			break;
		case 2:
			movePos = Vec2(0, visibleSize.height);
			break;
		case 3:
			movePos = Vec2(visibleSize.width, 0);
			break;
		}
		MoveBy *action_1 = MoveBy::create(CARD_REMOVE_SPEED, movePos);
		playedCards[i]->runAction(action_1);
	}
	playedIndex = 0;
}

void GameScene::loadAllCards()
{
	char filename[16] = "";
	for(int i = 2; i < 15; i++)
	{
		memset(filename, 0, 16);
		sprintf(filename, "cards/h%d.png", i);
		CardSprite *spr = CardSprite::create();
		if(spr)
		{
			spr->setPosition(Vec2(center.x, center.y - 10));
			//spr->setContentSize(spr->getContentSize() * g_screenScale);
			spr->setCard(filename, HEART, i);
			spr->setVisible(false);
			this->addChild(spr, 1000 + i);
			spr->retain();
			cardList.pushBack(spr);
		}
		else
		{
			/*FIXME: add exception */
			CCLog("GameScene loadAllCards exception");
		}
	}

	for(int i = 2; i < 15; i++)
	{
		memset(filename, 0, 16);
		sprintf(filename, "cards/c%d.png", i);
		CardSprite *spr = CardSprite::create();
		if(spr)
		{
			spr->setPosition(Vec2(center.x, center.y - 10));
			spr->setCard(filename, CLOVER, i);
			spr->setVisible(false);
			this->addChild(spr, 1000 + i);
			spr->retain();
			cardList.pushBack(spr);
		}
		else
		{
			/*FIXME: add exception */
			CCLog("GameScene loadAllCards exception");
		}
	}

	for(int i = 2; i < 15; i++)
	{
		memset(filename, 0, 16);
		sprintf(filename, "cards/d%d.png", i);
		CardSprite *spr = CardSprite::create();
		if(spr)
		{
			spr->setPosition(Vec2(center.x, center.y - 10));
			spr->setCard(filename, DIAMOND, i);
			spr->setVisible(false);
			this->addChild(spr, 1000 + i);
			spr->retain();
			cardList.pushBack(spr);
		}
		else
		{
			/*FIXME: add exception */
			CCLog("GameScene loadAllCards exception");
		}
	}

	for(int i = 2; i < 15; i++)
	{
		memset(filename, 0, 16);
		sprintf(filename, "cards/s%d.png", i);
		CardSprite *spr = CardSprite::create();
		if(spr)
		{
			spr->setPosition(Vec2(center.x, center.y - 10));
			spr->setCard(filename, SPADE, i);
			spr->setVisible(false);
			this->addChild(spr, 1000 + i);
			spr->retain();
			cardList.pushBack(spr);
		}
		else
		{
			/*FIXME: add exception */
			CCLog("GameScene loadAllCards exception");
		}
	}

	for (int i = 0; i < 4; i++)
		playedCards[i] = 0;
}

void GameScene::prepareGame()
{
	Sprite *table = (Sprite*)this->getChildByTag(TABLE_BOARD_TAG);
	cocos2d::Size tableSize = table->getContentSize();

	srand(time(0));
	int photo1 = rand() % 12;
	int photo2 = rand() % 12;
	int photo3 = rand() % 12;
	if (photo2 == photo1) photo2 = (photo2 + 1) % 12;
	if (photo3 == photo2) photo3 = (photo3 + 1) % 12;
	photo1++; photo2++; photo3++;

	player[0] = PlayerObject::create();
	player[0]->setPlayerIndex(0);

	player[0]->setAnchorPoint(Vec2(0.5, 0));
	player[0]->setPosition(Vec2(visibleSize.width / 2, 0));
	player[0]->setContentSize(Size(table->getContentSize().width + (visibleSize.width - tableSize.width) / 2, tableSize.height));
	player[0]->setParent(this);

	this->addChild(player[0], 110);

	player[1] = PlayerObject::create();
	player[1]->setAvatarIndex(photo1);
	player[1]->setPlayerIndex(1);

	player[1]->setAnchorPoint(Vec2(0, 0.5));
	player[1]->setPosition(Vec2(0, visibleSize.height / 2));
	player[1]->setParent(this);
	this->addChild(player[1], 102);

	player[2] = PlayerObject::create();
	player[2]->setAvatarIndex(photo2);
	player[2]->setPlayerIndex(2);

	player[2]->setPosition(Vec2(visibleSize.width / 2, visibleSize.height));
	player[2]->setParent(this);
	this->addChild(player[2], 103);

	player[3] = PlayerObject::create();
	player[3]->setAvatarIndex(photo3);
	player[3]->setPlayerIndex(3);

	player[3]->setPosition(Vec2(visibleSize.width, visibleSize.height / 2));
	player[3]->setParent(this);
	this->addChild(player[3], 104);

	if (Facebook::isLogined()) {
		const char *name = Facebook::getUserName();
		const char *avatar = Facebook::getUserImg();
		if (avatar && name) {
			player[0]->setUserInfo(name, avatar);
		}
	}
}

void GameScene::onStep(float time)
{
	game->step();
}

void GameScene::onSetHand(int index, Hand* hand)
{
	player[index]->setHand(&cardList, hand);
}

void GameScene::onWantBid()
{
	CCLog("Call GameScene::onWantBid");
	BidScene *scene = BidScene::create();
	scene->gameScene = this;

	setEnabled(false);
	this->addChild(scene, 2000, 2000);
}

void GameScene::onShowBid(int index, int bid)
{
	CCLog("Call GameScene::onShowBid");
	player[index]->setBid(bid);
	player[index]->setScore(bid, 0);
}

void GameScene::onFinishBid()
{
	CCLog("Call GameScene::onFinishBid");
	player[1]->removeBid();
	player[2]->removeBid();
	player[3]->removeBid();
}

void GameScene::onSpadeBroken()
{
	SimpleAudioEngine:: getInstance()->playEffect("sounds/spade.mp3", false);
}

void GameScene::onSetScore(int playerIndex, int bid, int score)
{
	player[playerIndex]->setScore(bid, score);
}

void GameScene::onRoundFinished()
{
	CCLog("Call GameScene::onRoundFinished");

	GameResult result = game->getGameResult();

	setEnabled(false);

	ResultScene *scene = (ResultScene*)getChildByTag(RESULT_SCENE_TAG);
	scene->gameScene = this;
	scene->viewResult(ResultScene::ROUND);

	//this->setTouchEnabled(false);
	//_eventDispatcher->setEnabled(false);
	scene->setPosition(0, 0);
}

void GameScene::onGameFinished(bool win)
{
	CCLog("Call GameScene::onGameFinished");

	if (win) {
		SimpleAudioEngine:: getInstance()->playEffect("sounds/win.mp3", false);
	}

	if (Facebook::isLogined()) {
		if (win) {
			Facebook::winGame();
		} else {
			Facebook::loseGame();
		}
	}

	GameResult result = game->getGameResult();

	setEnabled(false);

	ResultScene *scene = (ResultScene*)getChildByTag(RESULT_SCENE_TAG);
	scene->gameScene = this;
	scene->viewResult(ResultScene::GAMEOVER);

	//this->setTouchEnabled(false);
	//_eventDispatcher->setEnabled(false);
	scene->setPosition(0, 0);
}

void GameScene::setUserBid(int value, bool dNil)
{
	game->setBid(value, dNil);
}

void GameScene::newRound()
{
	char filename[16] = "";
	for(int i = 0; i < 52; i++)
	{
		CardSprite *spr = cardList.at(i);
		spr->initAll();
		spr->setPosition(Vec2(center.x, center.y - 10));
		spr->setZOrder(1000 + i);
	}

	for (int i = 0; i < 4; i++)
		playedCards[i] = 0;

	player[0]->setScore(0, 0);
	player[1]->setScore(0, 0);
	player[2]->setScore(0, 0);
	player[3]->setScore(0, 0);

	game->newRound();
}

void GameScene::onPlayCard(int playerIndex, int cardIndex)
{
	if (player[playerIndex]->leftCount <= 0) {
		CCLog("No cards!");
		return;
	}
	player[playerIndex]->setSelectedCard(cardIndex);
	player[playerIndex]->onTouchedTable();
}

void GameScene::onTouchEnable(bool enable)
{
	onTouchEnable(SUIT_ALL, enable);
}

void GameScene::onTouchEnable(int suitType, bool enable)
{
	player[0]->setEnableTouchCards(suitType, enable);
}

void GameScene::showUserCards()
{
	player[0]->showCards();
}
