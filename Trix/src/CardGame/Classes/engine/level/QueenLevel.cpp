#include "QueenLevel.h"
#include "Hand.h"
#include "Player.h"


USING_NS_CC;

QueenLevel::QueenLevel()
{
	for (int i = 0; i < 4; i++) {
		duplicate[i] = INVALID;
	}

	queenSuit = 0;
}

QueenLevel::~QueenLevel()
{
}

/** return is finished and calculate score */
bool QueenLevel::evaluateScore(int turn)
{
	Card card = table->playedCards[table->closedCardCount];
	Suit suitLed = card.suit;
	int highest_index = 0;
	int highest_value = 0;
	int score = 0;
	int suit = 0;

	for (int i = 0; i < 4; i++) {
		card = table->playedCards[i + table->closedCardCount];
		if (card.value == CARD_Q) {
			SET_BIT(suit, card.suit);
			SET_BIT(queenSuit, card.suit);
		}

		if (card.suit == suitLed && card.value > highest_value) {
			highest_value = card.value;
			highest_index = i;
		}
	}

	if (suit != 0) { //queen is played
		int winner = (turn + highest_index) % PLAYER_NUM;
		Suit suits[4] = {HEART, CLOVER, DIAMOND, SPADE};
		int dups[4] = {DUP_H_Q, DUP_C_Q, DUP_D_Q, DUP_S_Q};

		for (int i = 0; i < 4; i++) {
			if (HAS_BIT(suit, suits[i])) {
				players[winner]->score -= SCORE_QUEEN;
				SET_BIT(players[winner]->eatKQ, dups[i]);

				if (duplicate[i] != INVALID) {
					REMOVE_BIT(players[duplicate[i]]->duplicates, dups[i]);
					if (winner != duplicate[i]) {
						players[winner]->score -= SCORE_QUEEN;
						players[duplicate[i]]->score += SCORE_QUEEN;
					}
				}
			}
		}

		if (queenSuit == SUIT_ALL) {
			return true;
		}
	}

	return false;
}

bool QueenLevel::getDuplicateList(int player, OUT int& duplicates)
{
	Level::getDuplicateList(player, duplicates);

	bool result = false;
	Hand *hand = players[player]->hand;

	if (hand->findCard(HEART, CARD_Q) != INVALID) {
		SET_BIT(duplicates, DUP_H_Q);
		result = true;
	}

	if (hand->findCard(CLOVER, CARD_Q) != INVALID) {
		SET_BIT(duplicates, DUP_C_Q);
		result = true;
	}

	if (hand->findCard(DIAMOND, CARD_Q) != INVALID) {
		SET_BIT(duplicates, DUP_D_Q);
		result = true;
	}

	if (hand->findCard(SPADE, CARD_Q) != INVALID) {
		SET_BIT(duplicates, DUP_S_Q);
		result = true;
	}

	return result;
}

void QueenLevel::setDuplicate(int player, int duplicates)
{
	int flags[4] = { DUP_H_Q, DUP_C_Q, DUP_D_Q, DUP_S_Q };
	for (int i = 0; i < 4; i++) {
		if (HAS_BIT(duplicates, flags[i])) {
			duplicate[i] = player;
		}
	}
}

bool QueenLevel::thinkDuplicate(int player, OUT int& duplicates)
{
	CCLog("Call QueenLevel::thinkDuplicate");

	Level::thinkDuplicate(player, duplicates);

	bool result = false;
	Hand *hand = players[player]->hand;

	if (hand->findCard(HEART, CARD_Q) != INVALID && hand->countSuit(HEART) >= 5) {
		SET_BIT(duplicates, DUP_H_Q);
		result = true;
	}

	if (hand->findCard(CLOVER, CARD_Q) != INVALID && hand->countSuit(CLOVER) >= 5) {
		SET_BIT(duplicates, DUP_C_Q);
		result = true;
	}

	if (hand->findCard(DIAMOND, CARD_Q) != INVALID && hand->countSuit(DIAMOND) >= 5) {
		SET_BIT(duplicates, DUP_D_Q);
		result = true;
	}

	if (hand->findCard(SPADE, CARD_Q) != INVALID && hand->countSuit(SPADE) >= 5) {
		SET_BIT(duplicates, DUP_S_Q);
		result = true;
	}

	return result;
}

