#include "KingLevel.h"
#include "Hand.h"
#include "Player.h"

USING_NS_CC;

KingLevel::KingLevel()
{
	duplicate = -1;
}

KingLevel::~KingLevel()
{

}

/** return is finished and calculate score */
bool KingLevel::evaluateScore(int turn)
{
	Card card = table->playedCards[table->closedCardCount];
	Suit suitLed = card.suit;
	int highest_index = 0;
	int highest_value = 0;
	int kingIndex = -1;

	for (int i = 0; i < 4; i++) {
		card = table->playedCards[i + table->closedCardCount];
		if (card.suit == HEART && card.value == CARD_K) {
			kingIndex = i;
		}

		if (card.suit == suitLed && card.value > highest_value) {
			highest_value = card.value;
			highest_index = i;
		}
	}

	if (kingIndex != -1) { //King is played
		int winner = (turn + highest_index) % PLAYER_NUM;

		players[winner]->score -= SCORE_KING;
		SET_BIT(players[winner]->eatKQ, DUP_K);

		if (duplicate != INVALID) {
			REMOVE_BIT(players[duplicate]->duplicates, DUP_K);
			if (winner != duplicate) {
				players[winner]->score -= SCORE_KING;
				players[duplicate]->score += SCORE_KING;
			}
		}
		return true;
	}

	return false;
}

bool KingLevel::getDuplicateList(int player, int& duplicates)
{
	Level::getDuplicateList(player, duplicates);
	Hand *hand = players[player]->hand;

	if (hand->findCard(HEART, CARD_K) != INVALID) {
		SET_BIT(duplicates, DUP_K);
		return true;
	}

	return false;
}

void KingLevel::setDuplicate(int player, int duplicates)
{
	if (HAS_BIT(duplicates, DUP_K)) {
		duplicate = player;
	}
}

bool KingLevel::thinkDuplicate(int player, int& duplicates)
{
	bool result = false;
	Level::thinkDuplicate(player, duplicates);
	Hand *hand = players[player]->hand;

	if (hand->findCard(HEART, CARD_K) != INVALID && hand->countSuit(HEART) >= 5) {
		SET_BIT(duplicates, DUP_K);
		result = true;
	}

	return result;
}

Card KingLevel::thinkCardInSuit(Hand *hand, Suit suit, int turn)
{
	CCLog("Call KingLevel::thinkCardInSuit");

	Card card;
	card.suit = suit;

	// last play
	if (table->isLastPlayerInTrick()) {
		if ((partner && table->isTeamHighest(suit)) || !table->findCardInTrick(HEART, CARD_K)) {
			if (suit == HEART) {
				card.value = hand->highestCardExcept(suit, CARD_K);
			} else {
				card.value = hand->highestCard(suit);
			}

			return card;
		}
	}

	int highestOnTable = table->highestCardInTrick(suit);
	card.value = nearestCard(hand, suit, highestOnTable);
	return card;
}

Card KingLevel::thinkCardOutSuit(Hand *hand, Suit suit, int turn)
{
	CCLog("Call KingLevel::thinkCardOutSuit");

	Card card;

	// team is highest played, do not play eat card.
	if (partner && table->isTeamHighest(suit)) {
		card = highestCardExcept(hand, suit);
		return card;
	}

	// if I have Heart_K, then play it.
	if (hand->findCard(HEART, CARD_K) != INVALID) {
		card.suit = HEART;
		card.value = CARD_K;
		return card;
	}

	// else play max on my hand
	int v = CARD_BLANK;
	Suit s = BLANK;

	hand->highestCardOfAll(&s, &v);
	card.suit = s;
	card.value = v;
	return card;
}

Card KingLevel::thinkCardFirst(Hand *hand, int turn)
{
	CCLog("Call KingLevel::thinkCardFirst");

	Card card;

	int exceptSuits = 0;
	int team = (turn + 2) % 4;
	int other1 = (turn + 1) % 4;
	int other2 = (turn + 3) % 4;

	// if HEART_K exist
	if (hand->findCard(HEART, CARD_K) != INVALID ||
		(partner && (team == duplicate || (players[other1]->isEmptySuit(HEART) && players[other2]->isEmptySuit(HEART))))) {
		Suit suits[3] = {DIAMOND, CLOVER, SPADE};
		for (int i = 0; i < 3; i++) {
			int count = hand->countSuit(suits[i]);
			if (count > 0 && count < 3){
				card.suit = suits[i];
				card.value = hand->lowestCard(suits[i]);
				return card;
			}
		}

		SET_BIT(exceptSuits, HEART);
	} else {
		if (hand->hasSuit(HEART)) {
			int value = hand->lowCard(HEART, CARD_K);
			if (value >= CARD_2) {
				card.suit = HEART;
				card.value = value;
				return card;
			}
		}
	}

	lowestCardOfAllExcept(hand, exceptSuits, &card);

	return card;
}

Card KingLevel::highestCardExcept(Hand *hand, Suit suit)
{
	Card ret;
	ret.suit = BLANK;
	ret.value = CARD_BLANK;

	for ( int i = 0; i < PLAYER_CARD_NUM; i++) {
		Card card = hand->card(i);
		if (card.suit != BLANK && card.value > ret.value && !card.isHK()) {
			ret = card;
		}
	}

	if (ret.suit == BLANK) {
		int v = CARD_BLANK;
		Suit s = BLANK;

		hand->highestCardOfAll(&s, &v);
		ret.suit = s;
		ret.value = v;
	}

	return ret;
}

int KingLevel::nearestCard(Hand *hand, Suit suit, int highestOnTable)
{
	int value = hand->lowCard(suit, highestOnTable);

	if (value == 0) { // if i haven't low card
		if (table->isLastPlayerInTrick()) { // last play
			if (suit == HEART)
				value = hand->highestCardExcept(suit, CARD_K);
			else
				value = hand->highestCard(suit);
		} else {
			if (suit == HEART)
				value = hand->highCardExcept(suit, highestOnTable, CARD_K);
			else
				value = hand->highCard(suit, highestOnTable);
		}
	}
	return value;
}

bool KingLevel::lowestCardOfAllExcept(Hand *hand, int exceptSuits, Card *card)
{
	if (exceptSuits == 0) {
		return lowestCardOfAll(hand, card);
	}

	card->suit = BLANK;
	card->value = CARD_MAX;

	for (int i = 0; i < PLAYER_CARD_NUM; i++) {
		Suit suit = hand->cardSuit(i);
		int value = hand->cardValue(i);
		if (suit != BLANK && HAS_BIT(exceptSuits, suit) == 0 && !table->isFinishedSuit(hand, suit) && value < card->value) {
			card->set(suit, value);
		}
	}

	if (card->suit != BLANK) {
		return true;
	}

	return lowestCardOfAll(hand, card);
}
