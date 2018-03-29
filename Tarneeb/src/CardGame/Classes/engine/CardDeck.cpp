#include "CardDeck.h"
#include "cocos2d.h"

USING_NS_CC;

CardDeck::CardDeck()
{
	int i;

	for (i = 0; i < 13; i++) {
		cards[i].suit = HEART;
		cards[i].value = i + 2;
	}

	for (i = 13; i < 26; i++) {
		cards[i].suit = CLOVER;
		cards[i].value = i - 11;
	}

	for (i = 26; i < 39; i++) {
		cards[i].suit = DIAMOND;
		cards[i].value = i - 24;
	}

	for (i = 39; i < 52; i++) {
		cards[i].suit = SPADE;
		cards[i].value = i - 37;
	}
}

void CardDeck::shuffle()
{
	CCLog("Call start CardDeck::shuffle()");
	int random1, random2;

	srand(time(0));

	for (int i = 0; i < 100; i++) {
		random1 = rand() % CARD_NUM;
		random2 = rand() % CARD_NUM;

		swap_card(cards[random1], cards[random2]);
	}
	CCLog("Call end CardDeck::shuffle()");
}

Suit CardDeck::cardSuit(int i)
{
	return cards[i].suit;
}

int CardDeck::cardValue(int i)
{
	return cards[i].value;
}
