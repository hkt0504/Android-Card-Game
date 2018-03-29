#ifndef _CARD_DECK_H_
#define _CARD_DECK_H_

#include "cocos2d.h"
#include "common.h"

class CardDeck : public cocos2d::Node
{
    Card cards[CARD_NUM];                              // defines a deck

public:
    CardDeck();
    virtual ~CardDeck() {};

    virtual bool init();

    void shuffle();

    Suit cardSuit(int i);
    int cardValue(int i);

    CREATE_FUNC(CardDeck);
};

#endif //_CARD_DECK_H_
