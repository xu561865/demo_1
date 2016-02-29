#ifndef __client1__friendmain__
#define __client1__friendmain__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

#include "friendemploy.h"
#include "friendemploylog.h"

USING_NS_CC;
using namespace std;

class EmployPage
: public CCLayer
, public extension::CCBSelectorResolver
, public extension::CCBMemberVariableAssigner
, public extension::CCNodeLoaderListener
{
public:
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode);
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, extension::CCBValue* pCCBValue);
    virtual void onNodeLoaded(CCNode * pNode, extension::CCNodeLoader * pNodeLoader);
    
    //~EmployPage();
    CREATE_FUNC(EmployPage);
    static CCScene* scene();
    
private:
    
    virtual bool init();
    
    void showfriendemploy(CCObject* pSender);
    void showfriendemploylog(CCObject* pSender);
    
    friendemploy*       _friendemploy;
    friendemploylog*    _friendemploylog;
};

class EmployPageLoader : public extension::CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(EmployPageLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(EmployPage);
};

#endif /* defined(__client1__friendmain__) */
