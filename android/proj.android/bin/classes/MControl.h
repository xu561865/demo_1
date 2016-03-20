//
//  MControl.h
//  legend
//
//  Created by Xu Jinyang on 4/12/13.
//
//

#ifndef __legend__MControl__
#define __legend__MControl__

#include <iostream>
#include "mlib_ccext.h"

#define EVENT_MCONTROL_TOUCH_BEGAN "event_mcontrol_touch_began"
#define EVENT_MCONTROL_TOUCH_ENDED "event_mcontrol_touch_ended"
#define EVENT_MCONTROL_TAPPED "event_mcontrol_tapped"
#define EVENT_MCONTROL_MOVED "event_mcontrol_moved"
#define EVENT_MCONTROL_DROPPED "event_mcontrol_dropped"
#define EVENT_MCONTROL_EXIT "event_mcontrol_exit"  

MLIB_NS_BEGIN

typedef enum
{
    MToolTipPosition_Default,
    MToolTipPosition_UpAlignLeft,// 在MControl上面，左对齐
    MToolTipPosition_UpAlignRight,// 在MControl上面，右对齐
    MToolTipPosition_UpAlignCenter,// 在MControl上面，居中对齐
} MToolTipPosition;
class MControl;
class MControlListener
{
public:
    // 显示tooltip之前可以指定其位置，否则按照默认处理
    virtual std::tuple<MToolTipPosition, cocos2d::CCSize, float> onPrepareShowToolTip(MControl *mcontrol)
    {
        return std::make_tuple(MToolTipPosition_Default, CCSizeZero, 1);
    };
};
class MControl : public cocos2d::extension::CCControl, public mlib::MEventDispatcher
{
public:
    CREATE_FUNC(MControl);
    MControl();
    ~MControl();
    virtual bool init();
    void registerWithTouchDispatcher();

    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    virtual void onEnter();
    virtual void onExit();
    MLIB_DECLARE_RETAIN_PROPERTY_SETTER_TO_IMPL(cocos2d::CCNode *, tooltip);
    MLIB_DECLARE_PROPERTY(bool, showTooltipOnTapped);
    MLIB_DECLARE_PROPERTY(bool, isDragable);
    MLIB_DECLARE_PROPERTY(bool, popupCenter);
    MLIB_DECLARE_PROPERTY(bool, popupBottom);

protected:
    void onTouchBegan(mlib::MEvent *evt);
    void onTouchEnded(mlib::MEvent *evt);
    void onTapped(mlib::MEvent *evt);
    
    bool _isMoved;
    int _oldZOrder;
    
    MLIB_DECLARE_WEAK_PROPERTY(mlib::MControlListener *, listener);
};

MLIB_DECLARE_CCBLOADER(MControl, CCControl);

MLIB_NS_END

#endif /* defined(__legend__MControl__) */