Card QueenLevel::thinkCardInSuit(Hand *hand, Suit suit, int turn)
{
	CCLog("Call QueenLevel::thinkCardInSuit");

	Card card;
	card.suit = suit;

	int player = INVALID;
	int highestOnTable = table->highestCardInTrick(suit, player);

	bool partnerSafe = true;
	if (partner && table->isTeamHighest(suit)) {
		partnerSafe = false;
	}

	// If I have CARD_Q and highest card on the Table is power than CARD_Q
	if (partnerSafe && highestOnTable > CARD_Q && hand->findCard(suit, CARD_Q) != INVALID) {
		card.value = CARD_Q;
		return card;
	}

	// last play
	if (table->isLastPlayerInTrick()) {
		if (!partnerSafe) {
			card.value = hand->highestCardExcept(suit, CARD_Q);
			return card;
		}

		if (!table->findCardInTrick(CARD_Q)) {
			card.value = hand->highestCardExcept(suit, CARD_Q);
			return card;
		}
	}

	// play nearest card
	int value = 0;
	if (partnerSafe) {
		value = hand->lowCard(suit, highestOnTable);
	} else {
		value = hand->lowCardExcept(suit, highestOnTable, CARD_Q);
	}

	if (value == 0) { // if i haven't low card
		value = hand->highCardExcept(suit, highestOnTable, CARD_Q);
	}

	card.value = value;

	return card;
}

Card QueenLevel::thinkCardOutSuit(Hand *hand, Suit suit, int turn)
{
	CCLog("Call QueenLevel::thinkCardOutSuit");

	Card card;

	// team is highest played, do not play eat card.
	if (partner && table->isTeamHighest(suit)) {
		card = highestCardExcept(hand, suit);
		return card;
	}

	// If I have CARD_Q then play it
	Suit suits[4] = {HEART, CLOVER, DIAMOND, SPADE};
	for (int i = 0; i < 4; i++) {
		if (suit != suits[i] && duplicate[i] == turn && hand->findCard(suits[i], CARD_Q) != INVALID) {
			card.set(suits[i], CARD_Q);
			return card;
		}
	}

	for (int i = 0; i < 4; i++) {
		if (suit != suits[i] && hand->findCard(suits[i], CARD_Q) != INVALID) {
			card.set(suits[i], CARD_Q);
			return card;
		}
	}

	// If I have not CARD_Q then play highest card
	int v = CARD_BLANK;
	Suit s = BLANK;

	hand->highestCardOfAll(&s, &v);
	card.suit = s;
	card.value = v;
	return card;
}

Card QueenLevel::thinkCardFirst(Hand *hand, int turn)
{
	CCLog("Call QueenLevel::thinkCardFirst");

	Card card;

	int team = (turn + 2) % 4;
	int other1 = (turn + 1) % 4;
	int other2 = (turn + 3) % 4;
	Suit suits[4] = { HEART, CLOVER, DIAMOND, SPADE };
	int lows[4] = {0, };

	for (int i = 0; i < 4; i++) {
		if (hand->hasSuit(suits[i])) {
			lows[i] = hand->lowestCard(suits[i]);
			if (HAS_BIT(queenSuit, suits[i]) == 0 && hand->findCard(suits[i], CARD_Q) == INVALID) {
				if (partner) {
					if (duplicate[i] != team && (!players[other1]->isEmptySuit(suits[i]) || !players[other2]->isEmptySuit(suits[i]))) {
						if (lows[i] < CARD_Q) {
							card.set(suits[i], lows[i]);
							return card;
						}
					}
				} else {
					if (lows[i] < CARD_Q) {
						card.set(suits[i], lows[i]);
						return card;
					}
				}
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		if (lows[i] > 0 && lows[i] < CARD_Q) {
			card.set(suits[i], lows[i]);
			return card;
		}
	}

	for (int i = 0; i < 4; i++) {
		if (lows[i] > CARD_Q && HAS_BIT(queenSuit, suits[i])) {
			card.set(suits[i], lows[i]);
			return card;
		}
	}

	for (int i = 0; i < 4; i++) {
		if (lows[i] > CARD_Q) {
			card.set(suits[i], lows[i]);
			return card;
		}
	}

	for (int i = 0; i < 4; i++) {
		if (lows[i] > 0) {
			card.set(suits[i], lows[i]);
			return card;
		}
	}

	return card;
}

Card QueenLevel::highestCardExcept(Hand *hand, Suit suit)
{
	Card ret;
	ret.suit = BLANK;
	ret.value = CARD_BLANK;

	for ( int i = 0; i < PLAYER_CARD_NUM; i++) {
		Card card = hand->card(i);
		if (card.suit != BLANK && card.value > ret.value && !card.isQ()) {
			ret = card;
		}
	}

	if (ret.suit == BLANK) {
		for (int i = 0; i < PLAYER_CARD_NUM; i++) {
			Card card = hand->card(i);
			if (card.suit != BLANK) {
				return card;
			}
		}
	}

	return ret;
}

int QueenLevel::nearestCard(Hand *hand, Suit suit, int highestOnTable)
{
	int value = hand->lowCard(suit, highestOnTable);

	if (value == 0) { // if i haven't low card
		if (table->isLastPlayerInTrick()) { // last play
			value = hand->highestCardExcept(suit, CARD_Q);
		} else {
			value = hand->highCardExcept(suit, highestOnTable, CARD_Q);
		}
	}
	return value;
}
