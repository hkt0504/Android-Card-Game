#include "MainTable.h"
#include "cocos2d.h"
#include "Hand.h"

USING_NS_CC;

MainTable::MainTable()
{
	trickCardCount = 0;
	closedCardCount = 0;

	// setup the cards played
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
void MainTable::addCardPlayed(Suit suit, int value)
{
	int index = closedCardCount + trickCardCount;
	playedCards[index].suit = suit;
	playedCards[index].value = value;
	trickCardCount ++;

	playedSuitCount[suit_index(suit)] ++;
}

void MainTable::nextTrick()
{
	closedCardCount += 4;
	trickCardCount = 0;
}

int MainTable::highestCardInTrick(Suit suit, OUT int& player)
{
	int highValue = 0;
	player = INVALID;

	for (int i = 0; i < trickCardCount; i++) {
		if (playedCards[i + closedCardCount].suit == suit) {
			int value = playedCards[i + closedCardCount].value;
			if (value > highValue) {
				highValue = value;
				player = i;
			}
		}
	}

	return highValue;
}

int MainTable::highestCardInTrick(Suit suit)
{
	int highValue = 0;
	for (int i = 0; i < trickCardCount; i++) {
		if (playedCards[i + closedCardCount].suit == suit) {
			int value = playedCards[i + closedCardCount].value;
			if (value > highValue) {
				highValue = value;
			}
		}
	}

	return highValue;
}

bool MainTable::findCardInTrick(Suit suit, int value)
{
	for (int i = 0; i < trickCardCount; i++) {
		if (playedCards[i + closedCardCount].suit == suit && playedCards[i + closedCardCount].value == value) {
			return true;
		}
	}
	return false;
}

bool MainTable::findSuitInTrick(Suit suit)
{
	for (int i = 0; i < trickCardCount; i++) {
		if (playedCards[i + closedCardCount].suit == suit) {
			return true;
		}
	}
	return false;
}

bool MainTable::findCardInTrick(int value)
{
	for (int i = 0; i < trickCardCount; i++) {
		if (playedCards[i + closedCardCount].value == value) {
			return true;
		}
	}
	return false;
}

// Returns true if all the cards in a certain suit above a certain value have
// been played. Else, returns false.
bool MainTable::isHighestCard(Suit suit, int value)
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

bool MainTable::isLowestCard(Suit suit, int value)
{
	bool flag = false;

	for (int i = CARD_2; i < value; i++) {
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

int MainTable::evaluateTrickWin(int turn)
{
	CCLog("Call Table::evaluateTrickWin");

	Suit suitLed = playedCards[closedCardCount].suit;
	int highest_index = 0;
	int highest_value = playedCards[highest_index + closedCardCount].value;
	int current_value;
	int winner;

	for (int i = 1; i < 4; i++) {
		if (playedCards[i + closedCardCount].suit == suitLed) {
			current_value = playedCards[i + closedCardCount].value;
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

bool MainTable::isTeamHighest(Suit suit)
{
	int player = INVALID;
	int value = highestCardInTrick(suit, player);

	if (trickCardCount > 1 && player == trickCardCount - 2) {
		return true;
	}

	return false;
}

bool MainTable::isFinishedSuit(Hand *hand, Suit suit)
{
	int count = hand->countSuit(suit);
	if (count == 0)
		return false;
	return (count + playedSuitCount[suit_index(suit)] == 13);
}
