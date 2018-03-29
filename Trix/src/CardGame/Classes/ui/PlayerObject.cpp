#include "PlayerObject.h"
#include "common.h"
#include "CardSprite.h"
#include "CardDeck.h"
#include "Hand.h"
#include "GameScene.h"

USING_NS_CC;

#define SCORE_LABEL_TAG		10
#define NAME_LABEL_TAG		11
#define PHOTO_SPR_TAG		12

#define CIRCLE_SPRITE_TAG	13
#define MOVE_LABEL_TAG		14

#define PASS_SPRITE_TAG		15
#define CUR_SPRITE_TAG		16

#define COUNT_LABEL_TAG		17

#define LABEL_FONT_SIZE		22
#define FADE_TIMEOUT		1.0


PlayerObject::PlayerObject()
{
	for (int i = 0; i < 13; i++) {
		cards[i] = NULL;
	}

	for (int i = 0; i < 9; i++) {
		dupSprs[i] = NULL;
	}
}

PlayerObject::~PlayerObject()
{
	for (int i = 0; i < 13; i++) {
		if (cards[i] != NULL) {
			cards[i]->release();
			cards[i] = NULL;
		}
	}
}

// on "init" you need to initialize your instance
bool PlayerObject::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getWinSizeInPixels();
	origin = Director::getInstance()->getVisibleOrigin();
	center = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object

	selectedCard = NULL;
	leftCount = 0;
	avatarIndex = -1;
	CCLog("Call PlayerObject::init()");
	return true;
}

void PlayerObject::clear()
{
	stopAllAnimations();
	for (int i = 0; i < 9; i++) {
		if (dupSprs[i]) {
			dupSprs[i]->removeFromParent();
			dupSprs[i] = NULL;
		}
	}
	setScore(0);
}

void PlayerObject::stopAllAnimations()
{
	for (int i = 0; i < PLAYER_CARD_NUM; i++) {
		if (cards[i] != NULL) {
			cards[i]->stopAllActions();
		}
	}
}

void PlayerObject::setParent(GameScene *object)
{
	parentObject = object;
}

void PlayerObject::setAvatarIndex(int index)
{
	avatarIndex = index;
}

void PlayerObject::setPlayerIndex(int index)
{
	playerIndex = index;

	switch(index)
	{
	case 0: /* Player. Team 1*/
		drawAvatar();
		break;
	case 1: /* Bot 1. Team 2 */
		drawAvatar();
		break;
	case 2: /* Bot 2. Team 1 */
		drawAvatar();
		break;
	case 3: /* Bot 3. Team 2 */
		drawAvatar();
		break;
	}
}

