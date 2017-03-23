#include "headPage.h"
#include "json.h"
#include "LayerLogin.h"
#include "CData.h"
#include "SocketManager.h"
#include "Singleton.h"
#include "UserData.h"

void HeadPage::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader)
{
    _exp->setScaleX((float)Exp/maxExp);
}

CCNode* HeadPage::getNode(CCNode* no)
{
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("HeadPage", HeadPageLoader::loader());
    
    extension::CCBReader * ccbReader = new extension::CCBReader(ccNodeLoaderLibrary);
    CCNode* node = ccbReader->readNodeGraphFromFile("ccbi/HeadPage.ccbi", no);
    ccbReader->release();
    
    return node;
}

HeadPage::~HeadPage()
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, "refresh_zhuangtai");
}

//只能初始化数据
bool HeadPage::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(HeadPage::upData), "refresh_zhuangtai", NULL);
    
    this->initWithColor(ccc4(100, 256, 100, 25));
    
    return true;
}

void HeadPage::upData()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    Json::FastWriter write;
    Json::Value person;
    
    person["characterId"]=CData::getCData()->getCharactorId();
    std::string json_file1=write.write(person);
    
    SocketManager::getInstance()->sendMessage(json_file1.c_str(), 105);
    this->schedule(schedule_selector(HeadPage::receiveUpdata), 0.2);
}

void HeadPage::receiveUpdata()
{
    Message* revMsg1 = (Message *)CData::getCData()->m_dictionary->objectForKey(105);
    CData::getCData()->m_dictionary->removeObjectForKey(105);
    if(revMsg1)
    {
        this->unschedule(schedule_selector(HeadPage::receiveUpdata));
        Json::Reader reader;
        Json::Value root;
        Json::Value data;
        if(reader.parse(revMsg1->data, root))
        {
            data=root["data"];
            gold=data["gold"].asInt();
            coin=data["coin"].asInt();
            level=data["level"].asInt();
            maxExp=data["maxexp"].asInt();
            Exp=data["exp"].asInt();
            gas= 0;
            power=data["huoli"].asInt();
            this->setCoin(coin);
            
            CData::getCData()->setSendVal(revMsg1->data);
        }
        
        CCLog("%s",revMsg1->data);
        char goldNum[5]="";
        sprintf(goldNum, "%d",gold);
        _goldLabel->setString(goldNum);
        
        char agNum[5]="";
        sprintf(agNum, "%d",coin);
        _ag->setString(agNum);
        
        char gasNum[5]="";
        sprintf(gasNum, "%d",gas);
        _qi->setString(gasNum);
        
        char levelNum[20]="";
        sprintf(levelNum, "%d",level);
        
        levelLabel->setString(levelNum);
        char tiNum[20]="";
        sprintf(tiNum, "%d",power);
        _ti->setString(tiNum);
        
        nameLabel->setString(data["rolename"].asString().c_str());
    }
}

void HeadPage::setGid(int gid)
{
    _gid=gid;
}

int HeadPage::getGid()
{
    return _gid;
}

void HeadPage::reload()
{
    
}

void HeadPage::clkHomeLink()
{
    
}

void HeadPage::clkLinkRecharge()
{
    
}

void HeadPage::initData()
{
    
}

void HeadPage::sendZGBagInfo()
{
    load=Loading::create();
    load->setPosition(ccp(0, -760));
    this->addChild(load,9999);
   
    int characterId=CData::getCData()->getCharactorId();
    Json::FastWriter writer;
    Json::Value sendMessage;
    sendMessage["characterId"]=characterId;
    std::string json_file_zg=writer.write(sendMessage);
    
    SocketManager::getInstance()->sendMessage(json_file_zg.c_str(), 203);
    
    this->schedule(schedule_selector(HeadPage::receiveZGBagInfo),0.5);
}

void HeadPage::receiveZGBagInfo()
{
    
}

void HeadPage::removeB()
{
    CData::getCData()->setyinxiao();
    this->removeChild(backGround, true);
}

int HeadPage::getGas()
{
    return gas;
}

int HeadPage::getMaxExp()
{
    return maxExp;
}

int HeadPage::getExp()
{
    return Exp;
}

int HeadPage::getPower()
{
    return power;
}

int HeadPage::getGold()
{
    return gold;
}

void HeadPage::setCoin(int coin)
{
    __coin=coin;
}

int HeadPage::getcoin()
{
    return __coin;
}

void HeadPage::onExit()
{
    CCLayerColor::onExit();
}

void HeadPage::press_btn_personal(CCObject* pSender)
{
    //this->removelayers();
    //btnEmployFriends->setVisible(false);
    //btnHeCheng->setVisible(false);
    layerPersonInfo = personalinfo::create();
    this->addChild(layerPersonInfo);
}
