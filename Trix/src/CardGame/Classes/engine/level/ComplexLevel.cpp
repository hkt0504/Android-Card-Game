#include "ComplexLevel.h"
#include "Hand.h"
#include "Player.h"

USING_NS_CC;

ComplexLevel::ComplexLevel()
{
	playedCount = 0;
	duplicateK = INVALID;
	duplicateQ[0] = INVALID;
	duplicateQ[1] = INVALID;
	duplicateQ[2] = INVALID;
	duplicateQ[3] = INVALID;

	kingPlayed = false;
	queenPlayed = 0;
}

ComplexLevel::~ComplexLevel()
{

}

/** return is finished and calculate score */
bool ComplexLevel::evaluateScore(int turn)
{
	CCLog("Call ComplexLevel::evaluateScore");

	Card card = table->playedCards[table->closedCardCount];
	Suit suitLed = card.suit;
	int highest_index = 0;
	int highest_value = 0;

	int qSuit = 0;
	int diamond = 0;
	int kingIndex = -1;

	for (int i = 0; i < 4; i++) {
		card = table->playedCards[i + table->closedCardCount];

		if (card.suit == DIAMOND) {
			diamond ++;
		}

		if (card.value == CARD_Q) {
			SET_BIT(qSuit, card.suit);
			SET_BIT(queenPlayed, card.suit);
		}

		if (card.isHK()) {
			kingIndex = i;
			kingPlayed = true;
		}

		if (card.suit == suitLed && card.value > highest_value) {
			highest_value = card.value;
			highest_index = i;
		}
	}

	int winner = (turn + highest_index) % PLAYER_NUM;

	// lotosh score.
	players[winner]->score -= SCORE_LOTOSH;
	players[winner]->eatLotosh ++;

	// diamond score.
	if (diamond > 0) { //diamond is played
		players[winner]->score -= diamond * SCORE_DIAMOND;
		players[winner]->eatDiamond += diamond;
	}

	// queen score
	if (qSuit != 0) { //queen is played
		Suit suits[4] = {HEART, CLOVER, DIAMOND, SPADE};
		int dups[4] = {DUP_H_Q, DUP_C_Q, DUP_D_Q, DUP_S_Q};
		for (int i = 0; i < 4; i++) {
			if (HAS_BIT(qSuit, suits[i])) {
				players[winner]->score -= SCORE_QUEEN;
				SET_BIT(players[winner]->eatKQ, dups[i]);

				if (duplicateQ[i] != INVALID) {
					REMOVE_BIT(players[duplicateQ[i]]->duplicates, dups[i]);

					if (partner) {
						players[winner]->score -= SCORE_QUEEN;
						if (duplicateQ[i] == winner || duplicateQ[i] == ((winner + 2) % PLAYER_NUM)) {
							players[(duplicateQ[i] + 1) % PLAYER_NUM]->score += SCORE_QUEEN;
						} else {
							players[duplicateQ[i]]->score += SCORE_QUEEN;
						}
					} else {
						if (winner != duplicateQ[i]) {
							players[winner]->score -= SCORE_QUEEN;
							players[duplicateQ[i]]->score += SCORE_QUEEN;
						}
					}
				}
			}
		}
	}

	// H-King score
	if (kingIndex != -1) { //King is played
		players[winner]->score -= SCORE_KING;
		SET_BIT(players[winner]->eatKQ, DUP_K);

		if (duplicateK != INVALID) {
			REMOVE_BIT(players[duplicateK]->duplicates, DUP_K);

			if (partner) {
				players[winner]->score -= SCORE_KING;
				if (duplicateK == winner || duplicateK == ((winner + 2) % PLAYER_NUM)) {
					players[(duplicateK + 1) % PLAYER_NUM]->score += SCORE_KING;
				} else {
					players[duplicateK]->score += SCORE_KING;
				}
			} else {
				if (winner != duplicateK) {
					players[winner]->score -= SCORE_KING;
					players[duplicateK]->score += SCORE_KING;
				}
			}
		}
	}

	playedCount ++;
	if (playedCount == PLAYER_CARD_NUM) {
		return true;
	}

	return false;
}

