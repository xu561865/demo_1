//
//  MScrollView.h
//  legend
//
//  Created by xiaofuan on 13-3-19.
//
//

#ifndef __legend__MScrollView__
#define __legend__MScrollView__

#include <iostream>
#include "mlib_ccext.h"

#define EVENT_SCROLLVIEW_PAGE_CHANGED "event_scrollview_page_changed"
#define EVENT_SCROLLVIEW_START_SCROLLING "event_scrollview_start_scrolling"
#define EVENT_SCROLLVIEW_SCROLLING "event_scrollview_scrolling"

class MScrollView: public cocos2d::extension::CCScrollView, public cocos2d::extension::CCScrollViewDelegate,public mlib::MEventDispatcher
{
public:
    //call this function init container
    // implement the "static node()" method manually
    CREATE_FUNC(MScrollView);
    MScrollView();
    ~MScrollView();
    virtual bool init();
    
    MLIB_DECLARE_PROPERTY(bool, isScrolling);
    
public:
    void scrollViewDidScroll(cocos2d::extension::CCScrollView* view);
    void scrollViewDidZoom(cocos2d::extension::CCScrollView* view);
    void setScroll(bool scroll);
    cocos2d::CCSize getPageSize();
    void setPageSize(cocos2d::CCSize pageSize);
public:
    // touch event function
    virtual void onExit();
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void registerWithTouchDispatcher() override;
    //switch page
    void switchPage(int page, bool animated = true);
//    void scrollTop();
//    void scrollBottom();
    int getCurrentPage() {return _currentPage;}
    int getTotalPage() {return _page;}
    bool isCanSwitchToPrePage();
    bool isCanSwitchToNextPage();
    void setContainer(CCNode * pContainer);
private:
    cocos2d::CCPoint getOffset(int page);
    void adjustHorizontalScrollView();
    void adjustVerticalScrollView();
    //
    void end();
private:
    //view content
    int _page;
    int _currentPage;
    bool _isPage;
    cocos2d::CCPoint _touchPoint;
    cocos2d::CCPoint _endPoint;
    int _touchTime;
    int _endTime;
    cocos2d::CCPoint _startPoint;
    cocos2d::CCSize _pageSize;
    bool _isStartScrolling;
    
    MLIB_DECLARE_PROPERTY(int, pageSwitchCount);
    MLIB_DECLARE_PROPERTY(bool, isSwallowed)
};

MLIB_DECLARE_CCBLOADER(MScrollView, CCLayer)
#endif /* defined(__legend__MScrollView__) */