void PlayerObject::drawAvatar()
{
	auto frameSpr = Sprite::create("avatars/frame.png");
	Size frameSize = frameSpr->getContentSize();

	auto labelName = LabelTTF::create("Robot", "Feil", LABEL_FONT_SIZE);
	Size labelNameSize = labelName->getContentSize();

	auto labelScore = LabelTTF::create("Score:0", "Feil", LABEL_FONT_SIZE);
	Size labelSize = labelScore->getContentSize();

	auto circleSpr = Sprite::create("avatars/circle.png");
	auto moveLabel = LabelTTF::create("0", "Feil", LABEL_FONT_SIZE * 3);
	moveLabel->setColor(Color3B::BLACK);

	auto passSpr = Sprite::create("avatars/pass.png");
	auto curSpr = Sprite::create("avatars/frame_s.png");

	Sprite *photoSpr = NULL;

	LabelTTF *labelCardCount = LabelTTF::create("13", "Feil", LABEL_FONT_SIZE * 3);
	labelCardCount->setColor(Color3B::WHITE);

	char photoName[40];
	if (avatarIndex == -1) {
		avatarIndex = playerIndex;
	}
	sprintf(photoName, "avatars/player%d.png", avatarIndex);

	switch(playerIndex) {
	case 0:
		frameSpr->setPosition(Vec2( - frameSize.height / 2 + 550, frameSize.height / 2));
		this->addChild(frameSpr, 200);

		circleSpr->setPosition(Vec2( - frameSize.height / 2 + 550, frameSize.height / 2));
		this->addChild(circleSpr, 204, CIRCLE_SPRITE_TAG);
		moveLabel->setPosition(Vec2( - frameSize.height / 2 + 550, frameSize.height / 2));
		this->addChild(moveLabel, 205, MOVE_LABEL_TAG);
		circleSpr->setVisible(false);
		moveLabel->setVisible(false);

		photoSpr = Sprite::create("avatars/playeru.png");
		photoSpr->setPosition(Vec2(- frameSize.height / 2 + 550, frameSize.height / 2));

		this->addChild(photoSpr, 202, PHOTO_SPR_TAG);
		layerSize.width = frameSize.width;
		layerSize.height = frameSize.height;

		labelName->setString("You");
		labelName->setPosition(- frameSize.height / 2 + 550, frameSize.height - labelSize.height / 2);
		addChild(labelName, 203, NAME_LABEL_TAG);

		labelScore->setPosition(- frameSize.height / 2 + 550, labelSize.height / 2 + 6);
		addChild(labelScore, 203, SCORE_LABEL_TAG);

		labelCardCount->setPosition(0, 0);
		addChild(labelCardCount, 210, COUNT_LABEL_TAG);
		labelCardCount->setVisible(false);
		break;
	case 1:
		frameSpr->setPosition(Vec2( - frameSize.height / 2 + 10, frameSize.height / 2 + 10));
		this->addChild(frameSpr, 0);

		circleSpr->setPosition(Vec2( - frameSize.height / 2 + 10, frameSize.height / 2 + 10));
		this->addChild(circleSpr, 4, CIRCLE_SPRITE_TAG);
		moveLabel->setPosition(Vec2( - frameSize.height / 2 + 10, frameSize.height / 2 + 10));
		this->addChild(moveLabel, 5, MOVE_LABEL_TAG);
		circleSpr->setVisible(false);
		moveLabel->setVisible(false);

		photoSpr = Sprite::create(photoName);
		photoSpr->setPosition(Vec2(- frameSize.height / 2 + 10, frameSize.height / 2 + 10));

		this->addChild(photoSpr, 2);
		layerSize.width = frameSize.width;
		layerSize.height = frameSize.height;

		labelName->setString(PROFILE_NAMES[avatarIndex]);
		labelName->setPosition(- frameSize.height / 2 + 10, frameSize.height - labelSize.height / 2 + 10);
		this->addChild(labelName, 3, NAME_LABEL_TAG);

		labelScore->setPosition(- frameSize.height / 2 + 10, labelSize.height / 2 + 16);
		this->addChild(labelScore, 3, SCORE_LABEL_TAG);

		labelCardCount->setPosition( -150 / 2 - 15, - 204 / 2 - 5);
		addChild(labelCardCount, 210, COUNT_LABEL_TAG);
		labelCardCount->setVisible(false);
		break;
	case 2:
		frameSpr->setPosition(Vec2(0 - frameSize.width / 2 - 30,  - frameSize.height / 2));
		this->addChild(frameSpr, 0);

		circleSpr->setPosition(Vec2(0 - frameSize.width / 2 - 30,  - frameSize.height / 2));
		this->addChild(circleSpr, 4, CIRCLE_SPRITE_TAG);
		moveLabel->setPosition(Vec2(0 - frameSize.width / 2 - 30,  - frameSize.height / 2));
		this->addChild(moveLabel, 5, MOVE_LABEL_TAG);
		circleSpr->setVisible(false);
		moveLabel->setVisible(false);

		photoSpr = Sprite::create(photoName);
		photoSpr->setPosition(Vec2(0 - frameSize.width / 2 - 30, - frameSize.height / 2));

		this->addChild(photoSpr, 2);
		layerSize.width = frameSize.width;
		layerSize.height = frameSize.height;

		labelName->setString(PROFILE_NAMES[avatarIndex]);
		labelName->setPosition(0 - frameSize.width / 2 - 30, - labelSize.height + 12);
		this->addChild(labelName, 3, NAME_LABEL_TAG);

		labelScore->setPosition(0 - frameSize.width / 2 - 30, - frameSize.height + labelSize.height / 2 + 6);
		this->addChild(labelScore, 3, SCORE_LABEL_TAG);

		labelCardCount->setPosition(150 / 2 + 15, - 204 / 2);
		addChild(labelCardCount, 210, COUNT_LABEL_TAG);
		labelCardCount->setVisible(false);
		break;
	case 3:
		frameSpr->setPosition(Vec2(frameSize.width / 2 + 10, - frameSize.height / 2 - 10));
		this->addChild(frameSpr, 0);

		circleSpr->setPosition(Vec2(frameSize.width / 2 + 10, - frameSize.height / 2 - 10));
		this->addChild(circleSpr, 4, CIRCLE_SPRITE_TAG);
		moveLabel->setPosition(Vec2(frameSize.width / 2 + 10, - frameSize.height / 2 - 10));
		this->addChild(moveLabel, 5, MOVE_LABEL_TAG);
		circleSpr->setVisible(false);
		moveLabel->setVisible(false);

		photoSpr = Sprite::create(photoName);
		photoSpr->setPosition(Vec2(frameSize.width / 2 + 10, - frameSize.height / 2 - 10));

		this->addChild(photoSpr, 2);
		layerSize.width = frameSize.width;
		layerSize.height = frameSize.height;

		labelName->setString(PROFILE_NAMES[avatarIndex]);
		labelName->setPosition(frameSize.width / 2 + 10, 0 - labelNameSize.height + 2);
		addChild(labelName, 3, NAME_LABEL_TAG);

		labelScore->setPosition(frameSize.width / 2 + 10, 0 - frameSize.height + labelSize.height / 2 - 4);
		addChild(labelScore, 3, SCORE_LABEL_TAG);

		labelCardCount->setPosition(150 / 2 + 15, 204 / 2 + 5);
		addChild(labelCardCount, 210, COUNT_LABEL_TAG);
		labelCardCount->setVisible(false);
		break;
	}

	passSpr->setPosition(circleSpr->getPosition());
	this->addChild(passSpr, 206, PASS_SPRITE_TAG);
	curSpr->setPosition(frameSpr->getPosition());
	this->addChild(curSpr, 206, CUR_SPRITE_TAG);
	passSpr->setVisible(false);
	curSpr->setVisible(false);
}

