#include "TrixTable.h"

TrixTable::TrixTable()
{
	cardCount = 0;

	// setup the cards played
	for (int i = 0; i < CARD_NUM; i++) {
		playedCards[i].suit = BLANK;
		playedCards[i].value = 0;
	}
}

void TrixTable::addCardPlayed(Suit suit, int value)
{
	playedCards[cardCount].suit = suit;
	playedCards[cardCount].value = value;
	cardCount ++;
}

int TrixTable::minCard(Suit suit)
{
	int minValue = CARD_MAX;

	for (int i = 0; i < cardCount; i++) {

		if (playedCards[i].suit == suit) {
			int value = playedCards[i].value;
			if (value < minValue) {
				minValue = value;
			}
		}
	}

	return (minValue == CARD_MAX) ? INVALID : minValue;
}

int TrixTable::maxCard(Suit suit)
{
	int highValue = CARD_BLANK;

	for (int i = 0; i < cardCount; i++) {

		if (playedCards[i].suit == suit) {
			int value = playedCards[i].value;
			if (value > highValue) {
				highValue = value;
			}
		}
	}

	return (highValue == CARD_BLANK) ? INVALID : highValue;
}
