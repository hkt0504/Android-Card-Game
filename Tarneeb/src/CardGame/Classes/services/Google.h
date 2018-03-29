#ifndef _GOOGLE_H_
#define _GOOGLE_H_

class BuyScene;

#define BUY_ADS 1
#define BUY_WIN 2
#define BUY_LOSE 3

class Google
{
public:
	static void purchaseAds();
	static void purchaseWin();
	static void purchaseLose();

	static BuyScene *buyScene;
};

#endif //_GOOGLE_H_
