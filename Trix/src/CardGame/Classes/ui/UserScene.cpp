#include "UserScene.h"
#include "MenuScene.h"
#include "ModeScene.h"
#include "LeaderboardScene.h"
#include "AchievementScene.h"
#include "BuyScene.h"
#include "ui/UILoadingBar.h"
#include "Facebook.h"
#include "Google.h"

USING_NS_CC;

#define LABEL_USER_NAME_TAG	10
#define LABEL_LEVEL_TAG		11
#define LOAD_USER_TAG		12
#define LABEL_WIN_TAG		13
#define LABEL_LOSE_TAG		14
#define IMG_USER_AVATAR_TAG 15

Scene* UserScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = UserScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool UserScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}

	this->setKeypadEnabled(true);

	visibleSize = Director::getInstance()->getWinSizeInPixels();
	origin = Director::getInstance()->getVisibleOrigin();
	Vec2 center = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);

	auto spriteBg = Sprite::create("bg1.jpg");
	Size imgSize = spriteBg->getContentSize();
	spriteBg->setScale(SCR_W / imgSize.width, SCR_H / imgSize.height);
	spriteBg->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height / 2));
	this->addChild(spriteBg, 0);

	Sprite *sprGroup1 = Sprite::create("banners/userpage_btn.png");

	float deltaX = origin.x + sprGroup1->getContentSize().width / 2 + 110;
	float deltaY = origin.y + visibleSize.height - sprGroup1->getContentSize().height / 2 - 130;

	sprGroup1->setPosition(Vec2(deltaX, deltaY));
	this->addChild(sprGroup1, 1);

	auto userImg = Sprite::create("avatars/sample_user.png");
	userImg->setPosition(Vec2(deltaX - sprGroup1->getContentSize().width / 4 - userImg->getContentSize().width / 2, deltaY));
	addChild(userImg, 2, IMG_USER_AVATAR_TAG);

	auto labelUser = LabelTTF::create("UserName", "Felt", 18);
	labelUser->setPosition(Vec2(deltaX + sprGroup1->getContentSize().width / 8, deltaY + sprGroup1->getContentSize().height / 4));
	addChild(labelUser, 2, LABEL_USER_NAME_TAG);

	auto labelLevel = LabelTTF::create("Level 0", "Felt", 18);
	labelLevel->setPosition(Vec2(deltaX - 28, deltaY - sprGroup1->getContentSize().height / 4 + 8));
	addChild(labelLevel, 2, LABEL_LEVEL_TAG);

	ui::LoadingBar *levelBar = ui::LoadingBar::create("banners/green_strip.png", 0);
	levelBar->setPosition(ccp(deltaX + 66, deltaY - sprGroup1->getContentSize().height / 4 + 6));
	addChild(levelBar, 2, LOAD_USER_TAG);
	levelBar->setPercent(10);

	deltaX += sprGroup1->getContentSize().width / 2 + 80;

	auto starItem = MenuItemImage::create("Buttons/star_btn.png", "Buttons/star_btn.png",
			CC_CALLBACK_1(UserScene::menuBackCallback, this));
	deltaX += starItem->getContentSize().width / 2;
	starItem->setPosition(Vec2(deltaX, deltaY));
	auto menuStar = Menu::create(starItem, NULL);
	menuStar->setPosition(Vec2::ZERO);
	this->addChild(menuStar, 1);

	deltaX += starItem->getContentSize().width / 2 + 80;

	auto sprGroup3 = Sprite::create("banners/userpage_btn2.png");
	deltaX += 15 + sprGroup3->getContentSize().width / 2;
	sprGroup3->setPosition(Vec2(deltaX,	deltaY));
	this->addChild(sprGroup3, 1);

	auto labelWin = LabelTTF::create("Win : 0", "Felt", 18);
	labelWin->setPosition(Vec2(deltaX - labelWin->getContentSize().width / 2 - 10,
			deltaY - sprGroup3->getContentSize().height / 4 + 4));
	labelWin->setColor(Color3B::WHITE);
	addChild(labelWin, 2, LABEL_WIN_TAG);

	auto labelLose = LabelTTF::create("Lose : 0", "Felt", 18);
	labelLose->setPosition(Vec2(deltaX + labelLose->getContentSize().width + 24,
			deltaY - sprGroup3->getContentSize().height / 4 + 4));
	labelLose->setColor(Color3B::WHITE);
	addChild(labelLose, 2, LABEL_LOSE_TAG);

	deltaX += sprGroup3->getContentSize().width / 2 + 80;

	auto settingItem = MenuItemImage::create("Buttons/setting_btn.png", "Buttons/setting_btn.png",
			CC_CALLBACK_1(UserScene::menuBackCallback, this));
	deltaX += settingItem->getContentSize().width / 2;
	settingItem->setPosition(Vec2(deltaX, deltaY));
	auto menuSetting = Menu::create(settingItem, NULL);
	menuSetting->setPosition(Vec2::ZERO);
	this->addChild(menuSetting, 1);

	/** middle items */

	auto leaderboardItem = MenuItemImage::create("banners/leaderboard_btn.png", "banners/leaderboard_btn.png",
			CC_CALLBACK_1(UserScene::menuLeaderboard, this));
	deltaX = center.x - leaderboardItem->getContentSize().width / 2 - 30;
	deltaY = center.y + leaderboardItem->getContentSize().height / 2;
	leaderboardItem->setPosition(deltaX, deltaY);
	auto menuLB = Menu::create(leaderboardItem, NULL);
	menuLB->setPosition(Vec2::ZERO);
	this->addChild(menuLB, 1);

	auto achivItem = MenuItemImage::create("banners/achiv_btn.png", "banners/achiv_btn.png",
			CC_CALLBACK_1(UserScene::menuAchievement, this));
	deltaX = center.x + achivItem->getContentSize().width / 2 + 30;
	deltaY = center.y + achivItem->getContentSize().height / 2;
	achivItem->setPosition(deltaX, deltaY);
	auto menuAchiv = Menu::create(achivItem, NULL);
	menuAchiv->setPosition(Vec2::ZERO);
	this->addChild(menuAchiv, 1);

	/* Bottom Buttons */

	auto playItem = MenuItemImage::create("Buttons/play_now.png", "Buttons/play_now_d.png",
			CC_CALLBACK_1(UserScene::menuPlayNow, this));
	deltaX = origin.x + visibleSize.width / 2 - 64 * 1.5 - playItem->getContentSize().width * 1.5;
	deltaY = origin.y + 30 + playItem->getContentSize().height;
	playItem->setPosition(Vec2(deltaX, deltaY));
	auto menuPlay = Menu::create(playItem, NULL);
	menuPlay->setPosition(Vec2::ZERO);
	this->addChild(menuPlay, 1);

	deltaX += playItem->getContentSize().width + 64;

	auto shareItem = MenuItemImage::create("Buttons/share_game.png", "Buttons/share_game_d.png",
			CC_CALLBACK_1(UserScene::menuShareGame, this));
	shareItem->setPosition(Vec2(deltaX, deltaY));
	auto menuShare = Menu::create(shareItem, NULL);
	menuShare->setPosition(Vec2::ZERO);
	this->addChild(menuShare, 1);

	deltaX += shareItem->getContentSize().width + 64;

	auto inviteItem = MenuItemImage::create("Buttons/invite_friends.png", "Buttons/invite_friends_d.png",
			CC_CALLBACK_1(UserScene::menuInviteFriend, this));
	inviteItem->setPosition(Vec2(deltaX, deltaY));
	auto menuInvite = Menu::create(inviteItem, NULL);
	menuInvite->setPosition(Vec2::ZERO);
	this->addChild(menuInvite, 1);

	deltaX += inviteItem->getContentSize().width + 64;

	auto buyItem = MenuItemImage::create("Buttons/buy_features.png", "Buttons/buy_features_d.png",
			CC_CALLBACK_1(UserScene::menuBuyFeatures, this));
	buyItem->setPosition(Vec2(deltaX, deltaY));
	auto menuBuy = Menu::create(buyItem, NULL);
	menuBuy->setPosition(Vec2::ZERO);
	this->addChild(menuBuy, 1);

