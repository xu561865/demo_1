#ifndef __wx__MailWindow__
#define __wx__MailWindow__

#include <iostream>

#include "cocos2d.h"
#include "cocos-ext.h"
#include "MailRenderer.h"
#include "LodingLayer.h"

USING_NS_CC;

class MailPage
: public CCLayer
, public extension::CCBSelectorResolver
, public extension::CCBMemberVariableAssigner
, public extension::CCNodeLoaderListener
,public CCTableViewDelegate
,public CCTableViewDataSource
{
public:
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char * pSelectorName);
    virtual extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char * pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode);
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, extension::CCBValue* pCCBValue);
    virtual void onNodeLoaded(CCNode * pNode, extension::CCNodeLoader * pNodeLoader);

    CREATE_FUNC(MailPage);
    static CCScene* scene();
    
private:
    
    virtual bool init();
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
    
    //处理触摸事件，可以计算点击的是哪一个子项
    virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    //每一项的宽度和高度
    virtual CCSize cellSizeForTable(CCTableView *table);
    //生成列表每一项的内容
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    //一共多少项
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
    void rec_501();
    void send_505();
    void rec_505();
    void clk_btn_write();
    
    MailRenderer* tmpCell;
    int tmpid;
    Loading* load;
    CCTableView* tableView;
    CCSize size;
    CCArray* arr;
    bool istouch;

};

class MailPageLoader : public CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(MailPageLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(MailPage);
};

#endif /* defined(__wx__MailWindow__) */
