#ifndef _APPSERVICE_H_
#define _APPSERVICE_H_

class MenuScene;
class ModeScene;

class AppService
{
public:
	static void openMoreGame();
	static void openRateGame();

	static void muteVolume();
	static void unmuteVolume();
	static bool isMuteVolume();

	static void setPartner(bool);
	static bool isPartner();

	static void setComplex(bool);
	static bool isComplex();

	static MenuScene *menuScene;
	static ModeScene *modeScene;

private:
	static bool partner;
	static bool complex;
};

#endif //_APPSERVICE_H_
