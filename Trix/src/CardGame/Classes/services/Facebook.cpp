#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "Facebook.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>
#include "MenuScene.h"
#include "LeaderboardScene.h"

const char* FBConnectClassName = "com/al3aabna/trix/fb/FacebookConnect";
const char* FBFriendClassName = "com/al3aabna/trix/fb/FacebookFriend";
const char* FBPostClassName = "com/al3aabna/trix/fb/FacebookPost";
const char* FBSendRequestsClassName = "com/al3aabna/trix/fb/FacebookRequests";
const char* FBScoreClassName = "com/al3aabna/trix/fb/FacebookScore";
const char* GameClassName = "com/al3aabna/trix/entry/Game";

MenuScene *Facebook::menuScene = NULL;
LeaderboardScene *Facebook::leaderboardScene = NULL;


extern "C"
{
void Java_com_al3aabna_trix_fb_FacebookConnect_nativeCallback(JNIEnv* env, jobject thiz, jint action, jobject params)
{
	Facebook::menuScene->onFaceBook(action);
}

void Java_com_al3aabna_trix_fb_FacebookScore_nativeCallback(JNIEnv* env, jobject thiz, jobject games)
{
	jclass arrayListCls = env->GetObjectClass(games);
	jmethodID sizeMethod = env->GetMethodID(arrayListCls, "size", "()I");
	jmethodID getMethod = env->GetMethodID(arrayListCls, "get", "(I)Ljava/lang/Object;");

	int count = env->CallIntMethod(games, sizeMethod);
	if (count > 0) {
		GameScore *scores = new GameScore[count];
		for (int i = 0; i < count; i++) {
			jobject game = env->CallObjectMethod(games, getMethod, i);
			jboolean isCopy = false;
			jclass cls = env->FindClass(GameClassName);
			jfieldID field = env->GetFieldID(cls, "userName", "Ljava/lang/String;");
			scores[i].name = (char *) env->GetStringUTFChars((jstring)env->GetObjectField(game, field), &isCopy);
			field = env->GetFieldID(cls, "win", "I");
			scores[i].win = (int) env->GetIntField(game, field);
			field = env->GetFieldID(cls, "lose", "I");
			scores[i].lose = (int) env->GetIntField(game, field);
		}

		Facebook::leaderboardScene->onFacebook(scores, count);
		delete[] scores;
	}
}
};

void Facebook::login()
{
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, FBConnectClassName, "nativeLogin", "(I)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, FB_LOGIN);
		t.env->DeleteLocalRef(t.classID);
	}
}

void Facebook::logout()
{
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, FBConnectClassName, "nativeLogout", "(I)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, FB_LOGOUT);
		t.env->DeleteLocalRef(t.classID);
	}
}

bool Facebook::isLogined()
{
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, FBConnectClassName, "nativeIsLogined", "()Z")) {
		jboolean ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID);
		return ret;
	}
	return false;
}

const char* Facebook::getUserId()
{
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, FBConnectClassName, "nativeGetUserId", "()Ljava/lang/String;")) {
		jstring ret = (jstring)(t.env->CallStaticObjectMethod(t.classID, t.methodID));
		t.env->DeleteLocalRef(t.classID);
		const char* aStr = " ";
		jboolean isCopy = 0;
		aStr = t.env->GetStringUTFChars(ret, &isCopy);
		return aStr;
	}
	return NULL;
}

const char* Facebook::getUserName()
{
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, FBConnectClassName, "nativeGetUserName", "()Ljava/lang/String;")) {
		jstring ret = (jstring)(t.env->CallStaticObjectMethod(t.classID, t.methodID));
		if (ret) {
			t.env->DeleteLocalRef(t.classID);
			const char* aStr;
			jboolean isCopy = 0;
			aStr = t.env->GetStringUTFChars(ret, &isCopy);
			return aStr;
		}
	}
	return NULL;
}

const char* Facebook::getUserImg()
{
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, FBConnectClassName, "nativeGetUserImage", "()Ljava/lang/String;")) {
		jstring ret = (jstring)(t.env->CallStaticObjectMethod(t.classID, t.methodID));
		if (ret) {
			t.env->DeleteLocalRef(t.classID);
			const char* aStr;
			jboolean isCopy = 0;
			aStr = t.env->GetStringUTFChars(ret, &isCopy);
			return aStr;
		}
	}
	return NULL;
}

void Facebook::inviteFriend()
{
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, FBFriendClassName, "nativeInviteFriend", "(I)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, FB_PICKFRIEND);
		t.env->DeleteLocalRef(t.classID);
	}
}

void Facebook::postStatus()
{
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, FBPostClassName, "nativePostStatus", "()V")) {
		CCLOG("FacebookInterface::postStatus");
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}

void Facebook::getScores()
{
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, FBScoreClassName, "nativeGetScores", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}

void Facebook::saveScore(int win, int lose)
{
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, FBScoreClassName, "nativeSaveScore", "(II)V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID, win, lose);
		t.env->DeleteLocalRef(t.classID);
	}
}

void Facebook::winGame()
{
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, FBScoreClassName, "nativeWinGame", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}

void Facebook::loseGame()
{
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, FBScoreClassName, "nativeLoseGame", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}

int Facebook::getWin()
{
	int result = 0;
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, FBScoreClassName, "nativeGetWin", "()I")) {
		result = t.env->CallStaticIntMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
	return result;
}

int Facebook::getLose()
{
	int result = 0;
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, FBScoreClassName, "nativeGetLose", "()I")) {
		result = t.env->CallStaticIntMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
	return result;
}

#endif
