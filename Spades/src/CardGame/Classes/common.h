#ifndef _CARDGAME_COMMON_H_
#define _CARDGAME_COMMON_H_

#define FACEBOOK

#define MAX_SCORE 500

#define PLAYER_NUM 4
#define CARD_NUM 52
#define PLAYER_CARD_NUM 13
#define CARD_SPACE_DELTA_Y -50
#define CARD_SPACE_DELTA_X 40

#define GAME_SPEED 0.5
#define BACK_BTN_MARGIN	20

typedef enum Suit
{
	HEART = 1,
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
} Card_t;

void swap_card(Card &a, Card &b);
int calc_card_value(const Card &card);
int get_card_index(const Card &card);
int suit_index(Suit suit);
Suit get_suit(int index);

typedef struct GameResult {
	int trick_user;
	int bid_user;
	int score_user;
	int bag_user;
	int trick_bot;
	int bid_bot;
	int score_bot;
	int bag_bot;
} GameResult_t;


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


#define MOVE_SPEED 0.3
#define CARD_PLAY_SPEED 0.2
#define CARD_REMOVE_SPEED	0.5

#define GAME_FROME_GUEST	1
#define GAME_FROME_FACEBOOK	2
#endif /* _CARDGAME_COMMON_H_ */
