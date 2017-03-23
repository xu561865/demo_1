#ifndef __wx__LevelMap__
#define __wx__LevelMap__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "CData.h"

USING_NS_CC;
using namespace std;

class MiddlePage
: public CCLayer
, public extension::CCBSelectorResolver
, public extension::CCBMemberVariableAssigner
, public extension::CCNodeLoaderListener
{
public:
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName)
    {
        return NULL;
    }
    virtual extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName)
    {
        return NULL;
    }
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
    {
        CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "spr_bg", CCSprite*, _spr_bg);
        
        return false;
    }
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, extension::CCBValue* pCCBValue)
    {
        return true;
    }
    virtual void onNodeLoaded(CCNode * pNode, extension::CCNodeLoader * pNodeLoader);
    
    CREATE_FUNC(MiddlePage);
    static CCNode* getNode(CCNode* no);
    
    
    ~MiddlePage();
    virtual bool init();
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
    void adjustMap(bool isResetLevel);
    void clkBuilding(CCMenuItemImage * building);

private:
    CCSprite* _spr_bg;
    
    CCSize size;
    CCSprite* mapSp;
    CCSprite* mapLayer;
    CCPoint startP;
    CCPoint movingP;
    CCPoint endP;
    CCPoint distanceP;
    double offsetY;
    CCMenu* levelsMenu;
    bool isTouchMenu;
    int currentLevelId;
    int currentMap;
    int mapNum;
    CCDictionary* levelSpDic;
    Json::Value  data;
};

class MiddlePageLoader : public extension::CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(MiddlePageLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(MiddlePage);
};

#endif /* defined(__wx__LevelMap__) */
