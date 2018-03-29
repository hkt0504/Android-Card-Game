#include "Player.h"
#include "Hand.h"

Player::Player()
{
	hand = new Hand();
	score = 0;
	complex = false;
	bid = BT_INDIVIDUAL;

	clear();
}

Player::~Player()
{
	delete hand;
}

void Player::setMode(bool complex)
{
	this->complex = complex;
	bid = complex ? BT_COMPLEXS : BT_INDIVIDUAL;
}

void Player::clear()
{
	emptySuit[0] = false;
	emptySuit[1] = false;
	emptySuit[2] = false;
	emptySuit[3] = false;

	played = false;
	score = 0;
	duplicates = DUP_NONE;

	eatKQ = DUP_NONE;
	eatDiamond = 0;
	eatLotosh = 0;
}

bool Player::noHaveBid()
{
	return bid == BT_NONE;
}

void Player::setEmptySuit(Suit suit)
{
	emptySuit[suit_index(suit)] = true;
}

bool Player::isEmptySuit(Suit suit)
{
	return emptySuit[suit_index(suit)];
}

BidType Player::thinkBid()
{
	if (complex) {
		if (HAS_BIT(bid, BT_TRIX)) {
			if (hand->countHigh(CARD_J) > 4 && hand->countEqual(CARD_2) == 0) {
				REMOVE_BIT(bid, BT_TRIX);
				return BT_TRIX;
			}
		}

		if (HAS_BIT(bid, BT_COMPLEX)) {
			REMOVE_BIT(bid, BT_COMPLEX);
			return BT_COMPLEX;
		}

		if (HAS_BIT(bid, BT_TRIX)) {
			REMOVE_BIT(bid, BT_TRIX);
			return BT_TRIX;
		}
	} else {
		// check King
		if (HAS_BIT(bid, BT_KING)) {
			if ((hand->findCard(HEART, CARD_K) != INVALID && hand->countSuit(HEART) > 3) || hand->countSuit(HEART) >= 5) {
				REMOVE_BIT(bid, BT_KING);
				return BT_KING;
			}
		}

		// check Lotosh
		if (HAS_BIT(bid, BT_LOTOSH)) {
			if (hand->countHigh(CARD_J) == 0) {
				REMOVE_BIT(bid, BT_LOTOSH);
				return BT_LOTOSH;
			}
		}

		// check Trix
		if (HAS_BIT(bid, BT_TRIX)) {
			if (hand->countHigh(CARD_J) > 4 && hand->countEqual(CARD_2) == 0) {
				REMOVE_BIT(bid, BT_TRIX);
				return BT_TRIX;
			}
		}

		// check diamond
		if (HAS_BIT(bid, BT_DIAMOND)) {
			if (hand->highestCard(DIAMOND) <= CARD_5) {
				REMOVE_BIT(bid, BT_DIAMOND);
				return BT_DIAMOND;
			}
		}

		// if anything not selected, select order
		BidType bids[5] = {BT_QUEEN, BT_DIAMOND, BT_LOTOSH, BT_TRIX, BT_KING};
		for (int i = 0; i < 5; i++) {
			if (HAS_BIT(bid, bids[i])) {
				REMOVE_BIT(bid, bids[i]);
				return bids[i];
			}
		}
	}

	return BT_NONE;
}
