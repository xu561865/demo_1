#include "LayerLogin.h"
#include "homePage.h"
#include "json.h"
#include "SocketCenter.h"
#include "xuanren.h"
#include "headPage.h"
#include "startAnimate.h"
#include "SocketManager.h"
#include "CData.h"
#include "CustomPop.h"
#include "utils.h"
#include "Memory.h"

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

void LayerLogin::menuItemCallbackStart(CCObject *pSender)
{
    CCDirector::sharedDirector()->replaceScene(CCTransitionFlipY::create(2, HomePage::scene(), kCCTransitionOrientationDownOver));
}

void LayerLogin::menuItemCallbackLogin(CCObject* pSender)
{
    if(strcmp(editBoxUsername->getText(), "") && strcmp(editBoxPassword->getText(), ""))
    {
        const char * userName=editBoxUsername->getText();
        const char * password=editBoxPassword->getText();
        
        
        if(!utils::checkUserName(userName)||!utils::checkPwd(password))
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
            
            MEM_INFO->userId(userName);
            MEM_INFO->userPwd(password);
            
            Json::Value RegisterMsg;
            RegisterMsg["RegisterMsg"] = person;
            
            SocketRequest *req = SocketCenter::SharedInstance()->userRegister(RegisterMsg);
            req->onSuccess([this, userName, password](mlib::MSocketRequest *r) {
                
                login();
                
            });
            req->onError([this](mlib::MSocketRequest *r) {
                
            });
            req->isBackground() = true;
            
            req->send();
        }
        
    }
    else
    {
        CustomPop::show("帐号或密码不能为空！~");
    }
}

void LayerLogin::login()
{
    Json::Value person;
    person["User"] = MEM_INFO->userId();
    person["Passwd"] = MEM_INFO->userPwd();
    
    Json::Value LoginMsg;
    LoginMsg["LoginMsg"] = person;
    
    SocketRequest *req = SocketCenter::SharedInstance()->userLogin(LoginMsg);
    req->onSuccess([this](mlib::MSocketRequest *r) {
        
        
    });
    req->onError([this](mlib::MSocketRequest *r) {
        
    });
    req->isBackground() = true;
    
    req->send();
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
