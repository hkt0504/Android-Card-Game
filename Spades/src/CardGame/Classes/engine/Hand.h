#ifndef _HAND_H_
#define _HAND_H_

#include "cocos2d.h"
#include "common.h"

class CardDeck;

class Hand : public cocos2d::Node
{
	Card cards[PLAYER_CARD_NUM+1];                            // defines a hand (1 space in end for overshooting index)
public:
	Hand() {}

	virtual bool init();
	void setHand(const Hand *hand);

	Suit cardSuit(int i);
	int cardValue(int i);
	int cardIndex(int i);
	int getCardIndex(Card card);

	void setHand(CardDeck *deck, int index);
	void organizeHand();

	void applyRule(Suit suit_led, bool spades_broken);

	int findCard(Suit suit, int value);
	int countSuit(Suit suit);
	int lowClover();
	bool hasSuit(Suit suit);

	int highestCard(Suit suit);
	int lowestCard(Suit suit);
	int nearestCard(Suit suit, int value);
	int highCard(Suit suit, int value);

	void removeCard(Suit suit, int value);
	int bid();

	CREATE_FUNC(Hand);
};

#endif //_HAND_H_
