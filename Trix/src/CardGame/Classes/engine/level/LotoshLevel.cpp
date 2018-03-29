#include "LotoshLevel.h"
#include "Hand.h"
#include "Player.h"


USING_NS_CC;

LotoshLevel::LotoshLevel()
{
	playedCount = 0;
}

LotoshLevel::~LotoshLevel()
{

}

bool LotoshLevel::evaluateScore(int turn)
{
	CCLog("Call LotoshLevel::evaluateScore");

	Card card = table->playedCards[table->closedCardCount];
	Suit suitLed = card.suit;
	int highest_index = 0;
	int highest_value = 0;

	for (int i = 0; i < 4; i++) {
		card = table->playedCards[i + table->closedCardCount];
		if (card.suit == suitLed && card.value > highest_value) {
			highest_value = card.value;
			highest_index = i;
		}
	}

	int winner = (turn + highest_index) % PLAYER_NUM;
	players[winner]->score -= SCORE_LOTOSH;
	players[winner]->eatLotosh ++;

	playedCount ++;
	if (playedCount == PLAYER_CARD_NUM) {
		return true;
	}

	return false;
}

Card LotoshLevel::thinkCardInSuit(Hand *hand, Suit suit, int turn)
{
	CCLog("Call LotoshLevel::thinkCardInSuit");

	Card card;
	card.suit = suit;

	if (partner && table->isLastPlayerInTrick() && table->isTeamHighest(suit)) {
		card.value = hand->highestCard(suit);
		return card;
	}

	int highestOnTable = table->highestCardInTrick(suit);
	card.value = nearestCard(hand, suit, highestOnTable);

	return card;
}

Card LotoshLevel::thinkCardOutSuit(Hand *hand, Suit suit, int turn)
{
	CCLog("Call LotoshLevel::thinkCardOutSuit");

	Card card;

	int v = CARD_BLANK;
	Suit s = BLANK;

	hand->highestCardOfAll(&s, &v);
	card.suit = s;
	card.value = v;
	return card;
}

Card LotoshLevel::thinkCardFirst(Hand *hand, int turn)
{
	CCLog("Call LotoshLevel::thinkCardFirst");

	Card card;
	lowestCardOfAll(hand, &card);
	return card;
}

int LotoshLevel::nearestCard(Hand *hand, Suit suit, int highestOnTable)
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
