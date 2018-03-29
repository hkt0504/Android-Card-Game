#include "common.h"
#include <stdlib.h>

const char* PROFILE_NAMES[14] = {
	"You", "Fady", "Ali", "Saber", "Hashem", "Alaa", "Khalid",
	"Haytham", "Maher", "Samer", "Joshep", "James", "Tomas", "Sufarin"
};

const char* PROFILE_NAMES_UP[14] = {
	"YOU", "FADY", "ALI", "SABER", "HASHEM", "ALAA", "KHALID",
	"HAYTHAM", "MAHER", "SAMER", "JOSHEP", "JAMES", "TOMAS", "SUFARIN"
};

float frand(int min, int max)
{
	float r3 = min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max - min)));

	return r3;
}

void swap_card(Card &a, Card &b)
{
	Suit temp_suit;
	int temp_value;

	temp_suit = a.suit;
	temp_value = a.value;

	a.suit = b.suit;
	a.value = b.value;

	b.suit = temp_suit;
	b.value = temp_value;
}

int calc_card_value(const Card &card)
{
	switch (card.suit) {
	case HEART:
		return card.value;
	case CLOVER:
		return PLAYER_CARD_NUM + card.value;
	case DIAMOND:
		return PLAYER_CARD_NUM * 2 + card.value;
	case SPADE:
		return PLAYER_CARD_NUM * 3 + card.value;
	default:
		return -1;
	}
}

int get_card_index(const Card &card)
{
	switch (card.suit) {
	case HEART:
		return card.value - 2;
	case CLOVER:
		return PLAYER_CARD_NUM + card.value - 2;
	case DIAMOND:
		return PLAYER_CARD_NUM * 2 + card.value - 2;
	case SPADE:
		return PLAYER_CARD_NUM * 3 + card.value - 2;
	default:
		return -1;
	}
}

int suit_index(Suit suit)
{
	switch (suit) {
	case HEART:
		return 0;
	case CLOVER:
		return 1;
	case DIAMOND:
		return 2;
	case SPADE:
		return 3;
	default:
		return 4;
	}
}

Suit get_suit(int index)
{
	switch (index) {
	case 0:
		return HEART;
	case 1:
		return CLOVER;
	case 2:
		return DIAMOND;
	case 3:
		return SPADE;
	default:
		return BLANK;
	}
}
