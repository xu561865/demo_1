//
//  MDragDownView.h
//  legend
//
//  Created by Chen Wei on 14-10-10.
//
//

#ifndef __legend__MDragDownView__
#define __legend__MDragDownView__


#include "mlib_ccext.h"


#define DRAGDOWNVIEW_DURATION      0.5f
#define DRAGDOWNVIEW_TOUCH_CLICK_DIS  5.0f


#define EVENT_MDRAGDOWNVIEW_SHOW_BEGIN "event_mdragdownview_show_begin"
#define EVENT_MDRAGDOWNVIEW_SHOW_DOING "event_mdragdownview_show_doing"
#define EVENT_MDRAGDOWNVIEW_SHOW_END "event_mdragdownview_show_end"
#define EVENT_MDRAGDOWNVIEW_HIDE_BEGIN "event_mdragdownview_hide_begin"
#define EVENT_MDRAGDOWNVIEW_HIDE_DOING "event_mdragdownview_hide_doing"
#define EVENT_MDRAGDOWNVIEW_HIDE_END "event_mdragdownview_hide_end"


class MDragDownView;
class MDragDownViewListener
{
public:
    virtual void dragDownViewAction(MDragDownView *pView){};
};

class MDragDownView : public CCLayer, public mlib::MEventDispatcher
{
public:
    // 显示、隐藏时的状态
    enum class State
    {
        NONE,
        SHOW_BEGIN,
        SHOW_DOING,
        SHOW_END,
        HIDE_BEGIN,
        HIDE_DOING,
        HIDE_END,
        DRAGING,
    };
    
    
protected:
    float m_fViewWith; // 可视宽度
    float m_fViewMinHeight; // 可视最小高度
    float m_fViewMaxHeight; // 可视最大高度
    CCSize m_viewSize; // 当前可视尺寸
    CCSize m_viewOldSize;
    CCRect m_viewRect; // 当前可视区域（世界坐标）
    CCRect m_dragBarRect; // 拖动栏可视区域（世界坐标）
    bool m_isShow; // 是否显示（展开）
    bool m_isTouchDragBar; // 是否点击了拖动栏
    State m_eState; // 当前状态
    float m_fDefaultDuration; // 收起或展开的默认持续时间
    float m_fDuration; // 收起或展开的持续时间
    float m_fDurationTimer; // 收起或展开的持续时间计时器
    float m_fViewOffset; // 动画收起或展开时当前位置与目标位置的距离
    CCPoint m_touchBeginPosition;
    
    CCNode *m_pContainer;
    CCNode *m_pDragBar;
    
    MLIB_DECLARE_WEAK_PROPERTY(MDragDownViewListener *, listener);
    
    
public:
    CREATE_FUNC(MDragDownView);
    static MDragDownView *create(float fViewWith, float fViewMinHeight, float fViewMaxHeight, CCNode *pContainer = nullptr);
    
    MDragDownView();
    virtual ~MDragDownView();
    virtual bool init();
    virtual bool init(float fViewWith, float fViewMinHeight, float fViewMaxHeight, CCNode *pContainer = nullptr);
    
    
    virtual void setViewSize(float fViewWith, float fViewMinHeight, float fViewMaxHeight);
    virtual float getViewWidth();
    virtual float getViewMinHeight();
    virtual float getViewMaxHeight();
    virtual const CCSize & getVisibleSize() const { return m_viewSize; }
    virtual void setContainer(CCNode *pContainer);
    virtual CCNode *getContainer();
    virtual void setDragBar(CCNode *pDragBar);
    virtual State getState();
    virtual bool isShow();
    virtual void setDefaultDuration(float fDuration);
    virtual float getDefaultDuration(){ return m_fDefaultDuration; }
    
    virtual void refresh();
    virtual void show(float fDuration = 0.0f);
    virtual void hide(float fDuration = 0.0f);
    
protected:
    virtual void action(bool isShow, float fDuration = 0.0f);
    virtual void adjustByTouch(CCTouch *pTouch);
    virtual void updateRect();
    virtual void setViewVisibleOffset(float fOffset);
    virtual void adjustContainer();
    virtual void adjustDragBar();
    
    
public:
    virtual void step(float dt);
    
    virtual void setContentSize(const CCSize & size);
    virtual const CCSize& getContentSize() const;
    
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void visit();
    virtual void onEnter();
    virtual void onEnterTransitionDidFinish();
    virtual void onExit();
    virtual void onExitTransitionDidStart();
    
    virtual void addChild(CCNode *pChild, int nZOrder, int nTag);
    virtual void addChild(CCNode *pChild, int nZOrder);
    virtual void addChild(CCNode *pChild);
};


MLIB_DECLARE_CCBLOADER(MDragDownView, CCLayer)


#endif /* defined(__legend__MDragDownView__) */
