#ifndef _CARDGAME_PLAYER_OBJECT_H_
#define _CARDGAME_PLAYER_OBJECT_H_
#include "cocos2d.h"
#include "common.h"

class CardSprite;
class CardDeck;
class Hand;
class GameScene;

USING_NS_CC;

class PlayerObject : public cocos2d::Layer
{
public:
	PlayerObject();
	virtual ~PlayerObject();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone

    virtual bool init();

    cocos2d::Size visibleSize;
    cocos2d::Size layerSize;
    cocos2d::Point origin;
    cocos2d::Point center;

    CardSprite *cards[13];
    int playerIndex;
    int leftCount;
    int avatarIndex;
    CardSprite* selectedCard;
    GameScene *parentObject;

    void stopAllAnimations();
    bool onTouchedCard(CardSprite *card, bool select);
    void onTouchedTable();
    void setSelectedCard(int index);
    void setBid(int value);
    void removeBid();

    void setParent(GameScene *object);
    void setAvatarIndex(int index);
    void setPlayerIndex(int index);
    void drawCards();
    void setScore(int bid, int score);
    void setEnabled(bool enable);
    String getPlayerName();
    void setUserInfo(const char* name, const char* avatar);
    void showCards();

    void drawAvatar();
    void endMove(cocos2d::Node *pSender, cocos2d::Object *obj, int playerIndex, int index, float x, float y);
    void endPlayMove(Node *pSender, Object *obj, int playerIndex);
    void setEnableTouchCards(int suitType, int enable);

    void setHand(cocos2d::Vector<CardSprite*> *cardList, Hand *hand);
    void refreshUserCards();
    // implement the "static create()" method manually
    CREATE_FUNC(PlayerObject);

};

#endif /* _CARDGAME_PLAYER_OBJECT_H_ */
