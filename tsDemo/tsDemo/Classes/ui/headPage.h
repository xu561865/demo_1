#ifndef __client1__zhuangtaiLoad__
#define __client1__zhuangtaiLoad__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "SocketClient.h"
#include "message.h"
#include "LodingLayer.h"
#include "personalinfo.h"

//此类需要传递json数据，获取上面的人物资金，体力等
USING_NS_CC;
USING_NS_CC_EXT;

class HeadPage
: public CCLayerColor
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCNodeLoaderListener
{
    personalinfo *layerPersonInfo;
public:
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
    {
        CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_btn_personal", HeadPage::press_btn_personal);
        
        return NULL;
    }
    
    virtual extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
    {
        return NULL;
    }
    
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
    {
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "level", CCLabelTTF*, this->levelLabel);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "name", CCLabelTTF*, this->nameLabel);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "gold", CCLabelTTF*, this->_goldLabel);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "coin", CCLabelTTF*, this->_ag);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "tili", CCLabelTTF*, this->_ti);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "qili", CCLabelTTF*, this->_qi);
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "exp_bar", CCSprite*, this->_exp);
        
        return false;
    }
    
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, extension::CCBValue* pCCBValue)
    {
        return true;
    }
    
    virtual void onNodeLoaded(CCNode * pNode, extension::CCNodeLoader * pNodeLoader);

    virtual bool init();
    CREATE_FUNC(HeadPage);
    virtual void onExit();
    ~HeadPage();
    static CCNode* getNode(CCNode* no);
    
    Loading * load;
    SocketClient * sclient;
    
    void sendZGBagInfo();
    void receiveZGBagInfo();
    CCMenu *backGrounds;
    void removeZGBag();
    void setGid(int gid);
    int getGid();
    int _gid;
    int _goldNum;
    int _agNum;
    int _tiNum;
    int _qiNum;
    int _expNum;
    CCLabelTTF *levelLabel;
    CCLabelTTF * nameLabel;
    CCLabelTTF *_goldLabel;
    CCLabelTTF *_ag;
    CCLabelTTF *_ti;
    CCLabelTTF *_qi;
    
    int coin;
    int level;
    int vipNum;
    int Exp;
    int maxExp;
    int gas;
    int power;
    int gold;
    int __coin;
    
    std::string rolename;
    std::stringstream icoin;
    std::stringstream igold;
    std::stringstream ilevel;
    const  char * _gold;
    const  char * _coin;
    const  char * _level;
    const  char * _rollName;
    void setCoin(int coin);
    int getExp();
    int getGas();
    int getPower();
    int getMaxExp();
    int getGold();
    void openZGBag();
    int getcoin();
    void reload();
    void receiveUpdata();
    void clkLinkRecharge();
    void clkHomeLink();
    void initData();
    
    CCSprite * _exp;
    CCSprite *backGround;

    void removeB();
    void upData();
    
    CCSize size;
    
    void press_btn_personal(CCObject* pSender);
    
};

class HeadPageLoader : public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(HeadPageLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(HeadPage);
};

#endif /* defined(__client1__zhuangtaiLoad__) */
