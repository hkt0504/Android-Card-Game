#include "EatScene.h"
#include "common.h"
#include "GameScene.h"
#include "Controller.h"
#include "PlayerObject.h"

USING_NS_CC;

#define KING_TAG    11
#define QUEEN_TAG   12
#define DIAMOND_TAG 13
#define LOTOSH_TAG  14
#define TRIX_TAG    15
#define COMPLEX_TAG 16

// on "init" you need to initialize your instance
bool EatScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!LayerColor::initWithColor(ccc4(0, 0, 0, 0))) {
		return false;
	}

	BlendFunc blend;
	blend.src = GL_SRC_ALPHA;
	blend.dst = GL_ONE_MINUS_SRC_ALPHA;

	setBlendFunc(blend);
	setContentSize(visibleSize);

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	center = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object

	auto spriteBg = Sprite::create("bid/eat_bg.jpg");
	Size imgSize = spriteBg->getContentSize();
	spriteBg->setScale(SCR_W / imgSize.width, SCR_H / imgSize.height);
	spriteBg->setPosition(center);
	this->addChild(spriteBg, 1, 0);

	auto backItem = MenuItemImage::create("Buttons/btn_close.png", "Buttons/btn_close_d.png", CC_CALLBACK_1(EatScene::menuBackCallback, this));
	backItem->setPosition(Vec2(origin.x + visibleSize.width - backItem->getContentSize().width/2 - 40, origin.y + visibleSize.height - backItem->getContentSize().height/2 - 40));
	auto menuBack = Menu::create(backItem, NULL);
	menuBack->setPosition(Vec2::ZERO);
	this->addChild(menuBack, 2);

	CCLog(" Call EatScene::init()");
	return true;
}

void EatScene::showCards()
{
	EatCard cards[4];
	gameScene->controller->getEatCard(cards);

	char photo[40] = {0, };

	// draw avatar
	drawAvatar("You", "avatars/playeru.png", cards[0].score, 850, Y(86.0));

	int index = gameScene->player[1]->avatarIndex;
	sprintf(photo, "avatars/player%d.png", index);
	drawAvatar(PROFILE_NAMES[index], photo, cards[1].score, 1202, Y(140.0));

	index = gameScene->player[2]->avatarIndex;
	sprintf(photo, "avatars/player%d.png", index);
	drawAvatar(PROFILE_NAMES[index], photo, cards[2].score, 376, Y(780.0));

	index = gameScene->player[3]->avatarIndex;
	sprintf(photo, "avatars/player%d.png", index);
	drawAvatar(PROFILE_NAMES[index], photo, cards[3].score, 72, Y(648.0));

	// user
	auto sprItem0 = Sprite::create("bid/eat_item_h.png");
	sprItem0->setPosition(563, Y(86.0));
	this->addChild(sprItem0, 2);
	setOnePlayer(cards[0], 400, Y(86.0), 106, 0);

	// player 1
	auto sprItem1 = Sprite::create("bid/eat_item_v.png");
	sprItem1->setPosition(1202, Y(460.0));
	this->addChild(sprItem1, 2);
	setOnePlayer(cards[1], 1202, Y(646.0), 0, -116);

	// player 2
	auto sprItem2 = Sprite::create("bid/eat_item_h.png");
	sprItem2->setPosition(660, Y(780.0));
	this->addChild(sprItem2, 2);
	setOnePlayer(cards[2], 504, Y(780.0), 106, 0);

	// player 3
	auto sprItem3 = Sprite::create("bid/eat_item_v.png");
	sprItem3->setPosition(72, Y(330.0));
	this->addChild(sprItem3, 2);
	setOnePlayer(cards[3], 72, Y(515.0), 0, -116);
}

