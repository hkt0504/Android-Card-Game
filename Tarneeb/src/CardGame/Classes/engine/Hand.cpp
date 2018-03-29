#include "cocos2d.h"
#include "Hand.h"
#include "CardDeck.h"


USING_NS_CC;


void Hand::setHand(const Hand *hand)
{
	for (int i = 0; i < PLAYER_CARD_NUM; i++) {
		cards[i] = hand->cards[i];
	}
}

Suit Hand::cardSuit(int i)
{
	return cards[i].suit;
}

int Hand::cardValue(int i)
{
	return cards[i].value;
}

int Hand::cardIndex(int i)
{
	return get_card_index(cards[i]);
}

int Hand::getCardIndex(Card card)
{
	for (int i = 0; i < PLAYER_CARD_NUM; i++) {
		if (cards[i].suit == card.suit && cards[i].value == card.value) {
			return i;
		}
	}

	return -1;
}

void Hand::setHand(CardDeck *deck, int index)
{
	for (int i = 0; i < 13; i++) {
		cards[i].suit = deck->cardSuit(index);
		cards[i].value = deck->cardValue(index);
		index += 4;
	}
	cards[13].suit = BLANK;
	cards[13].value = 0;
	CCLog("Call Hand::setHand()");
}

void Hand::organizeHand()
{
	int i, stop = 0;

	while (!stop) {
		stop = true;
		for (i = 0; i < 12; i++) {
			if (calc_card_value(cards[i]) > calc_card_value(cards[i + 1])) {
				swap_card(cards[i], cards[i + 1]);
				stop = false;
			}
		}
	}
	CCLog("Call Hand::organizeHand()");
}

void Hand::applyRule(Suit suit_led)
{
	// If leading allow anything but spades until broken (in exception of nothing else to lead, then allow spades)
	if (suit_led == BLANK) {
		return;
	}

	// If void in suit led, can play anything.
	// If have suit led, limit to that suit
	if (hasSuit(suit_led)) {
		for (int i = 0; i < PLAYER_CARD_NUM; i++) {
			if (cards[i].suit != suit_led) {
				cards[i].suit = BLANK;
				cards[i].value = 0;
			}
		}
	}
}

// Find position of a card
// Return -1 if not found
int Hand::findCard(Suit suit, int value)
{
	for ( int i = 0; i < PLAYER_CARD_NUM; i++) {
		if (cards[i].suit == suit && cards[i].value == value) {
			return i;
		}
	}
	return -1;
}

// Returns how many cards of a certain suit
int Hand::countSuit(Suit suit)
{
	int suit_count = 0, i;

	for (i = 0; i < PLAYER_CARD_NUM; i++) {
		if (cards[i].suit == suit) {
			suit_count++;
		}
	}
	return suit_count;
}

// Return value of lowest clover or -1 if no clover
int Hand::lowClover()
{
	for (int i = 0; i < PLAYER_CARD_NUM; i++) {
		if (cards[i].suit == CLOVER) {
			return cards[i].value;
		}
	}
	return -1;
}

bool Hand::hasSuit(Suit suit)
{
	for (int i = 0; i < PLAYER_CARD_NUM; i++) {
		if (cards[i].suit == suit) {
			return true;
		}
	}
	return false;
}

bool Hand::hasOtherSuit(Suit suit)
{
	for (int i = 0; i < PLAYER_CARD_NUM; i++) {
		if (cards[i].suit != BLANK && cards[i].suit != suit) {
			return true;
		}
	}
	return false;
}

// Returns highest card of certain suit in hand
int Hand::highestCard(Suit suit)
{
	for (int i = 14; i >= 2; i--) {
		if (findCard(suit, i) != -1) {
			return i;
		}
	}
	return 0;
}

// Returns lowest card of certain suit in hand
int Hand::lowestCard(Suit suit)
{
	for (int i = 2; i <= 14; i++) {
		if (findCard(suit, i) != -1) {
			return i;
		}
	}
	return 0;
}

// Returns highest card small than card of certain (suit, value) in hand
// if no exist, returns nearest card large than certain card.
int Hand::nearestCard(Suit suit, int value)
{
	for (int i = value - 1; i >= 2; i--) {
		if (findCard(suit, i) != -1) {
			return i;
		}
	}

	for (int i = value + 1; i <= 14; i++) {
		if (findCard(suit, i) != -1) {
			return i;
		}
	}

	return 0;
}

int Hand::highCard(Suit suit, int value)
{
	for (int i = value + 1; i <= 14; i++) {
		if (findCard(suit, i) != -1) {
			return i;
		}
	}

	return 0;
}

// get longest type
Suit Hand::longestSuit(Suit tarneeb)
{
	Suit result = BLANK;

	int max = 0;
	int clubCount = 0;
	int heartCount = 0;
	int spadeCount = 0;
	int diamonCount = 0;

	for (int i = 0; i < PLAYER_CARD_NUM; i++) {
		switch (cards[i].suit) {
		case CLOVER:
			clubCount++;
			break;
		case DIAMOND:
			diamonCount++;
			break;
		case SPADE:
			spadeCount++;
			break;
		case HEART:
			heartCount++;
			break;
		default:
			break;
		}
	}

	if (tarneeb == BLANK) {
		if (max < spadeCount) {
			max = spadeCount;
			result = SPADE;
		}

		if (max < diamonCount) {
			max = diamonCount;
			result = DIAMOND;
		}

		if (max < clubCount) {
			max = clubCount;
			result = CLOVER;
		}

		if (max < heartCount) {
			max = heartCount;
			result = HEART;
		}
	} else {
		if (max < spadeCount && tarneeb != SPADE) {
			max = spadeCount;
			result = SPADE;
		}

		if (max < diamonCount && tarneeb != DIAMOND) {
			max = diamonCount;
			result = DIAMOND;
		}

		if (max < clubCount && tarneeb != CLOVER) {
			max = clubCount;
			result = CLOVER;
		}

		if (max < heartCount && tarneeb != HEART) {
			max = heartCount;
			result = HEART;
		}
	}

	return (max == 0) ? tarneeb : result;
}

// Removes desired card and sets new last element to zero
void Hand::removeCard(Suit suit, int value)
{
	int index = findCard(suit, value);
	cards[index].suit = BLANK;
	cards[index].value = 0;
}
