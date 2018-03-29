#include "MenuScene.h"
#include "common.h"
#include "CardSprite.h"
#include "GameScene.h"
#include "HelpScene.h"
#include "UserScene.h"
#include "Facebook.h"
#include "AppService.h"

USING_NS_CC;
#define MAX_SPADE_SIZE	80

#define MUTE_TAG 10

Scene* MenuScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MenuScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	AppService::menuScene = this;
	this->setKeypadEnabled(true);

	cocos2d::Sprite *sprite2 = Sprite::create("bg2.jpg");
	Size imgSize = sprite2->getContentSize();

	//Director::getInstance()->setContentScaleFactor(g_screenScale);
	visibleSize = Director::getInstance()->getWinSizeInPixels();
	origin = Director::getInstance()->getVisibleOrigin();

	sprite2->setPosition(Vec2(origin.x + visibleSize.width/2.0, origin.y + visibleSize.height / 2.0));
	cocos2d::log("visibleSize(%f, %f), imgSize(%f, %f), org(%f, %f)", visibleSize.width, visibleSize.height, imgSize.width, imgSize.height, origin.x, origin.y);

	this->addChild(sprite2, 0);

	auto playItem = MenuItemImage::create("Buttons/guest_btn.png", "Buttons/guest_btn_d.png",
			CC_CALLBACK_1(MenuScene::menuPlayCallback, this));

	playItem->setPosition(Vec2(origin.x + visibleSize.width / 4.0 + 50, origin.y + visibleSize.height / 4.0));
	auto menuPlay = Menu::create(playItem, NULL);
	menuPlay->setPosition(Vec2::ZERO);
	this->addChild(menuPlay, 7);

	auto helpItem = MenuItemImage::create("Buttons/btn_howtoplay.png", "Buttons/btn_howtoplay_d.png",
			CC_CALLBACK_1(MenuScene::menuHelpCallback, this));
	helpItem->setPosition(Vec2(origin.x + visibleSize.width * 3.0 / 4.0 - 50, origin.y + visibleSize.height / 4.0));

	auto menuHelp = Menu::create(helpItem, NULL);
	menuHelp->setPosition(Vec2::ZERO);
	this->addChild(menuHelp, 8);

	auto facebookItem = MenuItemImage::create("Buttons/facebook_btn.png", "Buttons/facebook_btn_d.png",
			CC_CALLBACK_1(MenuScene::menuFaceBookCallback, this));
	facebookItem->setPosition(Vec2(origin.x + visibleSize.width / 2.0, origin.y + visibleSize.height / 4.0));
	auto menuFB = Menu::create(facebookItem, NULL);
	menuFB->setPosition(Vec2::ZERO);
	this->addChild(menuFB, 9);

	auto quitItem = MenuItemImage::create("Buttons/btn_back.png", "Buttons/btn_back_d.png",
			CC_CALLBACK_1(MenuScene::menuCloseCallback, this));
	quitItem->setPosition(Vec2(origin.x + quitItem->getContentSize().width / 2 + 50, origin.y + visibleSize.height - quitItem->getContentSize().height / 2 - 50));
	auto menuQuit = Menu::create(quitItem, NULL);
	menuQuit->setPosition(Vec2::ZERO);
	this->addChild(menuQuit, 10);

	auto moreItem = MenuItemImage::create("Buttons/more_game_sidestrip.png", "Buttons/more_game_sidestrip.png",
			CC_CALLBACK_1(MenuScene::menuMoreCallback, this));
	moreItem->setPosition(Vec2(origin.x + moreItem->getContentSize().width / 2, origin.y + moreItem->getContentSize().height / 2));
	auto menuMore = Menu::create(moreItem, NULL);
	menuMore->setPosition(Vec2::ZERO);
	this->addChild(menuMore, 10);

	auto rateItem = MenuItemImage::create("Buttons/rate_game_rightstrip.png", "Buttons/rate_game_rightstrip.png",
			CC_CALLBACK_1(MenuScene::menuRateCallback, this));
	rateItem->setPosition(Vec2(origin.x + visibleSize.width - rateItem->getContentSize().width / 2, origin.y + rateItem->getContentSize().height / 2));
	auto menuRate = Menu::create(rateItem, NULL);
	menuRate->setPosition(Vec2::ZERO);
	this->addChild(menuRate, 10);

	auto soundOnItem = MenuItemImage::create("Buttons/sound_on.png", "Buttons/sound_on.png", NULL, NULL);
	auto soundOffItem = MenuItemImage::create("Buttons/sound_off.png", "Buttons/sound_off.png", NULL, NULL);
	soundItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(MenuScene::menuSoundCallback, this), soundOnItem, soundOffItem, NULL);
	soundItem->setPosition(Vec2(origin.x + visibleSize.width - soundItem->getContentSize().width / 2 - 50, origin.y + visibleSize.height - soundItem->getContentSize().height / 2 - 50));
	auto menuSound = Menu::create(soundItem, NULL);
	menuSound->setPosition(Vec2::ZERO);
	this->addChild(menuSound, 10, MUTE_TAG);

	spadeIndex = 0;
	particleAnimation();

	onSound(AppService::isMuteVolume());

	return true;
}

