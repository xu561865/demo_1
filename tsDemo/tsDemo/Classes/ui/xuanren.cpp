#include "xuanren.h"
#include "homePage.h"
#include "LayerLogin.h"
#include "SocketManager.h"
#include "CustomPop.h"
#include "message.h"

SEL_MenuHandler SelectRole::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_btn1", SelectRole::FirstPerson);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_btn2", SelectRole::SecondPerson);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_btn3", SelectRole::ThridPerson);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_btn_select", SelectRole::EnterName);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_btn_ok", SelectRole::replacScene);
    
    return NULL;
}

SEL_CCControlHandler SelectRole::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool SelectRole::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "renwu", CCSprite*, this->renwu);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "jieshao", CCLabelTTF*, this->jieshao);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn1", CCMenuItemImage*, this->btn1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn2", CCMenuItemImage*, this->btn2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn3", CCMenuItemImage*, this->btn3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "pop_node", CCNode*, this->pPopNode);
    
    return false;
}

bool SelectRole::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, cocos2d::extension::CCBValue* pCCBValue)
{
    return true;
}

void SelectRole::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader)
{
    flag=0;
    btn1->selected();
    jieshao->setString("齐天大圣：武僧，拥有高生命高防御");
    pPopNode->setVisible(false);
}

SelectRole::~SelectRole()
{

}

bool SelectRole::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    return true;
}

void SelectRole::onExit()
{

}

void SelectRole::FirstPerson(CCObject* pSender)
{
    if(flag!=1)
    {
        btn1->selected();
        btn2->unselected();
        btn3->unselected();
        
        flag=1;
        jieshao->setString("齐天大圣：武僧，拥有高生命高防御");
        renwu->initWithFile("qs_0001.png");
    }
}

void SelectRole::SecondPerson(CCObject* pSender)
{
    if(flag!=2)
    {
        btn1->unselected();
        btn2->selected();
        btn3->unselected();
        
        flag=2;
        jieshao->setString("蛮族之王：狂战士，拥有高攻击");
        renwu->initWithFile("qs_0002.png");
    }
}

void SelectRole::ThridPerson(CCObject* pSender)
{
    if(flag!=3)
    {
        btn1->unselected();
        btn2->unselected();
        btn3->selected();
        
        flag=3;
        jieshao->setString("恶魔统帅：术士，拥有高速度，可先手攻击");
        renwu->initWithFile("qs_0003.png");
    }
}

void SelectRole::EnterName(CCObject* pSender)
{
    pPopNode->setVisible(true);
    
    CCSize tmpsz = CCDirector::sharedDirector()->getWinSize();
    enterUsrName=CCEditBox::create(CCSizeMake(367, 60), CCScale9Sprite::create("common_pop_text_bkg.png"));
    enterUsrName->setReturnType(kKeyboardReturnTypeDone);
    enterUsrName->setText("");
    enterUsrName->setMaxLength(8);
    enterUsrName->setFontColor(ccc3(10, 10, 0));
    pPopNode->addChild(enterUsrName,4);
    enterUsrName->setPosition(ccp(tmpsz.width/2, tmpsz.height/2));
}

void SelectRole::replacScene(CCObject* pSender)
{
    if(strcmp(enterUsrName->getText(),""))
    {
        load=Loading::create();
        this->addChild(load,9999);
        
        Json::FastWriter  writer;
        Json::Value person;
        person["rolename"]=enterUsrName->getText();
        person["profession"]=flag;
        
        std::string json_file=writer.write(person);
       
        SocketManager::getInstance()->sendMessage(json_file.c_str(), 102);
        
        this->schedule(schedule_selector(SelectRole::receiveRoleInfo),0.2);
    }
    else
    {
        CustomPop::show("名称不能为空");
    }
}

void SelectRole::receiveRoleInfo()
{
    Message * recRoleMsg=(Message *)CData::getCData()->m_dictionary->objectForKey(102);
    CData::getCData()->m_dictionary->removeObjectForKey(102);
    
    if(recRoleMsg)
    {
        char * datastr=recRoleMsg->data;

        Json::Reader read;
        Json::Value root;
        Json::Value data;
        int characterId;
        const char * message="";
        bool result;
        if(datastr)
        {
            this->unschedule(schedule_selector( SelectRole::receiveRoleInfo));
            if (read.parse(datastr, root))
            {
                Json::Value data=root["data"];
                characterId=data["newCharacterId"].asInt();
                CData::getCData()->setCharactorId(characterId);
                
                result=root["result"].asBool();
                message=root["message"].asString().c_str();
                if(result)
                {
                    this->schedule(schedule_selector(SelectRole::receiveHomeInfo), 0.2);
                    
                    Json::FastWriter write;
                    Json::Value person1;
                    
                    person1["userId"]=CData::getCData()->getUserId();
                    person1["characterId"]=characterId;
                    
                    std::string json_file1=write.write(person1);
                         
                    SocketManager::getInstance()->sendMessage(json_file1.c_str(), 103);
                }
                else
                {
                    CustomPop::show("用户名已经拥有了！~");
                    load->removeFromParent();
                }
            }
        }
        else
        {
        }
    }
}

void SelectRole::receiveHomeInfo()
{
    Message * recRoleMsg=(Message *)CData::getCData()->m_dictionary->objectForKey(103);
    CData::getCData()->m_dictionary->removeObjectForKey(103);
    
    if(recRoleMsg)
    {
        this->removeChild(load);
        this->unschedule(schedule_selector(SelectRole::receiveHomeInfo));
        char * data=recRoleMsg->data;
        CData::getCData()->setSendVal(data);
        
        this->removeChild(load);
        this->removeAllChildren();
        CCScene *homePage=CCScene::create();
        homePage->addChild(HomePage::create());
        CCDirector::sharedDirector()->replaceScene(CCTransitionFadeBL::create(2,homePage));
    }
}

CCScene* SelectRole::getScene()
{
    CCScene *pScene = CCScene::create();
    CCScene* pp = CCTransitionFlipY::create(2, pScene, kCCTransitionOrientationDownOver);
    
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("SelectRole", SelectRoleLoader::loader());
    
    extension::CCBReader * ccbReader = new extension::CCBReader(ccNodeLoaderLibrary);
    CCNode * node = ccbReader->readNodeGraphFromFile("ccbi/SelectRole.ccbi", pScene);
    ccbReader->release();
    if(node != NULL)
    {
        pScene->addChild(node);
    }
    
    return pp;
}