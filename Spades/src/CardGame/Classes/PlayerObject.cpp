#include "PlayerObject.h"
#include "common.h"
#include "CardSprite.h"
#include "CardDeck.h"
#include "Hand.h"
#include "GameScene.h"

USING_NS_CC;

#define SCORE_LABEL_TAG 10
#define NAME_LABEL_TAG  11
#define PHOTO_SPR_TAG   12

#define CIRCLE_SPRITE_TAG 	13
#define LABEL_BID_TAG		14

#define LABEL_FONT_SIZE		22
#define FADE_TIMEOUT		1.0

PlayerObject::PlayerObject()
{
	for (int i = 0; i < 13; i++) {
		cards[i] = NULL;
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
	Sprite *frameSpr = Sprite::create("avatars/frame.png");
	Size frameSize = frameSpr->getContentSize();

	LabelTTF *labelName = LabelTTF::create("Robot", "Feil", LABEL_FONT_SIZE);
	Size labelNameSize = labelName->getContentSize();

	LabelTTF *labelScore = LabelTTF::create("Score:0", "Feil", LABEL_FONT_SIZE);
	Size labelSize = labelScore->getContentSize();

	Sprite *circleSpr = Sprite::create("avatars/circle.png");
	LabelTTF *bidLabel = LabelTTF::create("0", "Feil", LABEL_FONT_SIZE * 3);
	bidLabel->setColor(Color3B::BLACK);

	Sprite *photoSpr = NULL;

	char photoName[40];
	if (avatarIndex == -1) {
		avatarIndex = playerIndex;
	}
	sprintf(photoName, "avatars/player%d.png", avatarIndex);
	switch(playerIndex)
	{
	case 0:
		frameSpr->setPosition(Vec2( - frameSize.height / 2 + 550, frameSize.height / 2));
		this->addChild(frameSpr, 200);

		circleSpr->setPosition(Vec2( - frameSize.height / 2 + 550, frameSize.height / 2));
		this->addChild(circleSpr, 204, CIRCLE_SPRITE_TAG);
		bidLabel->setPosition(Vec2( - frameSize.height / 2 + 550, frameSize.height / 2));
		this->addChild(bidLabel, 205, LABEL_BID_TAG);
		circleSpr->setVisible(false);
		bidLabel->setVisible(false);

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
		break;
	case 1:
		frameSpr->setPosition(Vec2(frameSize.width / 2 + 10, - frameSize.height / 2 - 10));
		this->addChild(frameSpr, 0);

		circleSpr->setPosition(Vec2(frameSize.width / 2 + 10, - frameSize.height / 2 - 10));
		this->addChild(circleSpr, 4, CIRCLE_SPRITE_TAG);
		bidLabel->setPosition(Vec2(frameSize.width / 2 + 10, - frameSize.height / 2 - 10));
		this->addChild(bidLabel, 5, LABEL_BID_TAG);
		circleSpr->setVisible(false);
		bidLabel->setVisible(false);

		photoSpr = Sprite::create(photoName);
		photoSpr->setPosition(Vec2(frameSize.width / 2 + 10, - frameSize.height / 2 - 10));

		this->addChild(photoSpr, 2);
		layerSize.width = frameSize.width;
		layerSize.height = frameSize.height;

		labelName->setString("James Jhon");
		labelName->setPosition(frameSize.width / 2 + 10, 0 - labelNameSize.height + 2);
		addChild(labelName, 3, NAME_LABEL_TAG);

		labelScore->setPosition(frameSize.width / 2 + 10, 0 - frameSize.height + labelSize.height / 2 - 4);
		addChild(labelScore, 3, SCORE_LABEL_TAG);
		break;
	case 2:
		frameSpr->setPosition(Vec2(0 - frameSize.width / 2 - 30,  - frameSize.height / 2));
		this->addChild(frameSpr, 0);

		circleSpr->setPosition(Vec2(0 - frameSize.width / 2 - 30,  - frameSize.height / 2));
		this->addChild(circleSpr, 4, CIRCLE_SPRITE_TAG);
		bidLabel->setPosition(Vec2(0 - frameSize.width / 2 - 30,  - frameSize.height / 2));
		this->addChild(bidLabel, 5, LABEL_BID_TAG);
		circleSpr->setVisible(false);
		bidLabel->setVisible(false);

		photoSpr = Sprite::create(photoName);
		photoSpr->setPosition(Vec2(0 - frameSize.width / 2 - 30, - frameSize.height / 2));

		this->addChild(photoSpr, 2);
		layerSize.width = frameSize.width;
		layerSize.height = frameSize.height;

		labelName->setString("Alin Tomas");
		labelName->setPosition(0 - frameSize.width / 2 - 30, - labelSize.height + 12);
		addChild(labelName, 3, NAME_LABEL_TAG);

		labelScore->setPosition(0 - frameSize.width / 2 - 30, - frameSize.height + labelSize.height / 2 + 6);
		addChild(labelScore, 3, SCORE_LABEL_TAG);
		break;
	case 3:
		frameSpr->setPosition(Vec2( - frameSize.height / 2 + 10, frameSize.height / 2 + 10));
		this->addChild(frameSpr, 0);

		circleSpr->setPosition(Vec2( - frameSize.height / 2 + 10, frameSize.height / 2 + 10));
		this->addChild(circleSpr, 4, CIRCLE_SPRITE_TAG);
		bidLabel->setPosition(Vec2( - frameSize.height / 2 + 10, frameSize.height / 2 + 10));
		this->addChild(bidLabel, 5, LABEL_BID_TAG);
		circleSpr->setVisible(false);
		bidLabel->setVisible(false);

		photoSpr = Sprite::create(photoName);
		photoSpr->setPosition(Vec2(- frameSize.height / 2 + 10, frameSize.height / 2 + 10));

		this->addChild(photoSpr, 2);
		layerSize.width = frameSize.width;
		layerSize.height = frameSize.height;

		labelName->setString("Joshep Panhal");
		labelName->setPosition(- frameSize.height / 2 + 10, frameSize.height - labelSize.height / 2 + 10);
		addChild(labelName, 3, NAME_LABEL_TAG);

		labelScore->setPosition(- frameSize.height / 2 + 10, labelSize.height / 2 + 16);
		addChild(labelScore, 3, SCORE_LABEL_TAG);
		break;
	}
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

void PlayerObject::showCards()
{
	for(int i = 0; i < 13; i++)
		cards[i]->setVisible(true);
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
				Sequence *action_4 = Sequence::create(action_1, action_2, action_3, NULL);
				spr->runAction(action_4);
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
				//layerSize.height = layerSize.height + sprSize.height + 20;
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
	case 2:
		for(int i = 0; i < 13; i++)
		{
			spr = cards[i];
			if (spr) {
				spr->stopAllActions();
				Size frameSize = spr->getContentSize();

				//spr->removeFromParent();
				//spr->setVisible(true);

				Size sprSize = spr->getContentSize();
				//layerSize.height = layerSize.height + sprSize.height + 20;

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
				Size frameSize = spr->getContentSize();

				//spr->removeFromParent();
				//spr->setVisible(true);

				Size sprSize = spr->getContentSize();
				//layerSize.height = layerSize.height + sprSize.height + 20;
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
	leftCount = 13;
	drawCards();
	CCLog("Call end PlayerObject::setHand()");
}

bool PlayerObject::onTouchedCard(CardSprite *card, bool select)
{
	/* TODO: pre-processing module must be implemented.*/

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

		Point newPos;
		Size cardSize = selectedCard->getContentSize();
		switch(playerIndex)
		{
		case 0:
			selectedCard->setEnableTouch(false);
			newPos.x = origin.x + visibleSize.width / 2;
			newPos.y = origin.y + visibleSize.height / 2 - CARD_SPACE_DELTA_Y - cardSize.height / 2;
			break;
		case 1:
			newPos.x = origin.x + visibleSize.width / 2 - CARD_SPACE_DELTA_X - cardSize.width / 2;
			newPos.y = origin.y + visibleSize.height / 2;
			break;
		case 2:
			newPos.x = origin.x + visibleSize.width / 2;
			newPos.y = origin.y + visibleSize.height / 2 + CARD_SPACE_DELTA_Y + cardSize.height / 2;
			break;
		case 3:
			newPos.x = origin.x + visibleSize.width / 2 + CARD_SPACE_DELTA_X + cardSize.width / 2;
			newPos.y = origin.y + visibleSize.height / 2;
			break;
		}

		selectedCard->removeFromParent();
		int index = selectedCard->cardIndex;
		selectedCard->setVisible(true);
		parentObject->addChild(selectedCard, 50 + parentObject->playedIndex++);
		selectedCard->setPosition(getPosition().x + selectedCard->getPosition().x,
								getPosition().y + selectedCard->getPosition().y);

		//CCLog("play MovePos(%f, %f)", newPos.x, newPos.y);

		MoveTo *action_1 = MoveTo::create(CARD_PLAY_SPEED, newPos);
		CCCallFuncN *action_2 = CCCallFuncN::create(CC_CALLBACK_1(PlayerObject::endPlayMove, this, this, playerIndex));
		Sequence *action_3 = Sequence::create(action_1, action_2, NULL);
		selectedCard->runAction(action_3);
		SimpleAudioEngine:: getInstance()->playEffect("sounds/pop.mp3", false);

		cards[index]->release();
		leftCount --;
		cards[index] = NULL;
		setEnableTouchCards(SUIT_ALL, false);
		refreshUserCards();
		selectedCard = NULL;
	}
	CCLog("PlayerObject::onTouchedTable end");
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

void PlayerObject::setScore(int bid, int score)
{
	LabelTTF *labelScore = (LabelTTF*)getChildByTag(SCORE_LABEL_TAG);
	if(labelScore)
	{
		char scoreStr[24] = {0};
		sprintf(scoreStr, "Bid %d / Score %d", bid, score);
		labelScore->setString(scoreStr);
	}
}

void PlayerObject::setBid(int value)
{
	if (playerIndex == 0)
		return;

	FadeIn *action1 = FadeIn::create(FADE_TIMEOUT);

	LabelTTF *labelBid = (LabelTTF*)getChildByTag(LABEL_BID_TAG);
	Sprite *circleBid = (Sprite*)getChildByTag(CIRCLE_SPRITE_TAG);

	char valStr[5] = "";
	sprintf(valStr, "%d", value);
	labelBid->setVisible(true);
	circleBid->setVisible(true);

	labelBid->setString(valStr);
	labelBid->runAction(action1);
	circleBid->runAction(action1);
}

void PlayerObject::removeBid()
{
	if (playerIndex == 0)
		return;

	LabelTTF *labelBid = (LabelTTF*)getChildByTag(LABEL_BID_TAG);
	Sprite *circleBid = (Sprite*)getChildByTag(CIRCLE_SPRITE_TAG);
	labelBid->setVisible(false);
	circleBid->setVisible(false);
}

void PlayerObject::setEnabled(bool enable)
{
	for (int i = 0; i < 13; i++) {
		if (!cards[i])
			continue;
		cards[i]->setEnabled(enable);
	}
}