bool ComplexLevel::getDuplicateList(int player, int& duplicates)
{
	CCLog("Call ComplexLevel::getDuplicateList");

	Level::getDuplicateList(player, duplicates);

	bool result = false;
	Hand *hand = players[player]->hand;

	if (hand->findCard(HEART, CARD_K) != INVALID) {
		SET_BIT(duplicates, DUP_K);
		result = true;
	}

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

void ComplexLevel::setDuplicate(int player, int duplicates)
{
	CCLog("Call ComplexLevel::setDuplicate");

	if (HAS_BIT(duplicates, DUP_K)) {
		duplicateK = player;
	}

	int flags[4] = { DUP_H_Q, DUP_C_Q, DUP_D_Q, DUP_S_Q };
	for (int i = 0; i < 4; i++) {
		if (HAS_BIT(duplicates, flags[i])) {
			duplicateQ[i] = player;
		}
	}
}

bool ComplexLevel::thinkDuplicate(int player, int& duplicates)
{
	CCLog("Call ComplexLevel::thinkDuplicate");

	Level::thinkDuplicate(player, duplicates);

	bool result = false;
	Hand *hand = players[player]->hand;

	if (hand->findCard(HEART, CARD_K) != INVALID && hand->countSuit(HEART) >= 5) {
		SET_BIT(duplicates, DUP_K);
		result = true;
	}

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

Card ComplexLevel::thinkCardInSuit(Hand *hand, Suit suit, int turn)
{
	CCLog("Call ComplexLevel::thinkCardInSuit");

	Card card;
	card.suit = suit;

	int player = INVALID;
	int highestOnTable = table->highestCardInTrick(suit, player);

	// team is highest played
	if (partner && player == table->trickCardCount - 2) {
		if (table->isLastPlayerInTrick()) {
			card.value = highestCardInSuit(hand, suit);
			return card;
		} else if (table->trickCardCount > 1) {
			card.value = nearestCardWithTeam(hand, suit, highestOnTable);
			return card;
		}
	}

	// if suit is Heart, and I have Heart-K and there is Heart-A on table, then play It
	if (suit == HEART && hand->findCard(HEART, CARD_K) != INVALID && highestOnTable > CARD_K){
		card.value = CARD_K;
		return card;
	}

	// if I have Q-Card, and there is power card than Q on table, then play Q-card
	if (hand->findCard(suit, CARD_Q) != INVALID && highestOnTable > CARD_Q) {
		card.value = CARD_Q;
		return card;
	}

	card.value = nearestCard(hand, suit, highestOnTable);

	return card;
}

Card ComplexLevel::thinkCardOutSuit(Hand *hand, Suit suit, int turn)
{
	CCLog("Call ComplexLevel::thinkCardOutSuit");

	Card card;

	// team is highest played
	if (partner && table->isTeamHighest(suit)) {
		return highestCardOutSuit(hand, suit);
	}

	//If I have Heart-K, play it
	if (hand->findCard(HEART, CARD_K) != INVALID) {
		card.suit = HEART;
		card.value = CARD_K;
		return card;
	}

	// If I have Q-Card, play It
	Suit suits[4] = {HEART, CLOVER, DIAMOND, SPADE};
	for (int i = 0; i < 4; i++) {
		if (suit != suits[i] && duplicateQ[i] == turn &&  hand->findCard(suits[i], CARD_Q) != INVALID) {
			card.suit = suits[i];
			card.value = CARD_Q;
			return card;
		}
	}

	for (int i = 0; i < 4; i++) {
		if (suit != suits[i] && hand->findCard(suits[i], CARD_Q) != INVALID) {
			card.suit = suits[i];
			card.value = CARD_Q;
			return card;
		}
	}

	//If suit is not D-card, and I have D card, then play power D-card
	if (suit != DIAMOND && hand->hasSuit(DIAMOND)){
		card.suit = DIAMOND;
		card.value = hand->highestCard(DIAMOND);
		return card;
	}

	// find highest card.
	int v = CARD_BLANK;
	Suit s = BLANK;

	hand->highestCardOfAll(&s, &v);
	card.suit = s;
	card.value = v;
	return card;
}

Card ComplexLevel::thinkCardFirst(Hand *hand, int turn)
{
	CCLog("Call ComplexLevel::thinkCardFirst");

	Card card;

	int exceptSuit = 0;
	if (duplicateK == turn && !kingPlayed) {
		exceptSuit |= HEART;
	}

	Suit suits[4] = {HEART, CLOVER, DIAMOND, SPADE};
	for (int i = 0; i < 4; i++) {
		if (duplicateQ[i] == turn && !HAS_BIT(queenPlayed, suits[i])) {
			exceptSuit |= suits[i];
		}
	}

	if (partner) {
		int team = (turn + 2) % PLAYER_NUM;
		if (duplicateK == team && !kingPlayed) {
			exceptSuit |= HEART;
		}

		for (int i = 0; i < 4; i++) {
			if (duplicateQ[i] == team && !HAS_BIT(queenPlayed, suits[i])) {
				exceptSuit |= suits[i];
			}
		}
	}

	card = lowestCardExcept(hand, turn, exceptSuit);
	if (card.isInvalid()) {
		card = lowestCardExcept(hand, turn, 0);
		if (!card.isInvalid()) {
			return card;
		}
	} else {
		return card;
	}

	// If I have K, A, play this
	for (int i = 0; i < PLAYER_CARD_NUM; i++) {
		Card hcard = hand->card(i);
		if (hcard.suit != BLANK && !hcard.isHK() && !hcard.isQ()) {
			return hcard;
		}
	}

	// if I have D-Card, play lowest D-Card
	if (hand->hasSuit(DIAMOND)) {
		card.suit = DIAMOND;
		card.value = hand->lowestCard(DIAMOND);
		return card;
	}

	// If I have Q-Card, play It
	for (int i = 0; i < 4; i++) {
		if (hand->findCard(suits[i], CARD_Q) != INVALID) {
			card.suit = suits[i];
			card.value = CARD_Q;
			return card;
		}
	}

	// If I have Heart-K, play it
	if (hand->findCard(HEART, CARD_K) != INVALID) {
		card.suit = HEART;
		card.value = CARD_K;
		return card;
	}

	return card;
}

int ComplexLevel::highestCardInSuit(Hand *hand, Suit suit)
{
	for (int i = CARD_A; i >= CARD_2; i--) {
		if ((suit == HEART && i == CARD_K) || i == CARD_Q) {
			continue;
		}

		if (hand->findCard(suit, i) != INVALID) {
			return i;
		}
	}

	if (hand->findCard(suit, CARD_Q) != INVALID)
		return CARD_Q;

	if (hand->findCard(suit, CARD_K) != INVALID)
		return CARD_K;

	return 0;
}

Card ComplexLevel::highestCardOutSuit(Hand *hand, Suit suit)
{
	Card ret;
	ret.suit = BLANK;
	ret.value = CARD_BLANK;

	for ( int i = 0; i < PLAYER_CARD_NUM; i++) {
		Card card = hand->card(i);
		if (card.suit != BLANK && card.value > ret.value && !card.isHK() && !card.isQ() && !card.isD()) {
			ret = card;
		}
	}

	if (ret.suit == BLANK) {
		if (suit != DIAMOND) {
			int value = hand->highestCard(DIAMOND);
			if (value >= CARD_2 && value != CARD_Q) {
				ret.suit = DIAMOND;
				ret.value = value;
				return ret;
			}
		}

		for (int i = 0; i < PLAYER_CARD_NUM; i++) {
			Card card = hand->card(i);
			if (card.suit != BLANK) {
				return card;
			}
		}
	}

	return ret;
}

Card ComplexLevel::lowestCardExcept(Hand *hand, int turn, int exceptSuit)
{
	Card card;
	card.set(BLANK, 0);

	// i have lowest card and play it
	Suit suit4Lows[4] = { DIAMOND, CLOVER, SPADE, HEART };
	for (int i = 0; i < 4; i++) {
		if (!HAS_BIT(exceptSuit, suit4Lows[i]) && hand->hasSuit(suit4Lows[i]) && !table->isFinishedSuit(hand, suit4Lows[i])) {
			int value = hand->lowestCard(suit4Lows[i]);
			if (table->isLowestCard(suit4Lows[i], value) || value < CARD_4) {
				card.set(suit4Lows[i], value);
				return card;
			}
		}
	}

	int power = CARD_MAX;
	int index = INVALID;

	// get lowest card
	for (int i = 0; i < PLAYER_CARD_NUM; i++) {
		if (hand->cardSuit(i) != BLANK) {
			int suit = hand->cardSuit(i);
			int value = hand->cardValue(i);

			if (value >= CARD_Q || HAS_BIT(exceptSuit, suit)) {
				continue;
			}

			if (value < power) {
				index = i;
				power = value;
			}
		}
	}

	if (index != INVALID) {
		card.set(hand->cardSuit(index), hand->cardValue(index));
		return card;
	}

	return card;
}

int ComplexLevel::nearestCardWithTeam(Hand *hand, Suit suit, int highestOnTable)
{
	for (int i = highestOnTable - 1; i >= CARD_2; i--) {
		if (suit == HEART) {
			if (hand->findCard(suit, i) != INVALID && i != CARD_K && i != CARD_Q) {
				return i;
			}
		} else {
			if (hand->findCard(suit, i) != INVALID && i != CARD_Q) {
				return i;
			}
		}
	}

	return highCard(hand, suit, highestOnTable);
}

int ComplexLevel::nearestCard(Hand *hand, Suit suit, int highestOnTable)
{
	int value = hand->lowCard(suit, highestOnTable);

	if (value == 0) { // if i haven't low card
		return highCard(hand, suit, highestOnTable);
	}
	return value;
}

int ComplexLevel::highCard(Hand *hand, Suit suit, int highestOnTable)
{
	int value = 0;
	if (table->isLastPlayerInTrick()) { // last play
		if (suit == HEART) {
			for (int i = CARD_A; i >= CARD_2; i--) {
				if (hand->findCard(suit, i) != INVALID && i != CARD_Q && i != CARD_K) {
					return i;
				}
			}
		} else {
			for (int i = CARD_A; i >= CARD_2; i--) {
				if (hand->findCard(suit, i) != INVALID && i != CARD_Q) {
					return i;
				}
			}
		}
	} else {
		if (suit == HEART) {
			for (int i = highestOnTable + 1; i <= CARD_A; i++) {
				if (hand->findCard(suit, i) != INVALID && i != CARD_Q && i != CARD_K) {
					return i;
				}
			}
		} else {
			for (int i = highestOnTable + 1; i <= CARD_A; i++) {
				if (hand->findCard(suit, i) != INVALID && i != CARD_Q) {
					return i;
				}
			}
		}
	}

	if (hand->findCard(suit, CARD_Q) != INVALID) {
		return CARD_Q;
	}

	if (hand->findCard(suit, CARD_K) != INVALID) {
		return CARD_K;
	}

	return value;
}
