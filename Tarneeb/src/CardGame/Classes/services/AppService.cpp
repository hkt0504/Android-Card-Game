#include "AppService.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include "MenuScene.h"

const char* ServiceClassName = "com/al3aabna/tarneeb/AppService";

MenuScene *AppService::menuScene = NULL;
bool AppService::noTarneeb = false;


extern "C"
{
void Java_com_al3aabna_tarneeb_AppService_nativeSoundCallback(JNIEnv*  env, jobject thiz, jboolean mute)
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

bool AppService::isNoTarneeb()
{
	return AppService::noTarneeb;
}

void AppService::setNoTarneeb(bool notarneeb)
{
	AppService::noTarneeb = notarneeb;
}
