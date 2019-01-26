#include "AppDelegate.h"
//#include "HelloWorldScene.h"
#include "MainMenuScene.h"

// #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

AppDelegate::AppDelegate() {}
AppDelegate::~AppDelegate() {}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs() {
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };
    GLView::setGLContextAttrs(glContextAttrs);
}


bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    if(!glview) {
		glview = GLViewImpl::createWithRect("3AM Cooking Simulator!", cocos2d::Rect(0.0f, 0.0f, 1280.0f, 720.0f), 1.0f, false);
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    //director->setDisplayStats(false);

	auto mainMenu = MainMenu::createScene();
    director->runWithScene(mainMenu);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {}
void AppDelegate::applicationWillEnterForeground() {}