void EatScene::drawAvatar(const char *name, char *photo, int score, float x, float y)
{
	auto frameSpr = Sprite::create("avatars/frame2.png");
	Size frameSize = frameSpr->getContentSize();
	frameSpr->setPosition(x, y);
	this->addChild(frameSpr, 3);

	auto photoSpr = Sprite::create(photo);
	Size photoSize = photoSpr->getContentSize();
	photoSpr->setScale(frameSize.width / photoSize.width, frameSize.width / photoSize.width);
	photoSpr->setPosition(x, y);
	this->addChild(photoSpr, 4);

	auto labelName = LabelTTF::create(name, "Feil", 18);
	labelName->setColor(Color3B::WHITE);
	Size nameSize = labelName->getContentSize();
	labelName->setPosition(x, y + frameSize.height / 2 - nameSize.height / 2 - 1);
	this->addChild(labelName, 5);

	char scoreText[30] = {0, };
	sprintf(scoreText, "Score:%d", score);
	auto labelScore = LabelTTF::create(scoreText, "Feil", 18);
	labelScore->setColor(Color3B::WHITE);
	Size scoreSize = labelScore->getContentSize();
	labelScore->setPosition(x, y - frameSize.height / 2 + scoreSize.height / 2 + 1);
	this->addChild(labelScore, 5);
}

void EatScene::setOnePlayer(EatCard eat, float x, float y, float deltaX, float deltaY)
{
	if (HAS_BIT(eat.eatKQ, DUP_K)) {
		auto sprK = Sprite::create("bid/eat_k.png");
		sprK->setPosition(x, y);
		this->addChild(sprK, 3);

		x += deltaX;
		y += deltaY;
	}

	if (HAS_BIT(eat.eatKQ, DUP_Q_MASK)) {
		auto sprQ = Sprite::create("bid/eat_q.png");
		sprQ->setPosition(x, y);
		this->addChild(sprQ, 3);

		float x1 = x + 18;
		float y1 = y + 40;
		if (HAS_BIT(eat.eatKQ, DUP_H_Q)) {
			auto spr = Sprite::create("bid/eat_heart.png");
			spr->setPosition(x1, y1);
			this->addChild(spr, 4);
		}

		if (HAS_BIT(eat.eatKQ, DUP_C_Q)) {
			auto spr = Sprite::create("bid/eat_clover.png");
			spr->setPosition(x1, y1 - 25);
			this->addChild(spr, 4);
		}

		if (HAS_BIT(eat.eatKQ, DUP_D_Q)) {
			auto spr = Sprite::create("bid/eat_diamond.png");
			spr->setPosition(x1, y1 - 50);
			this->addChild(spr, 4);
		}

		if (HAS_BIT(eat.eatKQ, DUP_S_Q)) {
			auto spr = Sprite::create("bid/eat_spade.png");
			spr->setPosition(x1, y1 - 75);
			this->addChild(spr, 4);
		}

		x += deltaX;
		y += deltaY;
	}

	if (eat.eatD > 0) {
		auto sprD = Sprite::create("bid/eat_d.png");
		sprD->setPosition(x, y);
		this->addChild(sprD, 3);

		char count[10] = {0,};
		sprintf(count, "%d", eat.eatD);
		auto lblD = LabelTTF::create(count, "Felt", 30);
		lblD->setPosition(x, y);
		lblD->setColor(Color3B::WHITE);
		this->addChild(lblD, 4);

		x += deltaX;
		y += deltaY;
	}

	if (eat.eatL > 0) {
		auto sprL = Sprite::create("bid/eat_l.png");
		sprL->setPosition(x, y);
		this->addChild(sprL, 3);

		char count[10] = {0,};
		sprintf(count, "%d", eat.eatL);
		auto lblL = LabelTTF::create(count, "Felt", 30);
		lblL->setPosition(x, y);
		lblL->setColor(Color3B::WHITE);
		this->addChild(lblL, 4);
	}
}

void EatScene::menuBackCallback(Ref* pSender)
{
	this->removeFromParent();
	gameScene->setEnabled(true);
}