String PlayerObject::getPlayerName()
{
	LabelTTF *label = (LabelTTF*)getChildByTag(NAME_LABEL_TAG);
	return label->getString();
}

void PlayerObject::setUserInfo(const char* name, const char* avatar)
{
	if (playerIndex == 0) {
		LabelTTF *label = (LabelTTF*)getChildByTag(NAME_LABEL_TAG);
		if (strlen(name) > 13) {
			char otherName[16] = {0,};
			strncpy(otherName, name, 13);
			label->setString(otherName);
		} else {
			label->setString(name);
		}

		Sprite *spr = (Sprite *)getChildByTag(PHOTO_SPR_TAG);
		Size size = spr->getContentSize();
		spr->setTexture(avatar);
		Size realSize = spr->getContentSize();
		spr->setScale(size.width / realSize.width, size.height / realSize.height);
	}
}

void PlayerObject::endMove(Node *pSender, Object *obj, int playerIndex, int index, float x, float y)
{
	//	CCLog("Call start PlayerObject::endMove");

	PlayerObject *player = (PlayerObject*)obj;
	CardSprite *spr = (CardSprite*)pSender;

	spr->removeFromParent();
	player->addChild(spr, 100 + index);
	spr->setPosition(x, y);

	spr->setParent(player, index);

	//	CCLog("Call end PlayerObject::endMove");
}

void PlayerObject::showCard(cocos2d::Node *pSender)
{
	CardSprite *spr = (CardSprite*) pSender;
	spr->setVisible(true);
}

