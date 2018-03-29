#include "Player.h"
#include "Hand.h"

Player::Player()
{
	hand = new Hand();

	clear();
}

Player::~Player()
{
	delete hand;
}

void Player::clear()
{
	tricks = 0;
	bid = 0;
	tarneeb = BLANK;

	emptySuit[0] = false;
	emptySuit[1] = false;
	emptySuit[2] = false;
	emptySuit[3] = false;

	played = false;
	score = 0;
}

void Player::setEmptySuit(Suit suit)
{
	emptySuit[suit_index(suit)] = true;
}

bool Player::isEmptySuit(Suit suit)
{
	return emptySuit[suit_index(suit)];
}

void Player::thinkBid(int maxBid)
{
	int bid = 0;
	Suit suit = DIAMOND;
	int max = 0;

	int maxDiamon = 0;
	int maxSpades = 0;
	int maxClubs = 0;
	int maxHearts = 0;

	for (int i = 0; i < PLAYER_CARD_NUM; i++) {
		Card card = hand->card(i);
		switch (card.suit) {
		case DIAMOND:
			maxDiamon++;
			break;
		case HEART:
			maxHearts++;
			break;
		case SPADE:
			maxSpades++;
			break;
		case CLOVER:
			maxClubs++;
			break;
		default:
			break;
		}
	}

	max = maxDiamon;
	if (max < maxHearts) {
		max = maxHearts;
		suit = HEART;
	}

	if (max < maxSpades) {
		max = maxSpades;
		suit = SPADE;
	}

	if (max < maxClubs) {
		max = maxClubs;
		suit = CLOVER;
	}

	if (max >= 5) {
		bid = max + 1;
	}

	for (int i = 0; i < PLAYER_CARD_NUM; i++) {
		Card card = hand->card(i);
		if (card.suit != suit) {
			if (card.value == CARD_A) {
				bid++;
			} else if (card.value == CARD_K) {
				switch (card.suit) {
				case HEART:
					if (maxHearts > 1)
						bid++;
					break;
				case CLOVER:
					if (maxClubs > 1)
						bid++;
					break;
				case SPADE:
					if (maxSpades > 1)
						bid++;
					break;
				case DIAMOND:
					if (maxDiamon > 1)
						bid++;
					break;
				default:
					break;
				}
			} else if (card.value == CARD_Q) {
				switch (card.suit) {
				case HEART:
					if (maxHearts > 2)
						bid++;
					break;
				case CLOVER:
					if (maxClubs > 2)
						bid++;
					break;
				case SPADE:
					if (maxSpades > 2)
						bid++;
					break;
				case DIAMOND:
					if (maxDiamon > 2)
						bid++;
					break;
				default:
					break;
				}
			}
		}
	}

	if (bid > maxBid && bid > 7) {
		this->bid = bid;

		Suit secondSuit = BLANK;
		if (max == maxDiamon && suit != DIAMOND)
			secondSuit = DIAMOND;
		if (max == maxClubs && suit != CLOVER)
			secondSuit = CLOVER;
		if (max == maxHearts && suit != HEART)
			secondSuit = HEART;
		if (max == maxSpades && suit != SPADE)
			secondSuit = SPADE;

		if (secondSuit != BLANK) {
			this->tarneeb = getHighestSuit(suit, secondSuit);
		} else {
			this->tarneeb = suit;
		}
	} else {
		this->bid = 0;
		this->tarneeb = BLANK;
	}
}

Suit Player::getHighestSuit(Suit suit1, Suit suit2)
{
	int weight1 = 0;
	int weight2 = 0;

	for (int i = 0; i < PLAYER_CARD_NUM; i++) {
		Card card = hand->card(i);

		if (card.suit == suit1)
			weight1 += card.value;

		if (card.suit == suit2)
			weight2 += card.value;
	}

	return (weight1 > weight2) ? suit1 : suit2;
}
