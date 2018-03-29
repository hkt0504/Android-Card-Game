#include "CardSprite.h"
#include "PlayerObject.h"

USING_NS_CC;

// on "init" you need to initialize your instance
bool CardSprite::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	center = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);

	selected = false;
	visible = false;
	cardIndex = 0;
	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	//    CCLog("Call CardSprite::init()");
	enabled = true;
	return true;
}

void CardSprite::setCard(const char *filename, Suit suit, int value)
{
	Sprite *spr = Sprite::create(filename);
	if (spr) {
		spr->setPosition(Vec2(0,0));
		this->addChild(spr, 0);
		upCard = spr;

		setContentSize(upCard->getContentSize());
		spr = Sprite::create("cards/cardback1.png");
		if (spr) {
			spr->setPosition(Vec2(0,0));
			this->addChild(spr, 1);
			backCard = spr;
			backCard->setVisible(false);
		}
		setDim(true);
	}

	card.suit = suit;
	card.value = value;
}

void CardSprite::setVisible(bool value)
{
	visible = value;
	backCard->setVisible(!value);
}

void CardSprite::setEnabled(int enable)
{
	enabled = enable;
}

void CardSprite::setEnableTouch(bool enable)
{
	if (enable) {
		EventListenerTouchOneByOne *listener =  EventListenerTouchOneByOne::create();
		listener->onTouchBegan = [](Touch* touch, Event* event){
			// event->getCurrentTarget() returns the *listener's* sceneGraphPriority node.
			CardSprite *target = static_cast<CardSprite*>(event->getCurrentTarget());

			if(!target->enabled)
				return true;
			//Get the position of the current point relative to the button
			return target->onTouched(touch, event);
			//Check the click area
		};

		listener->setSwallowTouches(true);

		_eventDispatcher->addEventListenerWithFixedPriority(listener, 200 - getZOrder());
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	} else {
		_eventDispatcher->removeEventListenersForTarget(this, false);
	}
}

void CardSprite::initAll()
{
	selected = false;
	cardIndex = -1;
	setEnableTouch(false);
	setVisible(false);
	setDim(true);
}

bool CardSprite::onTouched(Touch *touch, Event *event)
{
	Point locationInNode = upCard->convertToNodeSpace(touch->getLocation());
	locationInNode = locationInNode;
	Size s = upCard->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	if (rect.containsPoint(locationInNode))
	{
		if(visible)
		{
			log("card touch...(%f - %f)  x = %f, y = %f", s.width, s.height, locationInNode.x, locationInNode.y);
			if(selected) /* move down */
			{
				parentObject->onTouchedCard(this, 0);
				MoveBy *action_1 = MoveBy::create(0.2f, Point(0, -30));
				runAction(action_1);
			}
			else /* move up */
			{
				if(parentObject)
				{
					bool allowCard = parentObject->onTouchedCard(this, 1);
					if(allowCard)
					{
						MoveBy *action_1 = MoveBy::create(0.2f, Point(0, 30));
						runAction(action_1);
					}
					else
						return true;
				}
				else
				{
					MoveBy *action_1 = MoveBy::create(0.2f, Point(0, 30));
					runAction(action_1);
				}
			}
			selected = !selected;
		}
		return true;
	}
	return false;
}

void CardSprite::onTouchedFromParent()
{
	if(visible && selected)
	{
		MoveBy *action_1 = MoveBy::create(0.2f, Point(0, -30));
		runAction(action_1);
		selected = !selected;
	}
}

void CardSprite::setParent(PlayerObject *parent, int index)
{
	parentObject = parent;
	cardIndex = index;
}

void CardSprite::setDim(bool dim)
{
	upCard->setColor(dim ? ccc3(180, 180, 180) : ccc3(255, 255, 255));
}
