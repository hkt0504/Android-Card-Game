#include "AppService.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include "MenuScene.h"
#include "ModeScene.h"

const char* ServiceClassName = "com/al3aabna/trix/AppService";

MenuScene *AppService::menuScene = NULL;
ModeScene *AppService::modeScene = NULL;
bool AppService::partner = false;
bool AppService::complex = false;

extern "C"
{
void Java_com_al3aabna_trix_AppService_nativeSoundCallback(JNIEnv*  env, jobject thiz, jboolean mute)
{
	if (AppService::menuScene)
		AppService::menuScene->onSound(mute);

	if (AppService::modeScene)
		AppService::modeScene->onSound(mute);
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

void AppService::setPartner(bool partner)
{
	AppService::partner = partner;
}

bool AppService::isPartner()
{
	return AppService::partner;
}

void AppService::setComplex(bool complex)
{
	AppService::complex = complex;
}

bool AppService::isComplex()
{
	return AppService::complex;
}