#ifdef FACEBOOK
	const char *name = Facebook::getUserName();
	if (name) {
		setUserName(name);
	}

	setUserWinCount(Facebook::getWin());
	setUserLoseCount(Facebook::getLose());

	const char *imgURL = Facebook::getUserImg();
	if (imgURL) {
		setUserAvatar(imgURL);
	}
#endif
	return true;
}

void UserScene::setUserAvatar(const char *imgURL)
{
	auto spr = (Sprite*)getChildByTag(IMG_USER_AVATAR_TAG);
	if (spr) {
		Size size = spr->getContentSize();
		spr->setTextureRect(Rect(0, 0, size.width, size.height));
		spr->setTexture(imgURL);
	}
}

void UserScene::setUserName(const char *name)
{
	auto labelName = (LabelTTF*)getChildByTag(LABEL_USER_NAME_TAG);
	if (labelName) {
		labelName->setString(name);
	}
}

void UserScene::setUserLevel(int level)
{
	auto label = (LabelTTF*)getChildByTag(LABEL_LEVEL_TAG);
	if (label) {
		char levelStr[10];
		sprintf(levelStr, "Level %d", level);
		label->setString(levelStr);
	}
}
void UserScene::setUserWinCount(int count)
{
	LabelTTF *label = (LabelTTF*)getChildByTag(LABEL_WIN_TAG);
	if(label)
	{
		char levelStr[16];
		sprintf(levelStr, "Win : %d", count);
		label->setString(levelStr);
	}
}

void UserScene::setUserLoseCount(int count)
{
	auto label = (LabelTTF*)getChildByTag(LABEL_LOSE_TAG);
	if (label) {
		char levelStr[16];
		sprintf(levelStr, "Lose : %d", count);
		label->setString(levelStr);
	}
}

void UserScene::menuBackCallback(Ref* pSender)
{
	Facebook::logout();
	Scene *pScene = TransitionFade::create(0.5, MenuScene::createScene());
	Director:: getInstance()->replaceScene(pScene);
}

void UserScene::menuPlayNow(Ref* pSender)
{
	Scene *pScene = TransitionFade::create(0.5, ModeScene::createScene(GAME_FROM_FACEBOOK));
	Director:: getInstance()->replaceScene(pScene);
}

void UserScene::menuShareGame(Ref* pSender)
{
	Facebook::postStatus();
}

void UserScene::menuInviteFriend(Ref* pSender)
{
	Facebook::inviteFriend();
}

void UserScene::menuBuyFeatures(Ref* pSender)
{
	Scene *pScene = TransitionFade::create(0.5, BuyScene::createScene());
	Director:: getInstance()->replaceScene(pScene);
}

void UserScene::menuAchievement(Ref* pSender)
{
	Scene *pScene = TransitionFade::create(0.5, AchievementScene::createScene(Facebook::getWin()));
	Director:: getInstance()->replaceScene(pScene);
}

void UserScene::menuLeaderboard(Ref* pSender)
{
	Scene *pScene = TransitionFade::create(0.5, LeaderboardScene::createScene());
	Director:: getInstance()->replaceScene(pScene);
}

void UserScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		menuBackCallback(NULL);
	}
}
