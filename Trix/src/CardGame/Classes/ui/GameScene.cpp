#include "common.h"
#include "GameScene.h"
#include "ModeScene.h"
#include "CardSprite.h"
#include "PlayerObject.h"
#include "CardDeck.h"
#include "PlayerObject.h"
#include "BidScene.h"
#include "EatScene.h"
#include "ClosePopup.h"
#include "ResultPopup.h"
#include "DuplicateScene.h"
#include "Controller.h"
#include "Hand.h"
#include "ResultScene.h"
#include "Facebook.h"
#include "AppService.h"

USING_NS_CC;

#define BACK_MARGIN			20

#define TABLE_BOARD_TAG		2
#define CLOSE_SCENE_TAG		20
#define RESULT_SCENE_TAG	21
#define LABEL_BID_TAG		23
#define SPR_BID_TAG			24

GameScene::GameScene()
{
	controller = Controller::create();
	controller->retain();
	controller->setPlayMode(AppService::isPartner(), AppService::isComplex());

	playedIndex = 0;
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

	auto spriteBg = Sprite::create("bg3.jpg");
	Size imgSize = spriteBg->getContentSize();
	spriteBg->setScale(SCR_W / imgSize.width, SCR_H / imgSize.height);
	spriteBg->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height / 2));
	this->addChild(spriteBg, 0);

	auto backItem = MenuItemImage::create("Buttons/btn_back.png", "Buttons/btn_back_d.png",
			CC_CALLBACK_1(GameScene::menuBackCallback, this));
	backItem->setPosition(Vec2(origin.x + visibleSize.width - backItem->getContentSize().width / 2 - BACK_MARGIN,
			origin.y + visibleSize.height - backItem->getContentSize().height / 2 - BACK_MARGIN));
	auto menuBack = Menu::create(backItem, NULL);
	menuBack->setPosition(Vec2::ZERO);
	this->addChild(menuBack, 1, BACK_ITEM_TAG);

	auto labelBid = LabelTTF::create("", "Felt", 26);
	labelBid->setColor(ccc3(214, 176, 102));
	labelBid->setPosition(Vec2(origin.x + visibleSize.width - 280, origin.y + visibleSize.height - labelBid->getContentSize().height / 2 - BACK_MARGIN));
	this->addChild(labelBid, 2, LABEL_BID_TAG);

	auto spriteBid = Sprite::create("bid/title_lotosh.png");
	spriteBid->setPosition(Vec2(labelBid->getPosition().x, labelBid->getPosition().y - 50));
	this->addChild(spriteBid, 2,  SPR_BID_TAG);
	spriteBid->setVisible(false);

	auto resultItem = MenuItemImage::create("Buttons/right_page_icon.png", "Buttons/right_page_icon.png", CC_CALLBACK_1(GameScene::menuResultCallback, this));
	resultItem->setPosition(Vec2(origin.x + resultItem->getContentSize().width / 2 + BACK_MARGIN,
			origin.y + visibleSize.height - resultItem->getContentSize().height / 2 - BACK_MARGIN));
	auto menuResult = Menu::create(resultItem, NULL);
	menuResult->setPosition(Vec2::ZERO);
	this->addChild(menuResult, 2, RESULT_ITEM_TAG);

	MenuItemImage *eatItem = MenuItemImage::create("Buttons/btn_eatpage.png", "Buttons/btn_eatpage.png", CC_CALLBACK_1(GameScene::menuEatCallback, this));
	eatItem->setPosition(Vec2(origin.x + eatItem->getContentSize().width / 2 + BACK_MARGIN,
			origin.y + eatItem->getContentSize().height / 2 + 10));
	auto menuEat = Menu::create(eatItem, NULL);
	menuEat->setPosition(Vec2::ZERO);
	this->addChild(menuEat, 2, RESULT_ITEM_TAG);

	auto spr = Sprite::create("play_table.png");
	spr->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height / 2 - 10));
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

