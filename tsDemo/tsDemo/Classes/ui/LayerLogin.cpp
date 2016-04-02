#include "LayerLogin.h"
#include "homePage.h"
#include "json.h"
#include "LSocketCenter.h"
#include "xuanren.h"
#include "headPage.h"
#include "startAnimate.h"
#include "SocketManager.h"
#include "CData.h"
#include "CustomPop.h"

SEL_MenuHandler LayerLogin::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_btn_login", LayerLogin::menuItemCallbackLogin);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_btn_start", LayerLogin::menuItemCallbackStart);
    
    return NULL;
}

extension::SEL_CCControlHandler LayerLogin::onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
{
    return NULL;
}

bool LayerLogin::onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "login_input", CCSprite*, this->pSpriteDialogLogin);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_login", CCMenuItemImage*, this->pMenuItemLogin);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btn_start", CCMenuItemImage*, this->pMenuItemStart);
    
    return false;
}

bool LayerLogin::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, extension::CCBValue* pCCBValue)
{
    return true;
}

void LayerLogin::onNodeLoaded(cocos2d::CCNode * pNode,  cocos2d::extension::CCNodeLoader * pNodeLoader)
{
    CCSize ms_winSize = CCDirector::sharedDirector()->getWinSize();
    CCScale9Sprite* editbkg = CCScale9Sprite::create();
    editBoxUsername = CCEditBox::create(CCSizeMake(210,50),editbkg);
    editBoxUsername->setReturnType(kKeyboardReturnTypeDone);
    editBoxUsername->setFontSize(12);
    editBoxUsername->setText("");
    editBoxUsername->setFontColor(ccc3(158, 122, 83));
    editBoxUsername->setMaxLength(8);
    editBoxUsername->setPosition(ccp(ms_winSize.width/2-10,96));
    pSpriteDialogLogin->addChild(editBoxUsername,2);
    
    CCScale9Sprite* editbkg1 = CCScale9Sprite::create();
    editBoxPassword = CCEditBox::create(CCSizeMake(190, 50),editbkg1);
    editBoxPassword->setReturnType(kKeyboardReturnTypeDone);
    editBoxPassword->setInputFlag(kEditBoxInputFlagPassword);
    editBoxPassword->setFontColor(ccc3(158, 122, 83));
    editBoxPassword->setMaxLength(8);
    editBoxPassword->setFontSize(8);
    editBoxPassword->setText("");
    editBoxPassword->setPosition(ccp(ms_winSize.width/2-22,45));
    pSpriteDialogLogin->addChild(editBoxPassword,2);
    
    sendData=denglu1;
    
    this->pMenuItemStart->setVisible(false);
}

LayerLogin::LayerLogin()
{
    
}

LayerLogin::~LayerLogin()
{

}

bool LayerLogin::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    //设置音乐
    CData::getCData()->setyinyue("bg.mp3");
    //连接服务器
    SocketManager::getInstance()->startSocket();
    
    return true;
}

void LayerLogin::onEnter()
{
    CCLayer::onEnter();
}

void LayerLogin::onExit()
{
    CCLayer::onExit();
}

void LayerLogin::menuItemCallbackLogin(CCObject* pSender)
{
    if(strcmp(editBoxUsername->getText(), "")&&strcmp(editBoxPassword->getText(), ""))
    {
        const char * userName=editBoxUsername->getText();
        const char * password=editBoxPassword->getText();
        int i=0,j=0,uIllegal=0,pIllegal=0;
        while((userName[i]!='\0'))
        {
            if((userName[i]>='0'&&userName[i]<='9')||(userName[i]>='a'&&userName[i]<='z')||(userName[i]>='A'&&userName[i]<='Z'))
            {
                i++;
                continue;
            }
            else
            {
                i++;
                uIllegal++;
            }
        }
        while(password[j]!='\0')
        {
            if((password[j]>='0'&&password[j]<='9')||(password[j]>='a'&&password[j]<='z')||(password[j]>='A'&&password[j]<='Z'))
            {
                j++;
                continue;
            }
            else
            {
                j++;
                pIllegal++;
            }
        }
        if(uIllegal>0||pIllegal>0)
        {
            CustomPop::show("用户名或密码包含非法字符!~");
        }
        else
        {
            load = Loading::create();
            addChild(load,9999);
            
            
            Json::Value person;
            person["User"]=userName;
            person["Passwd"]=password;
            
            Json::Value RegisterMsg;
            RegisterMsg["RegisterMsg"] = person;
            
            LSocketRequest *req = LSocketCenter::SharedInstance()->login(RegisterMsg);
            req->onSuccess([this](mlib::MSocketRequest *r) {
                M_DEBUG("login success");
                
                
            });
            req->onError([this](mlib::MSocketRequest *r) {
                
            });
            req->isBackground() = true;
            
            CCLog("req0: %x", req);
            req->send();
        }
        
    }
    else
    {
        CustomPop::show("帐号或密码不能为空！~");
    }
}

