#include "homePage.h"
#include "CCScrollView.h"
#include "json.h"
#include "SocketManager.h"
#include "CustomPop.h"

#include "headPage.h"
#include "BottomPage.h"
#include "middlePage.h"
#include "CombinePage.h"
#include "EmployPage.h"


void HomePage::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader)
{
    setTouchEnabled(true);
    
    size = CCDirector::sharedDirector()->getWinSize();
    
    load=Loading::create();
    this->addChild(load,9999);
    
    int characterId=CData::getCData()->getCharactorId();
    Json::FastWriter writer;
    Json::Value sendMessage;
    sendMessage["characterId"]=characterId;
    sendMessage["index"]=0;
    std::string json_file_jianghu=writer.write(sendMessage);
    SocketManager::getInstance()->sendMessage(json_file_jianghu.c_str(), 4500);
    
    this->schedule(schedule_selector(HomePage::rec_4500));
}

CCScene* HomePage::scene()
{
    CCScene *pScene = CCScene::create();
    
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("HomePage", HomePageLoader::loader());
    
    extension::CCBReader * ccbReader = new extension::CCBReader(ccNodeLoaderLibrary);
    CCNode * node = ccbReader->readNodeGraphFromFile("ccbi/HomePage.ccbi", pScene);
    ccbReader->release();
    if(node != NULL)
    {
        pScene->addChild(node);
    }
    
    return pScene;
}

HomePage::HomePage()
{
    
}

HomePage::~HomePage()
{
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

bool HomePage::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    return true;
}

void HomePage::rec_4500()
{
    Message *revMsg=(Message *)CData::getCData()->m_dictionary->objectForKey(4500);
    CData::getCData()->m_dictionary->removeObjectForKey(4500);
    
    if(revMsg)
    {
        this->unschedule(schedule_selector(HomePage::rec_4500));
        
        CData::getCData()->cityData = revMsg->data;
        load->removeFromParent();
        
        //加载角色信息
        CCNode* pHeadPage = HeadPage::getNode(this);
        pHeadPage->setPosition(ccp(-1, size.height - 118));
        this->addChild(pHeadPage,1);
        
        CCNotificationCenter::sharedNotificationCenter()->postNotification("refresh_zhuangtai");
        
        //加载中部地图
        CCNode* pMiddlePage = MiddlePage::getNode(this);
        this->addChild(pMiddlePage,-1);
        
        //加载底部按钮
        CCNode* pBottomPage = BottomPage::getNode(this);
        pBottomPage->setPosition(ccp(0, 25));
        this->addChild(pBottomPage, 2);
        
        if (CData::getCData()->isArenaFire)
        {
            //this->openArenaWindow();
        }
    }
}

//合成
void HomePage::press_btn_combine(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene(CombinePage::scene());
}

//雇佣
void HomePage::press_btn_employ(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene(EmployPage::scene());
    //CustomPop::show("敬请期待");
}

void HomePage::onExit()
{
    CCLayer::onExit();
}
