#ifndef _CARDGAME_COMMON_H_
#define _CARDGAME_COMMON_H_

#define FACEBOOK

#define INVALID -1

#define GAME_SPEED 0.5

#define SCORE_KING 75
#define SCORE_QUEEN 25
#define SCORE_DIAMOND 10
#define SCORE_LOTOSH 15

#define PLAYER_NUM 4
#define CARD_NUM 52
#define PLAYER_CARD_NUM 13

// card calue
#define CARD_BLANK 1
#define CARD_2	2
#define CARD_3 	3
#define CARD_4 	4
#define CARD_5 	5
#define CARD_6 	6
#define CARD_7 	7
#define CARD_8 	8
#define CARD_9 	9
#define CARD_10 10
#define CARD_J 	11
#define CARD_Q 	12
#define CARD_K 	13
#define CARD_A 	14
#define CARD_MAX 15

#define SUIT_CARDS 13


typedef enum Suit
{
	HEART = (1 << 1),
	CLOVER = (1 << 2),
	DIAMOND = (1 << 3),
	SPADE = (1 << 4),
	BLANK = (1 << 5),
	SUIT_ALL = (HEART | CLOVER | DIAMOND | SPADE),

} Suit_t;

// defines a card
typedef struct Card {
	Suit suit;
	int value;

	Card()
	{
		suit = BLANK;
		value = 0;
	}

	void set(Suit s, int v) { suit = s; value = v; }
	bool isInvalid() { return suit == BLANK; }
	bool isHK() { return (suit == HEART && value == CARD_K); };
	bool isQ() { return (value == CARD_Q); };
	bool isD() { return (suit == DIAMOND); }

} Card_t;

void swap_card(Card &a, Card &b);
int calc_card_value(const Card &card);
int get_card_index(const Card &card);
int suit_index(Suit suit);
Suit get_suit(int index);

typedef enum BidType {
	BT_NONE = 0,
	BT_KING = 1,
	BT_QUEEN = (1 << 1),
	BT_DIAMOND = (1 << 2),
	BT_LOTOSH = (1 << 3),
	BT_TRIX = (1 << 4),
	BT_COMPLEX = (1 << 5),

	BT_INDIVIDUAL = BT_KING | BT_QUEEN | BT_DIAMOND | BT_LOTOSH | BT_TRIX,
	BT_COMPLEXS = BT_TRIX | BT_COMPLEX
} BidType_t;

// flags
enum {
	DUP_NONE = 0,
	DUP_K = 1,
	DUP_H_Q = (1 << 1),
	DUP_C_Q = (1 << 2),
	DUP_D_Q = (1 << 3),
	DUP_S_Q = (1 << 4),

	// card 2
	DUP_H_2 = (1 << 5),
	DUP_C_2 = (1 << 6),
	DUP_D_2 = (1 << 7),
	DUP_S_2 = (1 << 8),

	DUP_Q_MASK = 0x1E,

	DUP_ALL_DUE = (DUP_H_2 | DUP_C_2 | DUP_D_2 | DUP_S_2),
};

typedef struct EatCard {
	int score;
	int eatKQ;
	int eatD;
	int eatL;
} EatCard_t;

#define JUNIOR    1
#define SENIOR    51
#define EXCELLENT 101
#define TALENT    201
#define GENIUS    301

struct GameScore {
	char *name;
	int win;
	int lose;

	GameScore()
	{
		name = 0x0;
		win = 0;
		lose = 0;
	}

	int score()
	{
		return win * 10 - lose * 7;
	}
};

float frand(int min, int max);

extern const char* PROFILE_NAMES[14];
extern const char* PROFILE_NAMES_UP[14];

#define SCR_W 1280.0
#define SCR_H 800.0
#define DSN_H 864.0

#define Y(y) (y * SCR_H / DSN_H)

#define MOVE_SPEED 0.3
#define CARD_PLAY_SPEED 0.2
#define CARD_REMOVE_SPEED	0.5

#define CARD_SPACE_DELTA_Y -50
#define CARD_SPACE_DELTA_X 40

#define GAME_FROM_GUEST	1
#define GAME_FROM_FACEBOOK	2

#define BACK_BTN_MARGIN		60

#define SAFE_DELETE(x) if (x) { delete x; x = NULL; }

#define HAS_BIT(x, b) (x & b)
#define SET_BIT(x, b) (x |= b)
#define REMOVE_BIT(x, b) (x &= (~b))

#define OUT

#endif /* _CARDGAME_COMMON_H_ */