void PlayerObject::drawCards()
{
	CardSprite *spr = NULL;

	CCLog("Call start PlayerObject::drawCards");

	switch(playerIndex)
	{
	case 0:
		for(int i = 0; i < 13; i++)
		{
			spr = cards[i];
			if (spr) {
				spr->stopAllActions();

				float width = getContentSize().width;
				Size sprSize = spr->getContentSize();
				float delta = (width - sprSize.width) / 12;
				//CCLog("width(%f), sprSize(%f), delta(%f)", width, sprSize.width, delta);

				Point newPos = Vec2(-(width / 2.0) + sprSize.width / 2.0 + i * delta, sprSize.height / 2);

				DelayTime *action_1 = DelayTime::create(0.05 * i);
				MoveTo *action_2 = MoveTo::create(MOVE_SPEED, Point(getPosition().x -(width / 2) + sprSize.width / 2 + i * delta, getPosition().y));
				CCCallFuncN *action_3 = CCCallFuncN::create(CC_CALLBACK_1(PlayerObject::endMove, this, this, playerIndex, i, newPos.x, newPos.y));
				CCCallFuncN *action_4 = CCCallFuncN::create(CC_CALLBACK_1(PlayerObject::showCard, this));
				Sequence *action_5 = Sequence::create(action_1, action_2, action_3, action_4, NULL);
				spr->runAction(action_5);
			}
		}
		SimpleAudioEngine:: getInstance()->playEffect("sounds/shuffle.mp3", false);

		break;
	case 1:
		for(int i = 0; i < 13; i++)
		{
			spr = cards[i];
			if (spr) {
				spr->stopAllActions();
				Size sprSize = spr->getContentSize();
				Point newPos = Vec2( -sprSize.width /2 - 15, - sprSize.height / 2 - 5);

				DelayTime *action_1 = DelayTime::create(0.05 * i);
				MoveTo *action_2 = MoveTo::create(MOVE_SPEED, Point(getPosition().x, getPosition().y - layerSize.height / 2 - 5));
				CCCallFuncN *action_3 = CCCallFuncN::create(CC_CALLBACK_1(PlayerObject::endMove, this, this, playerIndex, i, newPos.x, newPos.y));
				Sequence *action_4 = Sequence::create(action_1, action_2, action_3, NULL);
				spr->runAction(action_4);
			}
		}
		SimpleAudioEngine:: getInstance()->playEffect("sounds/shuffle.mp3", false);

		break;
	case 2:
		for(int i = 0; i < 13; i++)
		{
			spr = cards[i];
			if (spr) {
				spr->stopAllActions();
				Size sprSize = spr->getContentSize();
				Point newPos = Vec2(sprSize.width / 2 + 15, - sprSize.height / 2);

				DelayTime *action_1 = DelayTime::create(0.05 * i);
				MoveTo *action_2 = MoveTo::create(MOVE_SPEED, Point(getPosition().x + layerSize.width / 2 + 15, getPosition().y));
				CCCallFuncN *action_3 = CCCallFuncN::create(CC_CALLBACK_1(PlayerObject::endMove, this, this, playerIndex, i, newPos.x, newPos.y));
				Sequence *action_4 = Sequence::create(action_1, action_2, action_3, NULL);
				spr->runAction(action_4);
			}
		}
		SimpleAudioEngine:: getInstance()->playEffect("sounds/shuffle.mp3", false);

		break;
	case 3:
		for(int i = 0; i < 13; i++)
		{
			spr = cards[i];
			if (spr) {
				spr->stopAllActions();
				Size sprSize = spr->getContentSize();

				Point newPos = Vec2(sprSize.width / 2 + 15, sprSize.height / 2 + 5);

				DelayTime *action_1 = DelayTime::create(0.05 * i);
				MoveTo *action_2 = MoveTo::create(MOVE_SPEED, Point(getPosition().x, getPosition().y + sprSize.height / 2 + 5));
				CCCallFuncN *action_3 = CCCallFuncN::create(CC_CALLBACK_1(PlayerObject::endMove, this, this, playerIndex, i, newPos.x, newPos.y));
				Sequence *action_4 = Sequence::create(action_1, action_2, action_3, NULL);
				spr->runAction(action_4);
			}
		}
		SimpleAudioEngine:: getInstance()->playEffect("sounds/shuffle.mp3", false);

		break;
	}

	CCLog("Call end PlayerObject::drawCards");
}

