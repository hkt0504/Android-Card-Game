#include "TrixLevel.h"
#include "Hand.h"
#include "Player.h"

USING_NS_CC;

TrixLevel::TrixLevel()
{
	ranking = 0;
	card2[0] = INVALID;
	card2[1] = INVALID;
	card2[2] = INVALID;
	card2[3] = INVALID;
}

TrixLevel::~TrixLevel()
{
}

bool TrixLevel::evaluateScore(int turn)
{
	if (table->cardCount > 0) {
		Card card = table->playedCards[table->cardCount - 1];
		if (card.value == CARD_2) {
			int dups[4] = {DUP_H_2, DUP_C_2, DUP_D_2, DUP_S_2};
			REMOVE_BIT(players[turn]->duplicates, dups[suit_index(card.suit)]);
		}
	}

	if (players[turn]->hand->empty()) {
		players[turn]->score += (200 - ranking * 50);
		ranking ++;
	}

	return (ranking >= 4);
}

bool TrixLevel::thinkDuplicate(int player, OUT int& duplicates)
{
	Level::thinkDuplicate(player, duplicates);

	bool result = false;
	Hand *hand = players[player]->hand;

	if (hand->findCard(HEART, CARD_2) != INVALID) {
		SET_BIT(duplicates, DUP_H_2);
		result = true;
	}

	if (hand->findCard(CLOVER, CARD_2) != INVALID) {
		SET_BIT(duplicates, DUP_C_2);
		result = true;
	}

	if (hand->findCard(DIAMOND, CARD_2) != INVALID) {
		SET_BIT(duplicates, DUP_D_2);
		result = true;
	}

	if (hand->findCard(SPADE, CARD_2) != INVALID) {
		SET_BIT(duplicates, DUP_S_2);
		result = true;
	}

	return result;
}

void TrixLevel::setDuplicate(int player, int duplicates)
{
	int flags[4] = { DUP_H_2, DUP_C_2, DUP_D_2, DUP_S_2 };
	for (int i = 0; i < 4; i++) {
		if (HAS_BIT(duplicates, flags[i])) {
			card2[i] = player;
		}
	}
}

Card TrixLevel::thinkCard(Hand* hand, int turn)
{
	CCLog("TrixLevel::thinkCard");

	TrixTable *trixTable = (TrixTable *)table;
	Card card;

	Suit suits[4] = {SPADE, CLOVER, DIAMOND, HEART};
	Suit sortedSuits[4] = {BLANK, };

	int idx1 = 0, idx2 = 3;
	int team = (turn + 2) % 4;
	int player = partner ? team : turn;
	for (int i = 0; i < 4; i++) {
		if (card2[i] != INVALID && card2[i] == player) {
			sortedSuits[idx1] = suits[i];
			idx1++;
		} else {
			sortedSuits[idx2] = suits[i];
			idx2--;
		}
	}

	for (int i = 0; i < 4; i++) {
		Suit suit = sortedSuits[i];
		if (hand->hasSuit(suit)) {
			if (hand->findCard(suit, CARD_J) != INVALID) {
				card.set(suit, CARD_J);
				return card;
			}
		}
	}

	for (int i = 0; i < idx1; i++) {
		Suit suit = sortedSuits[i];
		if (hand->hasSuit(suit)) {
			if (i < idx1) {
				int value = trixTable->minCard(suit);
				if (value != INVALID && value > CARD_2 && hand->findCard(suit, value - 1) != INVALID) {
					card.set(suit, value - 1);
					return card;
				}
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		Suit suit = sortedSuits[i];
		if (hand->hasSuit(suit)) {
			int value = trixTable->maxCard(suit);
			if (value != INVALID && value < CARD_A && hand->findCard(suit, value + 1) != INVALID) {
				card.set(suit, value + 1);
				return card;
			}

			value = trixTable->minCard(suit);
			if (value != INVALID && value > CARD_2 && hand->findCard(suit, value - 1) != INVALID) {
				card.set(suit, value - 1);
				return card;
			}
		}
	}

	return card;
}

void TrixLevel::getEnabledCards(Hand *hand, Card cards[], int &count)
{
	Suit suits[4] = {HEART, DIAMOND, CLOVER, SPADE};

	count = 0;
	for (int i = 0; i < 4; i++) {
		Suit suit = suits[i];
		if (hand->hasSuit(suit)) {
			if (hand->findCard(suit, CARD_J) != INVALID) {
				cards[count].suit = suit;
				cards[count].value = CARD_J;
				count ++;
			}

			int value = table->minCard(suit);
			if (value != INVALID && value > CARD_2 && hand->findCard(suit, value - 1) != INVALID) {
				cards[count].suit = suit;
				cards[count].value = value -1;
				count ++;
			}

			value = table->maxCard(suit);
			if (value != INVALID && value < CARD_A && hand->findCard(suit, value + 1) != INVALID) {
				cards[count].suit = suit;
				cards[count].value = value + 1;
				count ++;
			}
		}
	}
}
