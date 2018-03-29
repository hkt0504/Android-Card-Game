#include "common.h"
#include "GameScene.h"
#include "MenuScene.h"
#include "CardSprite.h"
#include "PlayerObject.h"
#include "CardDeck.h"
#include "PlayerObject.h"
#include "BidScene.h"
#include "TarneebScene.h"
#include "ClosePopup.h"
#include "Controller.h"
#include "Hand.h"
#include "ResultScene.h"
#include "UserScene.h"
#include "Facebook.h"

USING_NS_CC;

#define TABLE_BOARD_TAG		2
#define CLOSE_SCENE_TAG		20
#define RESULT_SCENE_TAG	21
#define SPR_TARNEEB_TAG		30

GameScene::GameScene()
{
	controller = Controller::create();
	controller->retain();

	playedIndex = 0;
	gameIndex = 0;
	enabled = true;
}

GameScene::~GameScene()
{
	controller->release();
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
	if (!Layer::init()) {
		return false;
	}

	CCLog("Call GameScene::init s");

	this->setKeypadEnabled(true);

	enabled = true;
	controller->setGameScene(this);
	playedIndex = 0;

	visibleSize = Director::getInstance()->getWinSizeInPixels();
	origin = Director::getInstance()->getVisibleOrigin();
	center = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);

	auto spriteBg = Sprite::create("bg1.jpg");
	Size imgSize = spriteBg->getContentSize();
	spriteBg->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height / 2);
	this->addChild(spriteBg, 0);

	auto backItem = MenuItemImage::create("Buttons/btn_back.png", "Buttons/btn_back.png",
			CC_CALLBACK_1(GameScene::menuBackCallback, this));
	backItem->setPosition(origin.x + visibleSize.width - backItem->getContentSize().width / 2 - BACK_BTN_MARGIN,
			origin.y + visibleSize.height - backItem->getContentSize().height / 2 - BACK_BTN_MARGIN);
	auto menuBack = Menu::create(backItem, NULL);
	menuBack->setPosition(Vec2::ZERO);
	this->addChild(menuBack, 1, BACK_ITEM_TAG);

	auto resultItem = MenuItemImage::create("Buttons/right_page_icon.png", "Buttons/right_page_icon.png", CC_CALLBACK_1(GameScene::menuResultCallback, this));
	resultItem->setPosition(origin.x + resultItem->getContentSize().width / 2 + BACK_BTN_MARGIN,
			origin.y + visibleSize.height - resultItem->getContentSize().height / 2 - BACK_BTN_MARGIN);
	auto menuResult = Menu::create(resultItem, NULL);
	menuResult->setPosition(Vec2::ZERO);
	this->addChild(menuResult, 2, RESULT_ITEM_TAG);

	auto spriteTarneeb = Sprite::create("bid/btn_bid_spade.png");
	spriteTarneeb->setPosition(origin.x + visibleSize.width - 280, origin.y + visibleSize.height - spriteTarneeb->getContentSize().height / 2 - BACK_BTN_MARGIN);
	this->addChild(spriteTarneeb, 2,  SPR_TARNEEB_TAG);
	spriteTarneeb->setVisible(false);

	auto spr = Sprite::create("play_table.png");
	spr->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height / 2 - 10);
	this->addChild(spr, 3, TABLE_BOARD_TAG);
	spr->setVisible(false);

	ClosePopup *scene = ClosePopup::create();
	scene->gameScene = this;
	this->addChild(scene, 2001, CLOSE_SCENE_TAG);
	scene->setPosition(-visibleSize.width, -visibleSize.height);

	ResultScene *scene2 = ResultScene::create();
	scene2->gameScene = this;
	this->addChild(scene2, 2002, RESULT_SCENE_TAG);
	scene2->setPosition(-visibleSize.width, -visibleSize.height);

	loadAllCards();

	prepareGame();

	EventListenerTouchOneByOne *listener =  EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch* touch, Event* event) {
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
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	/* start game! */
	schedule(schedule_selector(GameScene::onStep), GAME_SPEED);

	CCLog("Call GameScene::init e");

	return true;
}

void GameScene::menuBackCallback(Ref* pSender)
{
	CCLog("Call GameScene::menuBackCallback");

	setEnabled(false);
	ClosePopup *scene = (ClosePopup*)getChildByTag(CLOSE_SCENE_TAG);
	scene->gameScene = this;
	scene->setPosition(0, 0);
}