void PlayerObject::setHand(cocos2d::Vector<CardSprite*> *cardList, Hand *hand)
{
	CCLog("Call start PlayerObject::setHand()");

	for (int i = 0; i < 13; i++) {
		if (cards[i] != NULL) {
			cards[i]->release();
		}
		cards[i] = cardList->at(hand->cardIndex(i));
		cards[i]->retain();
		cards[i]->card.suit = hand->cardSuit(i);
		cards[i]->card.value = hand->cardValue(i);
	}

	setLeftCount(13);

	drawCards();
	CCLog("Call end PlayerObject::setHand()");
}

bool PlayerObject::onTouchedCard(CardSprite *card, bool select)
{
	if(!select)
	{
		selectedCard = NULL;
		return true;
	}
	else
	{
		if(selectedCard)
		{
			selectedCard->onTouchedFromParent();
			selectedCard = NULL;
			return false;
		}

		selectedCard = card;
	}
	return true;
}

void PlayerObject::endPlayMove(Node *pSender, Object *obj, int playerIndex)
{
	PlayerObject *player = (PlayerObject*)obj;
	CardSprite *spr = (CardSprite*)pSender;

	CCLog("call endPlayMove(%p, %p)", player, spr);

	player->parentObject->onPlayed(playerIndex, spr);
	player->selectedCard = NULL;
}

void PlayerObject::onTouchedTable()
{
	CCLog("PlayerObject::onTouchedTable playerIndex(%d)", playerIndex);

	if (selectedCard) {
		selectedCard->setDim(false);
		if (playerIndex == 0) {
			selectedCard->setEnableTouch(false);
		}

		bool isTrix = parentObject->isTrixBid();
		Point newPos = isTrix ? getPlayedCardPositionForTrix() : getPlayedCardPosition();
		int zorder = isTrix && selectedCard->card.value > CARD_J ? 60 : 120;

		selectedCard->removeFromParent();
		int index = selectedCard->cardIndex;
		selectedCard->setVisible(true);
		parentObject->addChild(selectedCard, zorder + parentObject->playedIndex++);
		selectedCard->setPosition(getPosition().x + selectedCard->getPosition().x, getPosition().y + selectedCard->getPosition().y);

		//CCLog("play MovePos(%f, %f)", newPos.x, newPos.y);

		MoveTo *action_1 = MoveTo::create(CARD_PLAY_SPEED, newPos);
		CCCallFuncN *action_2 = CCCallFuncN::create(CC_CALLBACK_1(PlayerObject::endPlayMove, this, this, playerIndex));
		Sequence *action_3 = Sequence::create(action_1, action_2, NULL);
		selectedCard->runAction(action_3);
		SimpleAudioEngine:: getInstance()->playEffect("sounds/pop.mp3", false);

		cards[index]->release();
		setLeftCount(leftCount - 1);

		cards[index] = NULL;
		setEnableTouchCards(SUIT_ALL, false);
		refreshUserCards();
		selectedCard = NULL;
	}
	CCLog("PlayerObject::onTouchedTable end");
}

Point PlayerObject::getPlayedCardPosition()
{
	Size cardSize = selectedCard->getContentSize();

	Point newPos;
	switch(playerIndex) {
	case 0:
		newPos.x = center.x;
		newPos.y = center.y - CARD_SPACE_DELTA_Y - cardSize.height / 2;
		break;
	case 1:
		newPos.x = center.x + CARD_SPACE_DELTA_X + cardSize.width / 2;
		newPos.y = center.y;
		break;
	case 2:
		newPos.x = center.x;
		newPos.y = center.y + CARD_SPACE_DELTA_Y + cardSize.height / 2;
		break;
	case 3:
		newPos.x = center.x - CARD_SPACE_DELTA_X - cardSize.width / 2;
		newPos.y = center.y;
		break;
	}

	return newPos;
}

