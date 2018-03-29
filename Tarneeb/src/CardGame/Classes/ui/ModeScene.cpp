#include "ModeScene.h"
#include "MenuScene.h"
#include "GameScene.h"
#include "UserScene.h"
#include "AppService.h"

USING_NS_CC;


Scene* ModeScene::createScene(int index)
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = ModeScene::create();
	layer->gameIndex = index;

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool ModeScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}

	this->setKeypadEnabled(true);

	visibleSize = Director::getInstance()->getWinSizeInPixels();
	origin = Director::getInstance()->getVisibleOrigin();

	auto spriteBg = Sprite::create("bg3.jpg");
	Size imgSize = spriteBg->getContentSize();
	spriteBg->setPosition(Vec2(origin.x + visibleSize.width/2.0, origin.y + visibleSize.height / 2.0));
	this->addChild(spriteBg, 0);

	float x = origin.x + (visibleSize.width - 120) / 4.0 + 60;
	float y = origin.y + visibleSize.height / 4.0;

	auto notarneebItem = MenuItemImage::create("Buttons/btn_notarneeb.png", "Buttons/btn_notarneeb.png", CC_CALLBACK_1(ModeScene::menuNotarneebCallback, this));
	notarneebItem->setPosition(x, y);
	auto menuNotarneeb = Menu::create(notarneebItem, NULL);
	menuNotarneeb->setPosition(Vec2::ZERO);
	this->addChild(menuNotarneeb, 10);

	x += (visibleSize.width - 120) / 2.0;
	auto tarneebItem = MenuItemImage::create("Buttons/btn_tarneeb.png", "Buttons/btn_tarneeb.png", CC_CALLBACK_1(ModeScene::menuTarneebCallback, this));
	tarneebItem->setPosition(x, y);
	auto menuTarneeb = Menu::create(tarneebItem, NULL);
	menuTarneeb->setPosition(Vec2::ZERO);
	this->addChild(menuTarneeb, 10);

	return true;
}

void ModeScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		if (gameIndex == GAME_FROM_GUEST) {
			Scene *pScene = TransitionFade::create(0.5, MenuScene::createScene());
			Director:: getInstance()->replaceScene(pScene);
		} else if (gameIndex == GAME_FROM_FACEBOOK) {
			Scene *pScene = TransitionFade::create(0.5, UserScene::createScene());
			Director:: getInstance()->replaceScene(pScene);
		}
	}
}

void ModeScene::menuNotarneebCallback(cocos2d::Ref* pSender)
{
	AppService::setNoTarneeb(true);
	Scene *pScene = GameScene::createScene(gameIndex);
	Director::getInstance()->replaceScene(pScene);
}

void ModeScene::menuTarneebCallback(cocos2d::Ref* pSender)
{
	AppService::setNoTarneeb(false);
	Scene *pScene = GameScene::createScene(gameIndex);
	Director::getInstance()->replaceScene(pScene);
}
