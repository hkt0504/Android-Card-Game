#include "ModeScene.h"
#include "MenuScene.h"
#include "GameScene.h"
#include "UserScene.h"
#include "AppService.h"

USING_NS_CC;

#define MAX_SPADE_SIZE	80

#define COMPLEX_TAG 10
#define MUTE_TAG 11

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

	AppService::modeScene = this;
	this->setKeypadEnabled(true);

	auto spriteBg = Sprite::create("bg2.jpg");
	Size imgSize = spriteBg->getContentSize();

	visibleSize = Director::getInstance()->getWinSizeInPixels();
	origin = Director::getInstance()->getVisibleOrigin();

	spriteBg->setScale(SCR_W / imgSize.width, SCR_H / imgSize.height);
	spriteBg->setPosition(Vec2(origin.x + visibleSize.width/2.0, origin.y + visibleSize.height / 2.0));
	cocos2d::log("visibleSize(%f, %f), imgSize(%f, %f), org(%f, %f)", visibleSize.width, visibleSize.height, imgSize.width, imgSize.height, origin.x, origin.y);
	this->addChild(spriteBg, 0);

	auto spriteLogo = Sprite::create("logo2.png");
	spriteLogo->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 60));
	this->addChild(spriteLogo, 1);

	auto titleBg = Sprite::create("other/mode_title.png");
	titleBg->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - titleBg->getContentSize().height / 2 - 60));
	this->addChild(titleBg, 2);

	auto backItem = MenuItemImage::create("Buttons/btn_back.png", "Buttons/btn_back_d.png", CC_CALLBACK_1(ModeScene::menuBackCallback, this));
	backItem->setPosition(Vec2(origin.x + visibleSize.width - backItem->getContentSize().width / 2 - BACK_BTN_MARGIN,
			origin.y + visibleSize.height - backItem->getContentSize().height / 2 - BACK_BTN_MARGIN));
	auto menuBack = Menu::create(backItem, NULL);
	menuBack->setPosition(Vec2::ZERO);
	this->addChild(menuBack, 10);

	float x = origin.x + (visibleSize.width - 120) / 8.0 + 60;
	float y = origin.y + visibleSize.height / 4.0;

	auto kSingleItem = MenuItemImage::create("Buttons/btn_k_single.png", "Buttons/btn_k_single_d.png", CC_CALLBACK_1(ModeScene::menuKSingleCallback, this));
	kSingleItem->setPosition(x, y);
	auto menuKSingle = Menu::create(kSingleItem, NULL);
	menuKSingle->setPosition(Vec2::ZERO);
	this->addChild(menuKSingle, 10);

	x += (visibleSize.width - 120) / 4.0;
	auto kPartnerItem = MenuItemImage::create("Buttons/btn_k_partner.png", "Buttons/btn_k_partner_d.png", CC_CALLBACK_1(ModeScene::menuKPartnerCallback, this));
	kPartnerItem->setPosition(x, y);
	auto menuKPartner = Menu::create(kPartnerItem, NULL);
	menuKPartner->setPosition(Vec2::ZERO);
	this->addChild(menuKPartner, 10);

	x += (visibleSize.width - 120) / 4.0;
	auto cSingleItem = MenuItemImage::create("Buttons/btn_c_single.png", "Buttons/btn_c_single_d.png", CC_CALLBACK_1(ModeScene::menuCSingleCallback, this));
	cSingleItem->setPosition(x, y);
	auto menuCSingle = Menu::create(cSingleItem, NULL);
	menuCSingle->setPosition(Vec2::ZERO);
	this->addChild(menuCSingle, 10);

	x += (visibleSize.width - 120) / 4.0;
	auto cPartnerItem = MenuItemImage::create("Buttons/btn_c_partner.png", "Buttons/btn_c_partner_d.png", CC_CALLBACK_1(ModeScene::menuCPartnerCallback, this));
	cPartnerItem->setPosition(x, y);
	auto menuCPartner = Menu::create(cPartnerItem, NULL);
	menuCPartner->setPosition(Vec2::ZERO);
	this->addChild(menuCPartner, 10);

	auto soundOnItem = MenuItemImage::create("Buttons/sound_on.png", "Buttons/sound_on.png", NULL, NULL);
	auto soundOffItem = MenuItemImage::create("Buttons/sound_off.png", "Buttons/sound_off.png", NULL, NULL);
	soundItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(ModeScene::menuSoundCallback, this), soundOnItem, soundOffItem, NULL);
	soundItem->setPosition(Vec2(origin.x + soundItem->getContentSize().width / 2 + BACK_BTN_MARGIN,
			origin.y + visibleSize.height - soundItem->getContentSize().height / 2 - BACK_BTN_MARGIN));
	auto menuSound = Menu::create(soundItem, NULL);
	menuSound->setPosition(Vec2::ZERO);
	this->addChild(menuSound, 10, MUTE_TAG);

	onSound(AppService::isMuteVolume());

	return true;
}

void ModeScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		menuBackCallback(NULL);
	}
}

void ModeScene::menuBackCallback(Ref* pSender)
{
	AppService::modeScene = NULL;
	if (gameIndex == GAME_FROM_GUEST) {
		Scene *pScene = TransitionFade::create(0.5, MenuScene::createScene());
		Director:: getInstance()->replaceScene(pScene);
	} else if (gameIndex == GAME_FROM_FACEBOOK) {
		Scene *pScene = TransitionFade::create(0.5, UserScene::createScene());
		Director:: getInstance()->replaceScene(pScene);
	}
}

void ModeScene::menuKSingleCallback(cocos2d::Ref* pSender)
{
	AppService::setComplex(false);
	AppService::setPartner(false);
	Scene *pScene = GameScene::createScene(gameIndex);
	Director::getInstance()->replaceScene(pScene);
}

void ModeScene::menuKPartnerCallback(cocos2d::Ref* pSender)
{
	AppService::setComplex(false);
	AppService::setPartner(true);
	Scene *pScene = GameScene::createScene(gameIndex);
	Director::getInstance()->replaceScene(pScene);
}

void ModeScene::menuCSingleCallback(cocos2d::Ref* pSender)
{
	AppService::setComplex(true);
	AppService::setPartner(false);
	Scene *pScene = GameScene::createScene(gameIndex);
	Director::getInstance()->replaceScene(pScene);
}

void ModeScene::menuCPartnerCallback(cocos2d::Ref* pSender)
{
	AppService::setComplex(true);
	AppService::setPartner(true);
	Scene *pScene = GameScene::createScene(gameIndex);
	Director::getInstance()->replaceScene(pScene);
}

void ModeScene::menuSoundCallback(cocos2d::Ref* pSender)
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

void ModeScene::onSound(bool mute)
{
	soundItem->setSelectedIndex(mute ? 1 : 0);
}