Point PlayerObject::getPlayedCardPositionForTrix()
{
	Size cardSize = selectedCard->getContentSize();

	Suit suit = selectedCard->card.suit;
	int value = selectedCard->card.value;

	float deltaY = value > CARD_J ? - CARD_SPACE_DELTA_Y / 2 : CARD_SPACE_DELTA_Y / 2;
	Point newPos;

	switch(suit) {
	case HEART:
		newPos.x = center.x - cardSize.width * 2 - 20 + cardSize.width / 2;
		newPos.y = center.y + deltaY;
		break;
	case CLOVER:
		newPos.x = center.x - cardSize.width - 10 + cardSize.width / 2;
		newPos.y = center.y + deltaY;
		break;
	case DIAMOND:
		newPos.x = center.x + cardSize.width + 10 - cardSize.width / 2;
		newPos.y = center.y + deltaY;
		break;
	case SPADE:
		newPos.x = center.x + cardSize.width * 2 + 20 - cardSize.width / 2;
		newPos.y = center.y + deltaY;
		break;
	default:
		break;
	}

	return newPos;
}

void PlayerObject::refreshUserCards()
{
	/* Only for User */
	if(playerIndex != 0)
		return;

	CardSprite *spr = NULL;
	int drawIndex = 0;
	for (int i = 0; i < 13; i++) {
		spr = cards[i];
		if(spr == NULL)
			continue;

		spr->setVisible(true);

		float width = getContentSize().width;
		Size sprSize = spr->getContentSize();
		float delta = (width - sprSize.width) / 13;
		width = width - delta * (13 - leftCount);
		//CCLog("width(%f), sprSize(%f), delta(%f)", width, sprSize.width, delta);

		Point newPos = Vec2(-(width / 2.0) + sprSize.width / 2.0 + drawIndex * delta, sprSize.height / 2);

		MoveTo *action_2 = MoveTo::create(MOVE_SPEED, newPos);
		spr->runAction(action_2);
		drawIndex ++;
	}
}

void PlayerObject::setSelectedCard(int index)
{
	CCLog("PlayerObject::setSelectedCard %d", index);

	if (cards[index]) {
		selectedCard = cards[index];
	} else {
		selectedCard = NULL;
		CCLog("Card is empty!");
	}
}

void PlayerObject::setEnableTouchCards(int suitType, int enable)
{
	CCLog("PlayerObject::setEnableTouchCards");

	for (int i = 0; i < 13; i++) {
		if(!cards[i])
			continue;
		if (suitType & cards[i]->card.suit) {
			cards[i]->setDim(!enable);
			cards[i]->setEnableTouch(enable);
		} else {
			cards[i]->setDim(true);
			cards[i]->setEnableTouch(false);
		}
	}
}

void PlayerObject::setEnableTouchCards(Card enableds[], int count)
{
	CCLog("PlayerObject::setEnableTouchCards");

	for (int i = 0; i < 13; i++) {
		if (!cards[i])
			continue;

		bool enabled = false;
		for (int j = 0; j < count; j++) {
			if (cards[i]->card.suit == enableds[j].suit && cards[i]->card.value == enableds[j].value) {
				enabled = true;
				break;
			}
		}

		cards[i]->setDim(!enabled);
		cards[i]->setEnableTouch(enabled);
	}
}

void PlayerObject::setScore(int score)
{
	LabelTTF *labelScore = (LabelTTF*)getChildByTag(SCORE_LABEL_TAG);
	if(labelScore)
	{
		char scoreStr[24] = {0};
		sprintf(scoreStr, "Score %d", score);
		labelScore->setString(scoreStr);
	}
}

