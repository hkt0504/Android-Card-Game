#include "AppDelegate.h"
#include "StartScene.h"
#include "common.h"


using namespace CocosDenshion;
USING_NS_CC;

AppDelegate::AppDelegate()
{
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/pop.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/shuffle.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/win.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/spade.mp3");
}

AppDelegate::~AppDelegate() 
{
    SimpleAudioEngine::getInstance()->unloadEffect("sounds/pop.mp3");
    SimpleAudioEngine::getInstance()->unloadEffect("sounds/shuffle.mp3");
    SimpleAudioEngine::getInstance()->unloadEffect("sounds/win.mp3");
    SimpleAudioEngine::getInstance()->unloadEffect("sounds/spade.mp3");
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }

    // set dimension
    glview->setDesignResolutionSize(SCR_W, SCR_H, ResolutionPolicy::EXACT_FIT);

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = StartScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