void MenuScene::particleAnimation()
{
	/* load heart sprite */
	CallFuncN *action_1 = CallFuncN::create(CC_CALLBACK_0(MenuScene::createParticleAction, this));
	DelayTime *action_2 = DelayTime::create(0.05f);

	Sequence *action_3 = Sequence::create(action_1, action_2, NULL);
	RepeatForever *action_4 = RepeatForever::create(action_3);
	runAction(action_4);
}

void MenuScene::endParticleAction()
{
	if(spadeIndex > 0)
		spadeIndex--;
}

void MenuScene::createParticleAction()
{
	Sprite *spr = Sprite::create("effects/particle_spade.png");
	float width = spr->getContentSize().width;
	float height = spr->getContentSize().height;

	if (spadeIndex < MAX_SPADE_SIZE)
	{
		int rand1 = rand();
		Vec2 startPos;
		startPos.x = origin.x + 20 + (rand1 % (int)(visibleSize.width - 40));
		startPos.y = origin.y + visibleSize.height + height;
		float startDelayTime = 0.1 * (rand() % 10);

		float totalDistance = 0.0;
		totalDistance = 150 + (rand() % (int)(visibleSize.height));

		float scale = 1.0 / frand(1, 3);
		spr->setScale(scale);

		//CCLog("Call createHeartAction (%d), (%f,%f), %f, %f, %f", spadeIndex, startPos.x, startPos.y, startDelayTime, totalDistance, scale);
		this->addChild(spr);

		spr->setPosition(startPos);
		DelayTime *action_1 = DelayTime::create(startDelayTime);
		float yDelta = 0;
		if(startPos.x < origin.x + visibleSize.width / 2)
			yDelta  = 1 - rand() % 30;
		else
			yDelta  = rand() % 30;

		MoveBy *action_2 = MoveBy::create(1.0f, Point(yDelta / 5, - totalDistance / 5));
		FadeOut *action_3 = FadeOut::create(4.0);
		MoveBy *action_4 = MoveBy::create(4.0, Point((yDelta / 5) * 4, - (totalDistance / 5) * 4));

		Spawn *action_5 = Spawn::create(action_3, action_4, NULL);

		CallFunc *action_6 = CallFunc::create(CC_CALLBACK_0(MenuScene::endParticleAction, this));
		Sequence *action_7 = Sequence::create(action_1, action_2, action_5, action_6, NULL);

		spr->runAction(action_7);
		spadeIndex++;
	}
	else
		return;
}

void MenuScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
	return;
#endif

	AppService::menuScene = NULL;

	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void MenuScene::menuPlayCallback(Ref* pSender)
{
	Scene *pScene = TransitionFade::create(0.5, GameScene::createScene(GAME_FROME_GUEST));
	Director:: getInstance()->replaceScene(pScene);
}

void MenuScene::menuHelpCallback(Ref* pSender)
{
	Scene *pScene = TransitionFade::create(0.5, HelpScene::createScene());
	Director:: getInstance()->replaceScene(pScene);
}

void MenuScene::menuFaceBookCallback(cocos2d::Ref* pSender)
{
#ifdef FACEBOOK
	Facebook::menuScene = this;
	Facebook::login();
#else
	Scene *pScene = TransitionFade::create(0.5, UserScene::createScene());
	Director:: getInstance()->replaceScene(pScene);
#endif //FACEBOOK
}

void MenuScene::menuMoreCallback(cocos2d::Ref* pSender)
{
	AppService::openMoreGame();
}

void MenuScene::menuRateCallback(cocos2d::Ref* pSender)
{
	AppService::openRateGame();
}

void MenuScene::menuSoundCallback(cocos2d::Ref* pSender)
{
	auto toggleItem = dynamic_cast<MenuItemToggle*>(pSender);
	int selected = toggleItem->getSelectedIndex();
	if (selected == 0 ) {
		// Turn On Sound
		AppService::unmuteVolume();
	} else if (selected == 1) {
		// Turn Off Sound
		AppService::muteVolume();
	}
}

void MenuScene::onSound(bool mute)
{
	soundItem->setSelectedIndex(mute ? 1 : 0);
}

void MenuScene::onFaceBook(int action)
{
	switch (action) {
	case FB_LOGIN:
		Scene *pScene = TransitionFade::create(0.5, UserScene::createScene());
		Director:: getInstance()->replaceScene(pScene);
		break;
	}
}

void MenuScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	AppService::menuScene = NULL;

	if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		menuCloseCallback(NULL);
	}
}
