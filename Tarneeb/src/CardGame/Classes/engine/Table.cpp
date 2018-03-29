#include "Table.h"
#include "cocos2d.h"
#include "Hand.h"

USING_NS_CC;

Table::Table()
{
	clear();
}

Table::~Table()
{
}

void Table::clear()
{
	trickCardCount = 0;
	closedCardCount = 0;

	for (int i = 0; i < CARD_NUM; i++) {
		playedCards[i].suit = BLANK;
		playedCards[i].value = 0;
	}

	playedSuitCount[0] = 0;
	playedSuitCount[1] = 0;
	playedSuitCount[2] = 0;
	playedSuitCount[3] = 0;
}

// Adds played card to next available position in playedCards array
void Table::addCardPlayed(Suit suit, int value)
{
	int index = closedCardCount + trickCardCount;
	playedCards[index].suit = suit;
	playedCards[index].value = value;
	trickCardCount ++;

	playedSuitCount[suit_index(suit)] ++;
}

Card Table::getTrickCard(int index)
{
	index += closedCardCount;
	return playedCards[index];
}

// Return turn of who win the trick
int Table::evaluateTrickWin(int turn, Suit tarneeb)
{
	CCLog("Call Table::evaluateTrickWin");

	Suit suitLed = playedCards[closedCardCount].suit;
	int highest_index = -1;
	int highest_value = 0;
	int current_value = 0;
	int winner;

	for (int i = 0; i < 4; i++) {
		Card card = playedCards[i + closedCardCount];
		if (card.suit == suitLed || card.suit == tarneeb) {
			current_value = card.suit == tarneeb ? card.value + CARD_MAX : card.value;
			if (current_value > highest_value) {
				highest_value = current_value;
				highest_index = i;
			}
		}
	}

	winner = turn + highest_index;
	winner %= PLAYER_NUM;
	return winner;
}

void Table::nextTrick()
{
	closedCardCount += 4;
	trickCardCount = 0;
}

int Table::highestCardIndexInTrick(Suit suit)
{
	int highIndex = -1;
	int highValue = 0;
	for (int i = 0; i < trickCardCount; i++) {
		if (playedCards[i + closedCardCount].suit == suit || playedCards[i + closedCardCount].suit == SPADE) {
			int value = calc_card_value(playedCards[i + closedCardCount]);
			if (value > highValue) {
				highValue = value;
				highIndex = i;
			}
		}
	}

	return highIndex;
}

// Returns true if all the cards in a certain suit above a certain value have
// been played. Else, returns false.
bool Table::isHighestCard(Suit suit, int value)
{
	bool flag = false;

	for (int i = CARD_A; i > value; i--) {
		flag = false;
		for (int j = 0; j < closedCardCount; j++) {
			if (playedCards[j].suit == suit && playedCards[j].value == i) {
				flag = true;
			}
		}

		if (flag == false) {
			return false;
		}
	}

	return true;
}

bool Table::isFinishedSuit(Hand *hand, Suit suit)
{
	int count = hand->countSuit(suit);
	if (count == 0)
		return false;
	return (count + playedSuitCount[suit_index(suit)] == 13);
}
