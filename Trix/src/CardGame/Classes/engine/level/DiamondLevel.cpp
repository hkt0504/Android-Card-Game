#include "DiamondLevel.h"
#include "Hand.h"
#include "Player.h"

USING_NS_CC;

DiamondLevel::DiamondLevel()
{
	playedDiamonds = 0;
}

DiamondLevel::~DiamondLevel()
{

}

bool DiamondLevel::evaluateScore(int turn)
{
	CCLog("Call DiamondLevel::evaluateScore");

	Card card = table->playedCards[table->closedCardCount];
	Suit suitLed = card.suit;
	int highest_index = 0;
	int highest_value = 0;
	int diamond = 0;

	for (int i = 0; i < 4; i++) {
		card = table->playedCards[i + table->closedCardCount];
		if (card.suit == DIAMOND) {
			diamond ++;
		}

		if (card.suit == suitLed && card.value > highest_value) {
			highest_value = card.value;
			highest_index = i;
		}
	}

	if (diamond > 0) { //diamond is played
		int winner = (turn + highest_index) % PLAYER_NUM;
		players[winner]->score -= diamond * SCORE_DIAMOND;
		players[winner]->eatDiamond += diamond;

		playedDiamonds += diamond;
		if (playedDiamonds == SUIT_CARDS) {
			return true;
		}
	}

	return false;
}

Card DiamondLevel::thinkCardInSuit(Hand *hand, Suit suit, int turn)
{
	CCLog("Call DiamondLevel::thinkCardInSuit");

	Card card;
	card.suit = suit;

	// last play
	if (table->isLastPlayerInTrick()) {
		if (partner && table->isTeamHighest(suit)) {
			card.value = hand->highestCard(suit);
			return card;
		}

		if (suit != DIAMOND && !table->findSuitInTrick(DIAMOND)) {
			card.value = hand->highestCard(suit);
			return card;
		}
	}

	int highestOnTable = table->highestCardInTrick(suit);
	card.value = nearestCard(hand, suit, highestOnTable);

	return card;
}

Card DiamondLevel::thinkCardOutSuit(Hand *hand, Suit suit, int turn)
{
	CCLog("Call DiamondLevel::thinkCardOutSuit");

	Card card;

	// team is highest played
	if (partner && table->isTeamHighest(suit)) {
		return highestCardExcept(hand, suit);
	}

	// If I have Diamond card
	if (hand->hasSuit(DIAMOND)) {
		card.suit = DIAMOND;
		card.value = hand->highestCard(DIAMOND);
		return card;
	} else {
		int v = CARD_BLANK;
		Suit s = BLANK;

		hand->highestCardOfAll(&s, &v);
		card.suit = s;
		card.value = v;
		return card;
	}
}

Card DiamondLevel::thinkCardFirst(Hand *hand, int turn)
{
	CCLog("Call DiamondLevel::thinkCardFirst");

	Card card;

	// i have lowest diamond and play it
	if (hand->hasSuit(DIAMOND) && !table->isFinishedSuit(hand, DIAMOND)) {
		for (int i = CARD_2; i <= CARD_A; i++) {
			if (hand->findCard(DIAMOND, i) != INVALID) {
				if (table->isLowestCard(DIAMOND, i) || i < CARD_4) {
					card.suit = DIAMOND;
					card.value = i;
					return card;
				}
				break;
			}
		}
	}

	Suit suits[3] = {HEART, CLOVER, SPADE};
	for (int i = 0; i < 3; i++) {
		int count = hand->countSuit(suits[i]);
		if (count > 0 && count < 3 && !table->isFinishedSuit(hand, suits[i])) {
			int value = hand->lowestCard(suits[i]);
			if (!table->isHighestCard(suits[i], value)) {
				card.suit = suits[i];
				card.value = value;
				return card;
			}
		}
	}

	if (hand->hasSuit(HEART) || hand->hasSuit(CLOVER) || hand->hasSuit(SPADE)) {
		card.suit = BLANK;
		card.value = CARD_MAX;

		// get lowest card from my hand
		for (int i = 0; i < PLAYER_CARD_NUM; i++) {
			if (hand->cardSuit(i) != BLANK && hand->cardValue(i) < card.value && hand->cardSuit(i) != DIAMOND) {
				card.suit = hand->cardSuit(i);
				card.value = hand->cardValue(i);
			}
		}

		return card;
	} else {
		lowestCardOfAll(hand, &card);
		return card;
	}

}

Card DiamondLevel::highestCardExcept(Hand *hand, Suit suit)
{
	Card ret;
	ret.suit = BLANK;
	ret.value = CARD_BLANK;

	for ( int i = 0; i < PLAYER_CARD_NUM; i++) {
		Card card = hand->card(i);
		if (card.suit != BLANK && card.value > ret.value && !card.isD()) {
			ret = card;
		}
	}

	if (ret.suit == BLANK && suit != DIAMOND) {
		int value = hand->highestCard(DIAMOND);
		if (value >= CARD_2) {
			ret.suit = DIAMOND;
			ret.value = value;
			return ret;
		}
	}

	return ret;
}

int DiamondLevel::nearestCard(Hand *hand, Suit suit, int highestOnTable)
{
	int value = hand->lowCard(suit, highestOnTable);
	if (value == 0) { // if i haven't low card
		if (table->isLastPlayerInTrick()) { // last play
			value = hand->highestCard(suit);
		} else {
			value = hand->highCard(suit, highestOnTable);
		}
	}
	return value;
}
