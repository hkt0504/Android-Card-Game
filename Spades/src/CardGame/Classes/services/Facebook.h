#ifndef __FACEBOOK_H_
#define __FACEBOOK_H_

#include "common.h"

#define FB_LOGIN 1
#define FB_LOGOUT 2
#define FB_GETSTATUS 3
#define FB_PICKFRIEND 4
#define FB_POSTSTATUS 5

class MenuScene;
class LeaderboardScene;

class Facebook
{
public:
	static void login();
	static void logout();
	static bool isLogined();

	static const char* getUserId();
	static const char* getUserName();
	static const char* getUserImg();

	static void inviteFriend();
	static void postStatus();
	static void getScores();
	static void saveScore(int win, int lose);

	static void winGame();
	static void loseGame();
	static int getWin();
	static int getLose();

	static MenuScene *menuScene;
	static LeaderboardScene *leaderboardScene;
};

#endif  //__FACEBOOK_H_
