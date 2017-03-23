//
//  BottomPage.cpp
//  tsDemo
//
//  Created by 徐以 on 15-4-15.
//
//

#include "BottomPage.h"
#include "ArenaWindow.h"
#include "MailPage.h"
#include "homePage.h"


void BottomPage::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader)
{

}

CCNode* BottomPage::getNode(CCNode* no)
{    
    extension::CCNodeLoaderLibrary * ccNodeLoaderLibrary = extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("BottomPage", BottomPageLoader::loader());
    
    extension::CCBReader * ccbReader = new extension::CCBReader(ccNodeLoaderLibrary);
    CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/BottomPage.ccbi", no);
    ccbReader->release();
    
    return node;
}

BottomPage::~BottomPage()
{
    
}

bool BottomPage::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    return true;
}

void BottomPage::onExit()
{
    CCLayer::onExit();
}

//充值
void BottomPage::press_btn_recharge(CCObject* pSender)
{
    //this->removelayers();
    //btnEmployFriends->setVisible(false);
    //btnHeCheng->setVisible(false);
    //rechargeLayer=Recharge::create();
    //this->addChild(rechargeLayer);
}

//邮件
void BottomPage::press_btn_mail(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene(MailPage::scene());
}

//竞技
void BottomPage::press_btn_arena(CCObject* pSender)
{
    //pHomePage->removelayers();
    //layerArena = ArenaWindow::create();
    //this->addChild(layerArena);
}

//主页
void BottomPage::press_btn_home(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene(HomePage::scene());
}