void GameScene::menuEatCallback(cocos2d::Ref* pSender)
{
	CCLog("Call menuEatCallback");
	EatScene *scene = EatScene::create();
	scene->gameScene = this;
	scene->showCards();

	setEnabled(false);
	this->addChild(scene, 2000, 2000);
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
	_eventDispatcher->removeAllEventListeners();

	Scene *pScene = TransitionFade::create(0.5, ModeScene::createScene(gameIndex));
	Director:: getInstance()->replaceScene(pScene);
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
	player[0]->setPosition(Vec2(visibleSize.width / 2, 0));
	player[0]->setContentSize(Size(table->getContentSize().width + (visibleSize.width - tableSize.width) / 2, tableSize.height));
	player[0]->setParent(this);
	this->addChild(player[0], 110);

	player[1] = PlayerObject::create();
	player[1]->setAvatarIndex(photo1);
	player[1]->setPlayerIndex(1);
	player[1]->setAnchorPoint(Vec2(0, 0.5));
	player[1]->setPosition(Vec2(visibleSize.width, visibleSize.height / 2));
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
	player[3]->setPosition(Vec2(0, visibleSize.height / 2));
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

void GameScene::onWantBid(int enabledBid)
{
	CCLog("Call GameScene::onWantBid");
	BidScene *scene = BidScene::create();
	scene->gameScene = this;
	scene->setEnabledBids(enabledBid);

	setEnabled(false);
	this->addChild(scene, 2000, 2000);
}

void GameScene::onShowBid(int playerIndex, BidType bid)
{
	CCLog("Call GameScene::onShowBid");

	char name[200] = {0, };
	sprintf(name, "KINGDOM: %s", player[playerIndex]->getPlayerName().getCString());
	LabelTTF *labelBid = (LabelTTF *) this->getChildByTag(LABEL_BID_TAG);
	labelBid->setString(name);

	Sprite *spriteBid = (Sprite *)this->getChildByTag(SPR_BID_TAG);

	switch (bid) {
	case BT_KING:
		spriteBid->setTexture("bid/title_king.png");
		spriteBid->setVisible(true);
		break;
	case BT_QUEEN:
		spriteBid->setTexture("bid/title_queen.png");
		spriteBid->setVisible(true);
		break;
	case BT_DIAMOND:
		spriteBid->setTexture("bid/title_diamond.png");
		spriteBid->setVisible(true);
		break;
	case BT_LOTOSH:
		spriteBid->setTexture("bid/title_lotosh.png");
		spriteBid->setVisible(true);
		break;
	case BT_TRIX:
		spriteBid->setTexture("bid/title_trix.png");
		spriteBid->setVisible(true);
		break;
	case BT_COMPLEX:
		spriteBid->setTexture("bid/title_complex.png");
		spriteBid->setVisible(true);
		break;
	default:
		spriteBid->setVisible(false);
		break;
	}
}

void GameScene::onWantDuplicate(int duplicates)
{
	CCLog("Call GameScene::onWantBid");
	DuplicateScene *scene = DuplicateScene::create();
	scene->gameScene = this;
	scene->setDuplicate(duplicates);

	setEnabled(false);
	this->addChild(scene, 2000, 2000);
}

void GameScene::onShowDuplicate(int playerIndex, int duplicates)
{
	player[playerIndex]->showDuplicate(duplicates);
}

void GameScene::onPlayTurn(int playerIndex, bool pass)
{
	for (int i = 0; i < 4; i++) {
		if (i == playerIndex) {
			player[i]->showCurrent(true);
			player[i]->showPass(pass);
		} else {
			player[i]->showCurrent(false);
			player[i]->showPass(false);
		}
	}
}

void GameScene::onShowScore(int playerIndex, int score, int move)
{
	player[playerIndex]->setMove(move);
	player[playerIndex]->setScore(score);
}

void GameScene::onRoundFinished()
{
	CCLog("Call GameScene::onRoundFinished");

	setEnabled(false);
	onShowBid(0, BT_NONE);

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

	int count = -1;
	if (Facebook::isLogined()) {
		if (win) {
			count = Facebook::getWin() + 1;
			Facebook::winGame();
		} else {
			count = Facebook::getLose() + 1;
			Facebook::loseGame();
		}
	}

	setEnabled(false);

	ResultPopup *popup = ResultPopup::create();
	popup->gameScene = this;
	popup->viewResult(win, count);
	this->addChild(popup, 2000, 2000);
}

void GameScene::newRound()
{
	for (int i = 0; i < 52; i++)
	{
		CardSprite *spr = cardList.at(i);
		spr->initAll();
		spr->removeFromParent();
		spr->setPosition(Vec2(center.x, center.y - 10));
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

void GameScene::onTouchEnable(Card enableds[], int count)
{
	player[0]->setEnableTouchCards(enableds, count);
}

bool GameScene::isTrixBid()
{
	return controller->getBid() == BT_TRIX;
}
