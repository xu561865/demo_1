//
//  BottomPage.h
//  tsDemo
//
//  Created by 徐以 on 15-4-15.
//
//

#ifndef __tsDemo__BottomPage__
#define __tsDemo__BottomPage__

#include "cocos2d.h"
#include "cocos-ext.h"


USING_NS_CC;

class BottomPage
: public CCLayer
, public extension::CCBSelectorResolver
, public extension::CCBMemberVariableAssigner
, public extension::CCNodeLoaderListener
{
    
public:
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
    {
        CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_btn_recharge", BottomPage::press_btn_recharge);
        CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_btn_mail", BottomPage::press_btn_mail);
        CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_btn_arena", BottomPage::press_btn_arena);
        CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_btn_home", BottomPage::press_btn_home);
        
        return NULL;
    }
    virtual extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
    {
        return NULL;
    }
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
    {
        return false;
    }
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, extension::CCBValue* pCCBValue)
    {
        return true;
    }
    virtual void onNodeLoaded(CCNode * pNode, extension::CCNodeLoader * pNodeLoader);
    
    virtual bool init();
    CREATE_FUNC(BottomPage);
    virtual void onExit();
    ~BottomPage();
    static CCNode* getNode(CCNode* no);
    
    void press_btn_recharge(CCObject* pSender);
    void press_btn_mail(CCObject* pSender);
    void press_btn_arena(CCObject* pSender);
    void press_btn_home(CCObject* pSender);
    
};

class BottomPageLoader : public extension::CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(BottomPageLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(BottomPage);
};

#endif /* defined(__tsDemo__BottomPage__) */