void GameScene::menuResultCallback(Ref* pSender)
{
	CCLog("Call menuResultCallback");

	setEnabled(false);
	ResultScene *scene = (ResultScene*)getChildByTag(RESULT_SCENE_TAG);
	scene->gameScene = this;
	scene->viewResult(ResultScene::ANYWHERE);
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
	controller->setIdle();
	unscheduleAllSelectors();
	if (gameIndex == GAME_FROM_GUEST) {
		Scene *pScene = TransitionFade::create(0.5, MenuScene::createScene());
		Director:: getInstance()->replaceScene(pScene);
	} else if (gameIndex == GAME_FROM_FACEBOOK) {
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
	controller->setCardPlayed(playerIndex, card->cardIndex);
}

void GameScene::removePlayedCards(int winIndex)
{
	CCLog("Call GameScene::removePlayedCards");
	for (int i = 0; i < 4; i++) {
		if(!playedCards[i])
			continue;
		Point movePos;
		switch (winIndex)
		{
		case 0:
			movePos = Vec2(0, -visibleSize.height);
			break;
		case 1:
			movePos = Vec2(visibleSize.width, 0);
			break;
		case 2:
			movePos = Vec2(0, visibleSize.height);
			break;
		case 3:
			movePos = Vec2(-visibleSize.width, 0);
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
			spr->setPosition(center.x, center.y - 10);
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
			spr->setPosition(center.x, center.y - 10);
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
			spr->setPosition(center.x, center.y - 10);
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
			spr->setPosition(center.x, center.y - 10);
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

	CCLog("GameScene loadAllCards e");
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
	if (photo3 == photo1) photo3 = (photo3 + 1) % 12;
	photo1++;
	photo2++;
	photo3++;

	player[0] = PlayerObject::create();
	player[0]->setPlayerIndex(0);
	player[0]->setAnchorPoint(Vec2(0.5, 0));
	player[0]->setPosition(visibleSize.width / 2, 0);
	player[0]->setContentSize(Size(table->getContentSize().width + (visibleSize.width - tableSize.width) / 2, tableSize.height));
	player[0]->setParent(this);
	this->addChild(player[0], 110);

	player[1] = PlayerObject::create();
	player[1]->setAvatarIndex(photo1);
	player[1]->setPlayerIndex(1);
	player[1]->setAnchorPoint(Vec2(0, 0.5));
	player[1]->setPosition(visibleSize.width, visibleSize.height / 2);
	player[1]->setParent(this);
	this->addChild(player[1], 102);

	player[2] = PlayerObject::create();
	player[2]->setAvatarIndex(photo2);
	player[2]->setPlayerIndex(2);
	player[2]->setPosition(visibleSize.width / 2, visibleSize.height);
	player[2]->setParent(this);
	this->addChild(player[2], 103);

	player[3] = PlayerObject::create();
	player[3]->setAvatarIndex(photo3);
	player[3]->setPlayerIndex(3);
	player[3]->setPosition(0, visibleSize.height / 2);
	player[3]->setParent(this);
	this->addChild(player[3], 104);

	if (Facebook::isLogined()) {
		const char *name = Facebook::getUserName();
		const char *avatar = Facebook::getUserImg();
		if (avatar && name) {
			player[0]->setUserInfo(name, avatar);
		}
	}

	CCLog("GameScene prepareGame e");
}

void GameScene::onStep(float time)
{
	controller->step();
}

void GameScene::onSetHand(int index, Hand* hand)
{
	player[index]->setHand(&cardList, hand);
}

void GameScene::onWantBid(int min)
{
	CCLog("Call GameScene::onWantBid");
	BidScene *scene = BidScene::create();
	scene->gameScene = this;
	scene->setMin(min);

	setEnabled(false);
	this->addChild(scene, 2000, 2000);
}

void GameScene::onShowBid(int playerIndex, int bid)
{
	CCLog("Call GameScene::onShowBid");
	player[playerIndex]->setBid(bid);
	player[playerIndex]->setScore(bid, 0);
}

void GameScene::onFinishBid()
{
	CCLog("Call GameScene::onFinishBid");
	player[1]->removeBid();
	player[2]->removeBid();
	player[3]->removeBid();
}

void GameScene::onWantTarneeb()
{
	CCLog("Call GameScene::onWaitTarneeb");
	TarneebScene *scene = TarneebScene::create();
	scene->gameScene = this;

	setEnabled(false);
	this->addChild(scene, 2000, 2000);
}

void GameScene::onShowTarneeb(Suit suit)
{
	Sprite *sprite = (Sprite *)this->getChildByTag(SPR_TARNEEB_TAG);

	switch (suit) {
	case SPADE:
		sprite->setTexture("bid/btn_bid_spade.png");
		sprite->setVisible(true);
		break;
	case CLOVER:
		sprite->setTexture("bid/btn_bid_clover.png");
		sprite->setVisible(true);
		break;
	case HEART:
		sprite->setTexture("bid/btn_bid_heart.png");
		sprite->setVisible(true);
		break;
	case DIAMOND:
		sprite->setTexture("bid/btn_bid_diamond.png");
		sprite->setVisible(true);
		break;
	default:
		sprite->setVisible(false);
		break;
	}
}

void GameScene::onSetScore(int playerIndex, int bid, int score)
{
	player[playerIndex]->setScore(bid, score);
}

void GameScene::onRoundFinished()
{
	CCLog("Call GameScene::onRoundFinished");

	setEnabled(false);
	onShowTarneeb(BLANK);

	ResultScene *scene = (ResultScene*)getChildByTag(RESULT_SCENE_TAG);
	scene->gameScene = this;
	scene->viewResult(ResultScene::ROUND);

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

	setEnabled(false);

	ResultScene *scene = (ResultScene*)getChildByTag(RESULT_SCENE_TAG);
	scene->gameScene = this;
	scene->viewResult(ResultScene::GAMEOVER);

	scene->setPosition(0, 0);
}

void GameScene::newRound()
{
	for (int i = 0; i < 52; i++)
	{
		CardSprite *spr = cardList.at(i);
		spr->initAll();
		spr->removeFromParent();
		spr->setPosition(center.x, center.y - 10);
		this->addChild(spr, 1000 + i);
	}

	for (int i = 0; i < PLAYER_NUM; i++) {
		playedCards[i] = 0;
		player[i]->clear();
	}

	controller->newRound();
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

