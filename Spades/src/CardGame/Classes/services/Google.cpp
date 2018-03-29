#include "Google.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include "BuyScene.h"


BuyScene *Google::buyScene = NULL;

const char* GoogleClassName = "com/spade/spadegame/inapp/GoogleBilling";

extern "C"
{
	void Java_com_spade_spadegame_inapp_GoogleBilling_nativeCallback(JNIEnv*  env, jobject thiz, jint action)
	{
		Google::buyScene->onGooglePurchase(action);
	}
};

void Google::purchaseAds()
{
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, GoogleClassName, "nativePurchaseAds", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}

void Google::purchaseWin()
{
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, GoogleClassName, "nativePurchaseWin", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}

void Google::purchaseLose()
{
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, GoogleClassName, "nativePurchaseLose", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}
