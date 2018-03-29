#include "Level.h"
#include "cocos2d.h"
#include "Hand.h"
#include "Table.h"
#include "Player.h"

USING_NS_CC;

Level::Level()
{
	suitLed = BLANK;
	table = NULL;
	for (int i = 0; i < PLAYER_NUM; i++) {
		this->players[i] = NULL;
	}
}

Level::~Level()
{
}

void Level::setPlayers(Player *players[])
{
	for (int i = 0; i < PLAYER_NUM; i++) {
		this->players[i] = players[i];
	}
}

Card Level::thinkCard(Hand *hand, int turn, Suit tarneeb)
{
	CCLog("Call Level::thinkCard");

	switch (suitLed) {
	case SPADE:
	case DIAMOND:
	case CLOVER:
	case HEART:
		if (hand->hasSuit(suitLed)) {
			return thinkCardInSuit(hand, turn, tarneeb);
		} else {
			return thinkCardOutSuit(hand, turn, tarneeb);
		}
		break;

	default:
		// where suit is BLANK
		return thinkFirstCard(hand, turn, tarneeb);
	}
}

// we called this method to play first card
Card Level::thinkFirstCard(Hand *hand, int turn, Suit tarneeb)
{
	Card card;
	// player team bid
	int partner = (turn + 2) % PLAYER_NUM;
	if (players[turn]->bid > 0 || players[partner]->bid > 0) {
		// player team has tarneeb
		if (hand->hasSuit(tarneeb)) {
			// if the player has only tarneeb
			if (!hand->hasOtherSuit(tarneeb)) {
				card.suit = tarneeb;
				card.value = hand->highestCard(tarneeb);
				suitLed = tarneeb;
				return card;
			} else {
				// other team has tarneeb
				if (isOtherTeamHasSuit(tarneeb, turn)) {
					card.suit = tarneeb;
					card.value = hand->highestCard(tarneeb);
					suitLed = tarneeb;
					return card;
				}
			}
		}
	}

	card = thinkHighestOrLowestCard(hand, tarneeb);
	suitLed = card.suit;
	return card;
}

Card Level::thinkCardInSuit(Hand *hand, int turn, Suit tarneeb)
{
	Card card;
	card.suit = suitLed;

	int highestIndex = table->highestCardIndexInTrick(suitLed);
	if (highestIndex == -1) {
		// never called, only for no exception
		card.value = hand->highestCard(suitLed);
		return card;
	}

	// if already played tarneeb, play lowest card
	Card highestCard = table->getTrickCard(highestIndex);
	if (highestCard.suit == tarneeb && suitLed != tarneeb) {
		card.value = hand->lowestCard(suitLed);
		return card;
	}

	// if last play
	int round = table->currentOfTrick();
	if (round == 3) {
		// if team is win, play lowest card
		if (highestIndex == 1) {
			card.value = hand->lowestCard(suitLed);
			return card;
		}

		int highValue = hand->highCard(suitLed, highestCard.value);
		if (highValue > 0) {
			card.value = highValue;
			return card;
		}
	} else if (round == 2) {
		// if team is highest, play lowest card
		if (highestIndex == 0 && table->isHighestCard(suitLed, highestCard.value)) {
			card.value = hand->lowestCard(suitLed);
			return card;
		}
	}

	// if have highest card, play it
	if (table->isHighestCard(suitLed, hand->highestCard(suitLed))) {
		card.value = hand->highestCard(suitLed);
		return card;
	} else {
		card.value = hand->lowestCard(suitLed);
		return card;
	}
}

Card Level::thinkCardOutSuit(Hand *hand, int turn, Suit tarneeb)
{
	Card card;
	players[turn]->setEmptySuit(suitLed);

	// if led is tarneeb, play lowest card
	if (suitLed == tarneeb) {
		card.suit = hand->longestSuit(tarneeb);
		card.value = hand->lowestCard(card.suit);
		return card;
	}

	bool hasTarneeb = hand->hasSuit(tarneeb);

	int highestIndex = table->highestCardIndexInTrick(suitLed);
	if (highestIndex == -1) {
		// never called, only for no exception
		card.suit = hand->longestSuit(tarneeb);
		card.value = hand->lowestCard(card.suit);
		return card;
	}

	int round = table->currentOfTrick();

	// if team win, play lowest card
	if (highestIndex == round - 2) {
		card.suit = hand->longestSuit(tarneeb);
		card.value = hand->lowestCard(card.suit);
		return card;
	}

	// if has tarneeb, play lowest tarneeb
	if (hasTarneeb) {
		Card highestCard = table->getTrickCard(highestIndex);

		// if last play
		if (round == 4) {
			if (highestCard.suit == tarneeb) {
				// if no have high tarneeb, play lowest other card
				int highValue = hand->highCard(tarneeb, highestCard.value);
				if (highValue > 0) {
					card.suit = tarneeb;
					card.value = highValue;
					return card;
				}
			} else {
				card.suit = tarneeb;
				card.value = hand->lowestCard(tarneeb);
				return card;
			}
		} else {
			if (table->isFinishedSuit(hand, tarneeb)) {
				if (highestCard.suit == tarneeb) {
					// if no have high tarneeb, play lowest other card
					int highValue = hand->highCard(tarneeb, highestCard.value);
					if (highValue > 0) {
						card.suit = tarneeb;
						card.value = highValue;
						return card;
					}
				} else {
					card.suit = tarneeb;
					card.value = hand->lowestCard(tarneeb);
					return card;
				}
			}

			int other1 = (turn + 1) % PLAYER_NUM;
			int other2 = (turn + 3) % PLAYER_NUM;
			if ((!players[other1]->played && players[other1]->isEmptySuit(suitLed) && !players[other1]->isEmptySuit(tarneeb)) ||
				(!players[other2]->played && players[other2]->isEmptySuit(suitLed) && !players[other2]->isEmptySuit(tarneeb))) {
				card.suit = tarneeb;
				card.value = hand->highestCard(tarneeb);
				return card;
			} else {
				card.suit = tarneeb;
				card.value = hand->lowestCard(tarneeb);
				return card;
			}
		}
	}

	card.suit = hand->longestSuit(tarneeb);
	card.value = hand->lowestCard(card.suit);
	return card;
}

// select best card or lowest card
Card Level::thinkHighestOrLowestCard(Hand *hand, Suit tarneeb)
{
	Card card;
	Suit suits[4] = { HEART, CLOVER, DIAMOND, SPADE };

	for (int i = 0; i < 4; i++) {
		if (suits[i] != tarneeb && hand->hasSuit(suits[i])) {
			int value = hand->highestCard(suits[i]);
			if (table->isHighestCard(suits[i], value)) {
				card.suit = suits[i];
				card.value = value;
				return card;
			}
		}
	}

	Suit longestSuit = hand->longestSuit(tarneeb);
	card.suit = longestSuit;
	card.value = hand->lowestCard(longestSuit);
	return card;
}

bool Level::isOtherTeamHasSuit(Suit suit, int turn)
{
	int first = (turn + 1) % PLAYER_NUM;
	int second = (turn + 3) % PLAYER_NUM;

	if (players[first]->isEmptySuit(suit) && players[second]->isEmptySuit(suit)) {
		return false;
	}

	if (table->isFinishedSuit(players[turn]->hand, suit))
		return false;

	return true;
}
