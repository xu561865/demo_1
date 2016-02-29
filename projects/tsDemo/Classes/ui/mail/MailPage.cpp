#include "MailPage.h"
#include "SocketManager.h"
#include "CData.h"
#include "json.h"
#include <limits>
#include "MailInfo.h"
#include "MailRead.h"
#include "MailWrite.h"
#include "headPage.h"
#include "BottomPage.h"

SEL_MenuHandler MailPage::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

extension::SEL_CCControlHandler MailPage::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool MailPage::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    return false;
}

bool MailPage::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
    return true;
}

void MailPage::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader)
{
    this->setTouchEnabled(true);
    
    size = CCDirector::sharedDirector()->getWinSize();
    
    //加载角色信息
    CCNode* pHeadPage = HeadPage::getNode(this);
    pHeadPage->setPosition(ccp(0, size.height-100));
    this->addChild(pHeadPage,1);
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("refresh_zhuangtai");
    
    //加载底部按钮
    CCNode* pBottomPage = BottomPage::getNode(this);
    pBottomPage->setPosition(ccp(0, 25));
    this->addChild(pBottomPage, 2);
    
    arr= CCArray::create();
    arr->retain();
    tableView = CCTableView::create(this,CCSizeMake(0, 0));
    tableView->setViewSize(CCSizeMake(420, 420));
    tableView->setContentOffset(ccp(0, 400-140));
    tableView->setVerticalFillOrder(kCCTableViewFillBottomUp);
    tableView->setAnchorPoint(ccp(0, 0));
    tableView->setPosition(ccp(30,210));
    tableView->setDelegate(this);
    this->addChild(tableView);
    
    Json::FastWriter writer;
    Json::Value root;
    
    root["characterId"]=CData::getCData()->getCharactorId();
    
    string json_file=writer.write(root);
    SocketManager::getInstance()->sendMessage(json_file.c_str(), 501);
    this->schedule(schedule_selector(MailPage::rec_501));
    load = Loading::create();
    this->addChild(load, 9999);
}

CCScene* MailPage::scene()
{
    CCScene *pScene = CCScene::create();
    
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("MailPage", MailPageLoader::loader());
    
    extension::CCBReader * ccbReader = new extension::CCBReader(ccNodeLoaderLibrary);
    CCNode * node = ccbReader->readNodeGraphFromFile("ccbi/MailPage.ccbi", pScene);
    ccbReader->release();
    if(node != NULL)
    {
        pScene->addChild(node);
    }
    
    return pScene;
}

bool MailPage::init()
{
    if (!CCLayer::init())
    {
        return false;
    }

    return true;
}

void MailPage::clk_btn_write()
{
    MailWrite::show();
}

void MailPage::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,(std::numeric_limits<int>::min()), true);
    CCLayer::registerWithTouchDispatcher();
}

bool MailPage::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    istouch=tableView->ccTouchBegan(touch, event);
    if(istouch)
    {
        return true;
    }
    else
    {
        return false;
    }
    
    return true;
}

void MailPage::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    if(istouch)
    {
        tableView->ccTouchMoved(touch, event);
    }
}

void MailPage::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    if (istouch)
    {
        tableView->ccTouchEnded(touch, event);
        
        istouch=false;
    }
}

void MailPage::scrollViewDidScroll(CCScrollView* view)
{
    
}

void MailPage::scrollViewDidZoom(CCScrollView* view)
{
    
}

unsigned int MailPage::numberOfCellsInTableView(CCTableView *table)
{
    return arr->count();
}

void MailPage::send_505()
{
    
}

void MailPage::rec_505()
{
    Message *revMsg=(Message *)CData::getCData()->m_dictionary->objectForKey(505);
    CData::getCData()->m_dictionary->removeObjectForKey(505);
    
    if(revMsg)
    {
        load->removeFromParent();
        
        this->unschedule(schedule_selector(MailPage::rec_505));
        
        char* data=revMsg->data;
        CCLog("%s",data);
        Json::Reader read;
        Json::Value root;
        Json::Value jsonData;
        
        if(read.parse(data, root))
        {
            jsonData=root["data"];
            MailInfo* info = (MailInfo*)(arr->objectAtIndex(tmpid));
            info->isReaded = 1;
            info->content = jsonData["content"].asString();
            
            tableView->reloadData();
            
            MailRead::show(info);
        }
    }
}

//处理触摸事件，可以计算点击的是哪一个子项
void MailPage::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    CCLog("点击了  %d",cell->getIdx());

    load = Loading::create();
    this->addChild(load, 9999);
    
    Json::FastWriter writer;
    Json::Value root;
    
    root["characterId"]=CData::getCData()->getCharactorId();
    root["mailID"] = ((MailInfo*)(arr->objectAtIndex(cell->getIdx())))->mailId;

    string json_file=writer.write(root);
    SocketManager::getInstance()->sendMessage(json_file.c_str(), 505);
    this->schedule(schedule_selector(MailPage::rec_505));
    
    tmpCell = (MailRenderer*)cell;
    tmpid = cell->getIdx();
}

//每一项的宽度和高度
CCSize MailPage::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(420, 80);
}

//生成列表每一项的内容
CCTableViewCell* MailPage::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    MailRenderer *cell = NULL;
    // table->dequeueCell();
    cell =new MailRenderer();
    cell->setInfo((MailInfo*)(arr->objectAtIndex(idx)));
    return cell;
}

void MailPage::rec_501()
{
    Message *revMsg=(Message *)CData::getCData()->m_dictionary->objectForKey(501);
    CData::getCData()->m_dictionary->removeObjectForKey(501);
    
    if(revMsg)
    {
        load->removeFromParent();
        
        this->unschedule(schedule_selector(MailPage::rec_501));
        char* data=revMsg->data;
        CCLog("%s",data);
        Json::Reader read;
        Json::Value root;
        Json::Value jsonData;
        Json::Value petlist;
        
        if(read.parse(data, root))
        {
            jsonData=root["data"];
            petlist=jsonData["maillist"];
            
            int len = petlist.size();
            
            for (int i=0; i<len; i++)
            {
                MailInfo* info = new MailInfo();
                info->autorelease();
                
                info->mailId = petlist[i]["id"].asInt();
                info->isReaded = petlist[i]["isReaded"].asInt();
                info->sender = petlist[i]["sender"].asString();
                info->title = petlist[i]["title"].asString();
                info->sendTime = petlist[i]["sendTime"].asString();

                arr->addObject(info);
          
            }
        }
        tableView->reloadData();
    }
}