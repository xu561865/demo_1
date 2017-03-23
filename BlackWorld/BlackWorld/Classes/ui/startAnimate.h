#ifndef __zd__startAnimate__
#define __zd__startAnimate__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class StartAnimate
: public CCLayer
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
, public CCNodeLoaderListener
{
public:
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode);
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, extension::CCBValue* pCCBValue);
    virtual void onNodeLoaded(CCNode * pNode, extension::CCNodeLoader * pNodeLoader);

    ~StartAnimate();
    
    CREATE_FUNC(StartAnimate);
    virtual bool init();
    static CCScene* getScene();
    
    CCSprite *str1,*str2,*str3,*str4,*str5,*str6,*str7,*str8,*str9;
    
    int round;
    void kong();
    void animateDesc();
    void animateTwo();
    void animateThree();
    void animateFour();
    void callbackSelectRole(CCObject *pSender);
    
};

class StartAnimateLoader : public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(StartAnimateLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(StartAnimate);
};

#endif /* defined(__zd__startAnimate__) */
