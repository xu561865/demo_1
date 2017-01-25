#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "TestScrollView.h"

USING_NS_CC;

#define RESOLUTION_SIZE_X 640
#define RESOLUTION_SIZE_Y 960

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
    CCDirector::sharedDirector()->setDepthTest(false);
    //turn on display FPS
    //pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
    
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(RESOLUTION_SIZE_X, RESOLUTION_SIZE_Y, kResolutionShowAll);
    
    CCScene * pScene = CCScene::create();
    TestScrollView * pTestScrollView = TestScrollView::create();
    pTestScrollView->setPosition(ccp(RESOLUTION_SIZE_X / 2, RESOLUTION_SIZE_Y / 2));
    pScene->addChild(pTestScrollView);
    
    
    CCDirector::sharedDirector()->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
}
