//  friendmain.cpp
//  client1
//
//  Created by guoyahui on 13-5-27.


#include "EmployPage.h"
#include "headPage.h"
#include "BottomPage.h"

SEL_MenuHandler EmployPage::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_btn_employ", EmployPage::showfriendemploy);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_btn_employ_log", EmployPage::showfriendemploylog);
    
    return NULL;
}

extension::SEL_CCControlHandler EmployPage::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool EmployPage::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    return false;
}

bool EmployPage::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
    return true;
}

void EmployPage::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader)
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    //加载角色信息
    CCNode* pHeadPage = HeadPage::getNode(this);
    pHeadPage->setPosition(ccp(0, size.height-100));
    this->addChild(pHeadPage,1);
    
    //加载底部按钮
    CCNode* pBottomPage = BottomPage::getNode(this);
    pBottomPage->setPosition(ccp(0, 25));
    this->addChild(pBottomPage, 2);
}

CCScene* EmployPage::scene()
{
    CCScene *pScene = CCScene::create();
    
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("EmployPage", EmployPageLoader::loader());
    
    extension::CCBReader * ccbReader = new extension::CCBReader(ccNodeLoaderLibrary);
    CCNode * node = ccbReader->readNodeGraphFromFile("ccbi/EmployPage.ccbi", pScene);
    ccbReader->release();
    if(node != NULL)
    {
        pScene->addChild(node);
    }
    
    return pScene;
}

bool EmployPage::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    return true;
}

void EmployPage::showfriendemploy(CCObject* pSender)
{
    _friendemploy = friendemploy::create();
    this->addChild(_friendemploy);
}

void EmployPage::showfriendemploylog(CCObject* pSender)
{
    _friendemploylog = friendemploylog::create();
    this->addChild(_friendemploylog);
}

