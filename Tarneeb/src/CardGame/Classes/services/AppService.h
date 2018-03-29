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

	static bool isNoTarneeb();
	static void setNoTarneeb(bool);

	static MenuScene *menuScene;
private:
	static bool noTarneeb;
};

#endif //_APPSERVICE_H_
