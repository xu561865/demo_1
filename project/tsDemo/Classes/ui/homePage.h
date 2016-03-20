//  homePage.h
//  Client
//
//  Created by lh on 13-2-22.

#ifndef __Client__homePage__
#define __Client__homePage__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "CCScrollView.h"

#include "LayerLogin.h"
#include "LodingLayer.h"
#include "popwindow.h"
#include "newBuZhen.h"


USING_NS_CC;
USING_NS_CC_EXT;

class HomePage
: public CCLayer
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCNodeLoaderListener
{    
    CCSize size;
    Loading * load;
    
    void recPersonal();
    void recEquips();
    void rec_4500();
    
    void press_btn_combine(CCObject* pSender);
    void press_btn_employ(CCObject* pSender);
    
public:
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
    {
        CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_btn_combine", HomePage::press_btn_combine);
        CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "press_btn_employ", HomePage::press_btn_employ);
        
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
    
    HomePage();
    ~HomePage();
    virtual bool init();
    CREATE_FUNC(HomePage);
    virtual void onExit();
    static CCScene* scene();

};

class HomePageLoader : public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(HomePageLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(HomePage);
};

#endif /* defined(__Client__homePage__) */
