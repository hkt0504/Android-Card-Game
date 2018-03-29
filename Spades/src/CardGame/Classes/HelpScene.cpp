#include "HelpScene.h"
#include "MenuScene.h"
#include "common.h"
#include "ui/UIScrollView.h"
#include "ui/UIImageView.h"

USING_NS_CC;

Scene* HelpScene::createScene()
{
	auto scene = Scene::create();
	auto layer = HelpScene::create();
	scene->addChild(layer);
	return scene;
}

bool HelpScene::init()
{
	if (!Layer::init()) {
		return false;
	}

	this->setKeypadEnabled(true);

	visibleSize = Director::getInstance()->getWinSizeInPixels();
	origin = Director::getInstance()->getVisibleOrigin();

	cocos2d::Sprite *spriteBg = Sprite::create("bg1.jpg");
	Size imgSize = spriteBg->getContentSize();
	spriteBg->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height / 2));
	this->addChild(spriteBg, 0);

	auto backItem = MenuItemImage::create("Buttons/btn_back.png", "Buttons/btn_back_d.png", CC_CALLBACK_1(HelpScene::menuBackCallback, this));
	backItem->setPosition(Vec2(origin.x + visibleSize.width - backItem->getContentSize().width / 2 - BACK_BTN_MARGIN,
			origin.y + visibleSize.height - backItem->getContentSize().height / 2 - BACK_BTN_MARGIN));
	auto menuBack = Menu::create(backItem, NULL);
	menuBack->setPosition(Vec2::ZERO);
	this->addChild(menuBack, 1);

	Sprite *titleBg = Sprite::create("help/howtoplay_title.png");
	titleBg->setPosition(Vec2(origin.x + visibleSize.width/2, origin.y + visibleSize.height - titleBg->getContentSize().height / 2 - 50));
	this->addChild(titleBg, 2);

	Sprite *textBg = Sprite::create("help/text_bg.png");
	this->addChild(textBg);
	textBg->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 60);

	ui::ScrollView* scrollView = ui::ScrollView::create();

	scrollView->setTouchEnabled(true);
	this->addChild(scrollView);

	ui::ImageView* imageView = ui::ImageView::create("help/text.png");

	scrollView->setContentSize(Size(imageView->getContentSize().width, textBg->getContentSize().height - 20));

	float innerWidth = scrollView->getContentSize().width;
	float innerHeight = imageView->getContentSize().height;

	scrollView->setInnerContainerSize(Size(innerWidth, innerHeight));
	scrollView->setPosition(Vec2(visibleSize.width / 2 - innerWidth / 2, (visibleSize.height / 2) - textBg->getContentSize().height / 2 - 50));

	scrollView->addChild(imageView);

	imageView->setPosition(Vec2(innerWidth / 2.0f, innerHeight / 2 + 20));

	return true;
}

void HelpScene::menuBackCallback(Ref* pSender)
{
	Scene *pScene = TransitionFade::create(0.5, MenuScene::createScene());
	Director:: getInstance()->replaceScene(pScene);
}

void HelpScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
		menuBackCallback(NULL);
	}
}
