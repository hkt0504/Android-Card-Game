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

void Hand::applyRule(Suit suitLed)
{
	// leading allow anything
	if (suitLed == BLANK) {
		return;
	}

	// If void in suit led, can play anything.
	// If have suit led, limit to that suit
	if (hasSuit(suitLed)) {
		for (int i = 0; i < PLAYER_CARD_NUM; i++) {
			if (cards[i].suit != suitLed) {
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
	return INVALID;
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

bool Hand::hasSuit(Suit suit)
{
	for (int i = 0; i < PLAYER_CARD_NUM; i++) {
		if (cards[i].suit == suit) {
			return true;
		}
	}
	return false;
}

// Returns highest card regardless of suit in hand
bool Hand::highestCardOfAll(Suit* suit, int* value)
{
	Suit s = BLANK;
	int v = CARD_BLANK;

	for ( int i = 0; i < PLAYER_CARD_NUM; i++) {
		if (cards[i].suit != BLANK && cards[i].value > v) {
			s = cards[i].suit;
			v = cards[i].value;
		}
	}

	if (s != BLANK){
		*value = v;
		*suit = s;
		return true;
	}

	return false;
}

// Returns highest card of certain suit in hand
int Hand::highestCard(Suit suit)
{
	for (int i = CARD_A; i >= CARD_2; i--) {
		if (findCard(suit, i) != INVALID) {
			return i;
		}
	}
	return 0;
}

// Returns lowest card of certain suit in hand
int Hand::lowestCard(Suit suit)
{
	for (int i = CARD_2; i <= CARD_A; i++) {
		if (findCard(suit, i) != INVALID) {
			return i;
		}
	}
	return 0;
}

// Returns highest from small cards than card of certain (suit, value) in hand
// if no exist, returns nearest card large than certain card.
int Hand::nearestCard(Suit suit, int value)
{
	for (int i = value - 1; i >= CARD_2; i--) {
		if (findCard(suit, i) != INVALID) {
			return i;
		}
	}

	for (int i = value + 1; i <= CARD_A; i++) {
		if (findCard(suit, i) != INVALID) {
			return i;
		}
	}

	return 0;
}

// return lowest of high cards
int Hand::highCard(Suit suit, int value)
{
	for (int i = value + 1; i <= CARD_A; i++) {
		if (findCard(suit, i) != INVALID) {
			return i;
		}
	}

	return 0;
}

// return highest of low cards
int Hand::lowCard(Suit suit, int value)
{
	for (int i = value - 1; i >= CARD_2; i--) {
		if (findCard(suit, i) != INVALID) {
			return i;
		}
	}

	return 0;
}

int Hand::lowCardExcept(Suit suit, int value, int except)
{
	for (int i = value - 1; i >= CARD_2; i--) {
		if (findCard(suit, i) != INVALID && i != except) {
			return i;
		}
	}

	return 0;
}

int Hand::highCardExcept(Suit suit, int value, int except)
{
	for (int i = value + 1; i <= CARD_A; i++) {
		if (findCard(suit, i) != INVALID && i != except) {
			return i;
		}
	}

	return except;
}

int Hand::highestCardExcept(Suit suit, int value)
{
	for (int i = CARD_A; i >= CARD_2; i--) {
		if (findCard(suit, i) != INVALID && i != value) {
			return i;
		}
	}

	return value;
}

// Removes desired card and sets new last element to zero
void Hand::removeCard(Suit suit, int value)
{
	int index = findCard(suit, value);
	cards[index].suit = BLANK;
	cards[index].value = 0;
}

bool Hand::empty()
{
	for (int i = 0; i < PLAYER_CARD_NUM; i++) {
		if (cards[i].suit != BLANK) {
			return false;
		}
	}

	return true;
}

int Hand::countLow(int value)
{
	int count = 0;
	for (int i = 0; i < PLAYER_CARD_NUM; i++) {
		if (cards[i].suit != BLANK && cards[i].value <= value) {
			count ++;
		}
	}
	return count;
}

int Hand::countHigh(int value)
{
	int count = 0;
	for (int i = 0; i < PLAYER_CARD_NUM; i++) {
		if (cards[i].suit != BLANK && cards[i].value >= value) {
			count ++;
		}
	}
	return count;
}

int Hand::countEqual(int value)
{
	int count = 0;
	for (int i = 0; i < PLAYER_CARD_NUM; i++) {
		if (cards[i].suit != BLANK && cards[i].value == value) {
			count ++;
		}
	}
	return count;
}
