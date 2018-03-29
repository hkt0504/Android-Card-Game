#include "AppService.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include "MenuScene.h"

const char* ServiceClassName = "com/spade/spadegame/AppService";

MenuScene *AppService::menuScene = NULL;

extern "C"
{
void Java_com_spade_spadegame_AppService_nativeSoundCallback(JNIEnv*  env, jobject thiz, jboolean mute)
{
	if (AppService::menuScene)
		AppService::menuScene->onSound(mute);
}
};

void AppService::openMoreGame()
{
	cocos2d::JniMethodInfo methodInfo;

	if (cocos2d::JniHelper::getStaticMethodInfo(methodInfo, ServiceClassName, "nativeOpenMoreGame", "()V")) {
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

void AppService::openRateGame()
{
	cocos2d::JniMethodInfo methodInfo;

	if (cocos2d::JniHelper::getStaticMethodInfo(methodInfo, ServiceClassName, "nativeOpenRateGame", "()V")) {
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

void AppService::muteVolume()
{
	cocos2d::JniMethodInfo methodInfo;

	if (cocos2d::JniHelper::getStaticMethodInfo(methodInfo, ServiceClassName, "nativeMuteVolume", "()V")) {
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

void AppService::unmuteVolume()
{
	cocos2d::JniMethodInfo methodInfo;

	if (cocos2d::JniHelper::getStaticMethodInfo(methodInfo, ServiceClassName, "nativeUnmuteVolume", "()V")) {
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
}

bool AppService::isMuteVolume()
{
	cocos2d::JniMethodInfo methodInfo;

	if (cocos2d::JniHelper::getStaticMethodInfo(methodInfo, ServiceClassName, "nativeIsMuteVolume", "()Z")) {
		bool result = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
		return result;
	}

	return false;
}
