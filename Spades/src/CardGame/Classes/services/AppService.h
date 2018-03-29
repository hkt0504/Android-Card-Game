#ifndef _APPSERVICE_H_
#define _APPSERVICE_H_


class MenuScene;

class AppService
{
public:
	static void openMoreGame();
	static void openRateGame();

	static void muteVolume();
	static void unmuteVolume();
	static bool isMuteVolume();

	static MenuScene *menuScene;
};

#endif //_APPSERVICE_H_