void LayerLogin::receiveLoginData()
{
    Message* revMsg2 = (Message *)CData::getCData()->m_dictionary->objectForKey(101);
    CData::getCData()->m_dictionary->removeObjectForKey(101);
    
    if(revMsg2)
    {
        this->unschedule(schedule_selector(LayerLogin::receiveLoginData));
        
        char * denglu=revMsg2->data;
        CData::getCData()->setSendVal(denglu1);
        Json::Reader read;
        Json::Value root;
        bool result;
        Json::Value data;
        if(denglu)
        {
            if (read.parse(denglu, root))
            {
                Json::Value data=root["data"];
                std::string message=root["message"].asString();
                result=root["result"].asBool();
                if(result)
                {
                    CData::getCData()->setCharactorId(data["characterId"].asInt());
                    hasRole=data["hasRole"].asBool();
                    CData::getCData()->setUserId(data["userId"].asInt());
                    if(hasRole)
                    {
                        CData::getCData()->setfirstLogin(2);
                        sendPersonalData();
                    }
                    else
                    {
                        CData::getCData()->setfirstLogin(1);
                        
                        load->removeFromParent();
                        CCDirector::sharedDirector()->replaceScene(StartAnimate::getScene());
                    }
                }
                else
                {
                    CustomPop::show(message.c_str());
                    load->removeFromParent();
                }
            }
        }
    }
}

void LayerLogin::sendPersonalData()
{
    this->schedule(schedule_selector(LayerLogin::receivePersonalData), 0.2);
    
    Json::FastWriter write;
    Json::Value person1;
    person1["userId"]= CData::getCData()->getUserId();
    person1["characterId"]=CData::getCData()->getCharactorId();
    std::string json_file1=write.write(person1);
    SocketManager::getInstance()->sendMessage(json_file1.c_str(), 103);
    
    Json::Value citymsg;
    citymsg["userId"] = CData::getCData()->getUserId();
    citymsg["characterId"] = CData::getCData()->getCharactorId();
    citymsg["index"] = 0;
    std::string json_file_city = write.write(citymsg);
}

void LayerLogin::receivePersonalData()
{
    Message* revMsg1 = (Message *)CData::getCData()->m_dictionary->objectForKey(103);
    CData::getCData()->m_dictionary->removeObjectForKey(103);
    if(revMsg1)
    {
        this->unschedule(schedule_selector(LayerLogin::receivePersonalData));
        
        denglu1=revMsg1->data;
        CData::getCData()->setSendVal(denglu1);
        pSpriteDialogLogin->removeFromParent();
        load->removeFromParent();
        
        pMenuItemLogin->setVisible(false);
        pMenuItemStart->setVisible(true);
    }
}

void LayerLogin::menuItemCallbackStart(CCObject *pSender)
{
    CCDirector::sharedDirector()->replaceScene(CCTransitionFlipY::create(2, HomePage::scene(), kCCTransitionOrientationDownOver));
}

CCScene* LayerLogin::scene()
{
    CCScene *pScene = CCScene::create();
    
    extension::CCNodeLoaderLibrary * ccNodeLoaderLibrary = extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("LayerLogin", LayerLoginLoader::loader());
    
    extension::CCBReader * ccbReader = new extension::CCBReader(ccNodeLoaderLibrary);
    CCNode * node = ccbReader->readNodeGraphFromFile("ccbi/Login.ccbi", pScene);
    ccbReader->release();
    if(node != NULL)
    {
        pScene->addChild(node);
    }
    
    return pScene;
}

