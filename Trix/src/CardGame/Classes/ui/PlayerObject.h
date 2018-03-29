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

    void clear();
    void stopAllAnimations();
    bool onTouchedCard(CardSprite *card, bool select);
    void onTouchedTable();
    void setSelectedCard(int index);
    void setMove(int value);

    void setParent(GameScene *object);
    void setAvatarIndex(int index);
    void setPlayerIndex(int index);
    void drawCards();
    void setScore(int score);
    void setEnabled(bool enable);
    String getPlayerName();
    void setUserInfo(const char* name, const char* avatar);

    void drawAvatar();
    void endMove(cocos2d::Node *pSender, cocos2d::Object *obj, int playerIndex, int index, float x, float y);
    void endPlayMove(Node *pSender, Object *obj, int playerIndex);
    void endShowScore(Node *pSender, Object *obj);
    void setEnableTouchCards(int suitType, int enable);
    void setEnableTouchCards(Card enableds[], int count);

    void setHand(cocos2d::Vector<CardSprite*> *cardList, Hand *hand);
    void refreshUserCards();

    void showCard(cocos2d::Node* pSender);

    void showPass(bool show);
    void showCurrent(bool show);
    void showDuplicate(int duplicates);

    // implement the "static create()" method manually
    CREATE_FUNC(PlayerObject);

private:
    void setLeftCount(int count);
    Point getPlayedCardPosition();
    Point getPlayedCardPositionForTrix();

    Sprite *dupSprs[9];
};

#endif /* _CARDGAME_PLAYER_OBJECT_H_ */