void PlayerObject::endShowScore(Node *pSender, Object *obj)
{
	PlayerObject *player = (PlayerObject*)obj;

	LabelTTF *moveLabel = (LabelTTF*)getChildByTag(MOVE_LABEL_TAG);
	Sprite *moveCircle = (Sprite*)getChildByTag(CIRCLE_SPRITE_TAG);

	moveLabel->setVisible(false);
	moveCircle->setVisible(false);
}

void PlayerObject::setMove(int value)
{
	if (value == 0)
		return;

	FadeIn *action1 = FadeIn::create(FADE_TIMEOUT);
	FadeOut *action2 = FadeOut::create(FADE_TIMEOUT);
	CCCallFuncN *action3 = CCCallFuncN::create(CC_CALLBACK_1(PlayerObject::endShowScore, this, this));

	LabelTTF *moveLabel = (LabelTTF*)getChildByTag(MOVE_LABEL_TAG);
	Sprite *moveCircle = (Sprite*)getChildByTag(CIRCLE_SPRITE_TAG);

	char valStr[5] = "";
	sprintf(valStr, "%d", value);
	moveLabel->setString(valStr);

	moveLabel->setVisible(true);
	moveCircle->setVisible(true);

	Sequence *action4 = Sequence::create(action1, action2, action3, NULL);
	moveLabel->runAction(action4);
	moveCircle->runAction(action4);
}

void PlayerObject::setEnabled(bool enable)
{
	for (int i = 0; i < 13; i++) {
		if (!cards[i])
			continue;
		cards[i]->setEnabled(enable);
	}
}

void PlayerObject::showPass(bool show)
{
	Sprite *spr = (Sprite *)getChildByTag(PASS_SPRITE_TAG);
	spr->setVisible(show);
}

void PlayerObject::showCurrent(bool show)
{
	Sprite *spr = (Sprite *)getChildByTag(CUR_SPRITE_TAG);
	spr->setVisible(show);
}

void PlayerObject::setLeftCount(int count)
{
	leftCount = count;

	LabelTTF *label = (LabelTTF *) getChildByTag(COUNT_LABEL_TAG);
	if (playerIndex != 0 && parentObject->isTrixBid()) {
		char str[20] = {0};
		sprintf(str, "%d", leftCount);
		label->setString(str);
		label->setVisible(true);
	} else {
		label->setVisible(false);
	}
}

void PlayerObject::showDuplicate(int duplicates)
{
	CCLog("Call PlayerObject::showDuplicate");

	float x = 0;
	float y = 0;

	switch (playerIndex) {
	case 0:
		x = 200;
		y = 240;
		break;
	case 1:
		x = - 224;
		y = - 5;
		break;
	case 2:
		x = 180;
		y = -45;
		break;
	case 3:
		x = 180;
		y = 180;
		break;
	default:
		return;
	}

	int dups[9] = {DUP_K, DUP_H_Q, DUP_C_Q, DUP_D_Q, DUP_S_Q, DUP_H_2, DUP_C_2, DUP_D_2, DUP_S_2};
	char names[9][30] = {"bid/dup_hk.png", "bid/dup_hq.png", "bid/dup_cq.png", "bid/dup_dq.png", "bid/dup_sq.png",
			"bid/dup_h2.png", "bid/dup_c2.png", "bid/dup_d2.png", "bid/dup_s2.png"};

	int count = 0;
	for (int i = 0; i < 9; i++) {
		if (dupSprs[i]) {
			dupSprs[i]->removeFromParent();
			dupSprs[i] = NULL;
		}

		if (HAS_BIT(duplicates, dups[i])) {
			dupSprs[count] = Sprite::create(names[i]);
			Size size = dupSprs[count]->getContentSize();
			dupSprs[count]->setPosition(x + size.width / 2, y - size.height / 2);
			this->addChild(dupSprs[count], 220);

			if (playerIndex == 0) {
				x += size.width + 3;
			} else {
				y -= size.height + 3;
			}
			count ++;
		}
	}

}
