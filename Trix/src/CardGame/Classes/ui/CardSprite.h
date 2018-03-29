#ifndef _CARD_SPRITE_H_
#define _CARD_SPRITE_H_
#include "cocos2d.h"
#include "common.h"

USING_NS_CC;
class PlayerObject;

class CardSprite : public cocos2d::Layer
{
public:
    virtual bool init();

    void setCard(const char *filename, Suit suit, int value);
    void setVisible(bool value);

    void initAll();
    bool onTouched(Touch *touch, Event *event);
    void onTouchedFromParent();
    void setParent(PlayerObject *parent, int index);
    void setEnabled(int enable);
    void setDim(bool dim);
    bool enabled;

    void setEnableTouch(bool enable);
    cocos2d::Size visibleSize;
    cocos2d::Point origin;
    cocos2d::Point center;
    cocos2d::Sprite *upCard;
    cocos2d::Sprite *backCard;

    PlayerObject *parentObject;
    Card card;

    bool visible;
    bool selected;
    int cardIndex;
    // implement the "static create()" method manually
    CREATE_FUNC(CardSprite);
};

#endif /* _CARD_SPRITE_H_ */
