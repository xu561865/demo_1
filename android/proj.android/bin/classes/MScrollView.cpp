//
//  MScrollView.cpp
//  legend
//
//  Created by xiaofuan on 13-3-19.
//
//
#include "cocos2d.h"
#include "MScrollView.h"

USING_NS_CC;
USING_NS_CC_EXT;

MLIB_REGISTER_CCBLOADER(MScrollView)

static const int kMTouchTime = 150;
static const float kMAnimDelay = 0.2;
static const int kMMicsecond = 1000;

MScrollView::MScrollView()
{
    _isSwallowed = false;
    _isScrolling = false;
    _pageSize = CCSizeZero;
    _pageSwitchCount = 1;
}

MScrollView::~MScrollView()
{
    
}

bool MScrollView::init()
{
    if (!CCScrollView::init())
    {
        return false;
    }
    
    setDelegate(this);
    _currentPage = 1;
    _startPoint = CCPointZero;
    
    return true;
}

void MScrollView::scrollViewDidScroll(cocos2d::extension::CCScrollView* view)
{
    dispatchEvent(EVENT_SCROLLVIEW_SCROLLING);
    if (_isScrolling)
    {
        if (getOffset(_currentPage).equals(getContentOffset()))
        {
            _isScrolling = false;
            dispatchEvent(EVENT_SCROLLVIEW_PAGE_CHANGED);
        }
    }
}
void MScrollView::scrollViewDidZoom(cocos2d::extension::CCScrollView* view)
{
    
}

void MScrollView::setScroll(bool isPage)
{
     _isPage = isPage;
    if (_isPage)
    {
        if (this->getDirection() == cocos2d::extension::CCScrollViewDirection::kCCScrollViewDirectionVertical) {
            int contentHeight = getContentSize().height;
            int viewHeight = getViewSize().height;
            int pageHeight = getPageSize().height;
            int reminder ;
            reminder = (contentHeight - viewHeight) % pageHeight;
            //_currentPage number
            if (reminder == 0) {
                _page = 1 + (contentHeight - viewHeight) / pageHeight;
            }
            else
            {
                _page = 1 + (contentHeight - viewHeight) / pageHeight + 1;
            }
        }else if (this->getDirection() == cocos2d::extension::CCScrollViewDirection::kCCScrollViewDirectionHorizontal) {
            int contentWidth = getContentSize().width;
            int viewWidth = getViewSize().width;
            int pageWidth = getPageSize().width;
            //_currentPage number
            int reminder;
            reminder = (contentWidth - viewWidth) % pageWidth;
            if (reminder == 0) {
                _page = 1 + (contentWidth - viewWidth) / pageWidth;
            }
            else
            {
                _page = 1 + (contentWidth - viewWidth) / pageWidth + 1;
            }
        }
    }
}

void MScrollView::onExit()
{
    CCLayer::onExit();
}

bool MScrollView::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (_isPage)
    {
        if (_isScrolling)
        {
            return false;
        }
        cocos2d::CCPoint point = getPosition();
        struct timeval tvpre;
        gettimeofday(&tvpre, nullptr);
        _startPoint = getContentOffset();
        _touchTime = tvpre.tv_usec / kMMicsecond + tvpre.tv_sec * kMMicsecond;//touch time begin
        _touchPoint = pTouch->getLocation();
    }
    
    _isStartScrolling = true;
    return cocos2d::extension::CCScrollView::ccTouchBegan(pTouch, pEvent);
}

void MScrollView::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    cocos2d::extension::CCScrollView::ccTouchMoved(pTouch, pEvent);
    
    if (_isStartScrolling && m_bTouchMoved)
    {
        dispatchEvent(EVENT_SCROLLVIEW_START_SCROLLING);
        _isStartScrolling = false;
    }
}

void MScrollView::end()
{
    if (this->getDirection() == cocos2d::extension::CCScrollViewDirection::kCCScrollViewDirectionVertical) {
        adjustVerticalScrollView();
    }
    if (this->getDirection() == cocos2d::extension::CCScrollViewDirection::kCCScrollViewDirectionHorizontal) {
        adjustHorizontalScrollView();
    }
}

void MScrollView::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (!this->isVisible())
    {
        return;
    }
    if (_isPage) {
        struct timeval tvpre;
        gettimeofday(&tvpre, nullptr);
        _endTime = tvpre.tv_usec / kMMicsecond + tvpre.tv_sec * kMMicsecond;
        _endPoint = pTouch->getLocation();
        if (m_pTouches->containsObject(pTouch))
        {
            if (m_pTouches->count() == 1 && m_bTouchMoved)
            {
                end();
            }
            m_pTouches->removeObject(pTouch);
        }
        
        if (m_pTouches->count() == 0)
        {
            m_bDragging = false;
            m_bTouchMoved = false;
        }
    }
    else
    {
        cocos2d::extension::CCScrollView::ccTouchEnded(pTouch, pEvent);
    }
}

void MScrollView::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (_isPage)
    {
        end();
    }
    cocos2d::extension::CCScrollView::ccTouchCancelled(pTouch, pEvent);
}

void MScrollView::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, CCLayer::getTouchPriority(), isSwallowed());
}

// 实现可能有问题
//void MScrollView::scrollTop()
//{
//    if (_isPage)
//    {
//        switchPage(_page);
//    }
//    else
//    {
//        cocos2d::CCPoint adjustPos;
//        //vertical
//        if (this->getDirection() == cocos2d::extension::CCScrollViewDirection::kCCScrollViewDirectionVertical)
//        {
//            adjustPos.y = getPositionY() - (getContentSize().height - getViewSize().height);
//            adjustPos.x = getContentOffset().x;
//        }
//        //horizontal
//        if (this->getDirection() == cocos2d::extension::CCScrollViewDirection::kCCScrollViewDirectionHorizontal)
//        {
//            adjustPos.y = getContentOffset().y;
//            adjustPos.x = getPositionX() - (getContentSize().width - getViewSize().width);
//        }
//        _currentPage = _page;
//        setContentOffsetInDuration(adjustPos, kMAnimDelay);
//    }
//}
//
//void MScrollView::scrollBottom()
//{
//    if (_isPage)
//    {
//        switchPage(1);
//    }
//    else
//    {
//        cocos2d::CCPoint adjustPos;
//        adjustPos = getPosition();
//        _currentPage = 1;
//        setContentOffsetInDuration(adjustPos, kMAnimDelay);
//    }
//}

cocos2d::CCPoint MScrollView::getOffset(int page)
{
    CCSize viewSize = getViewSize();
    CCSize pageSize = getPageSize();
    
    if (getDirection() == CCScrollViewDirection::kCCScrollViewDirectionVertical)
    {
        return ccp(0, (page - 1) * pageSize.height + viewSize.height - getContentSize().height);
    }
    else if (getDirection() == CCScrollViewDirection::kCCScrollViewDirectionHorizontal)
    {
        return ccp((1 - page) * pageSize.width, 0);
    }
    else
    {
        M_ASSERT(0);
        return CCPointZero;
    }
}

void MScrollView::switchPage(int page, bool animated/* = true*/)
{
    if (page < 1)
    {
        page = 1;
    }
    if (page > _page)
    {
        page = _page;
    }
    
    cocos2d::CCPoint adjustPos = getOffset(page);
    
    _currentPage = page;
    
//    M_DEBUG("oldPos: " << getContentOffset().x << "  newPos: " << adjustPos.x);
    
    if (animated)
    {
        setContentOffsetInDuration(adjustPos, kMAnimDelay);
        _isScrolling = true;
    }
    else
    {
        setContentOffset(adjustPos);
    }
}

void MScrollView::adjustVerticalScrollView()
{
    unscheduleAllSelectors();
    int height = getPageSize().height;
//    float y = getContentOffset().y;
//    int offset = (int)y % height;
    cocos2d::CCPoint adjustPos;
    int elapseTime = ( _endTime - _touchTime);
    int page = _currentPage;
    if(elapseTime > 0 && elapseTime < kMTouchTime)
    {
        if (_touchPoint.y == _endPoint.y) {
            return;
        }
        if (
            ((_touchPoint.y < _endPoint.y)
             && (_currentPage == 1)
             )
            || ((_touchPoint.y > _endPoint.y) && _currentPage == _page)
            )
        {
            switchPage(page);
        }
        else
        {
            //up
            if (_touchPoint.y < _endPoint.y) {
                switchPage(page - _pageSwitchCount);
            }
            //down
            if (_touchPoint.y > _endPoint.y) {
               switchPage(page + _pageSwitchCount);
            }
        }
    }
    else
    {
        float distance;
        distance = _touchPoint.y - _endPoint.y;
        if (fabs(distance) < (height / 2)) {
            switchPage(page);
        }
        else
        {
            if (((distance < 0) && (_currentPage == 1)) || ((distance > 0) && (_currentPage == _page)))
            {
                switchPage(page);
            }
            else
            {
                if (distance < 0) {
                    switchPage(page - _pageSwitchCount);
                }
                if (distance > 0) {
                    switchPage(page + _pageSwitchCount);
                }
            }
        }
    }
}
void MScrollView::adjustHorizontalScrollView()
{
    unscheduleAllSelectors();
    int width = getPageSize().width;
    //cocos2d::CCPoint adjustPos;
    int elapseTime = ( _endTime - _touchTime);
    int page = _currentPage;
    if(elapseTime > 0 && elapseTime < kMTouchTime)
    {
        if (_touchPoint.x == _endPoint.x)
        {
            return;
        }
        if (((_touchPoint.x < _endPoint.x) && (page == 1)) ||
            ((_touchPoint.x > _endPoint.x) && (page == _page)))
        {
            switchPage(page);
            return ;
        }
        //right
        if (_touchPoint.x < _endPoint.x)
        {
            switchPage(page-_pageSwitchCount);
            return ;
        }
        //left
        if (_touchPoint.x > _endPoint.x)
        {
            switchPage(page+_pageSwitchCount);
            return ;
        }
    }
    else
    {
        float distance;
        distance = _touchPoint.x - _endPoint.x;
        if (((distance < 0) && (page == 1)) ||
            ((distance > 0) && (page == _page)))
        {
            switchPage(page);
            return;
        }
        if (fabs(distance) < (width / 2))
        {
            switchPage(page);
        }
        else
        {
            
            if (distance < 0)
            {
                switchPage(page-_pageSwitchCount);
            }
            if (distance > 0)
            {
                switchPage(page + _pageSwitchCount);
            }
        }
    }
}


bool MScrollView::isCanSwitchToPrePage()
{
    return _page > 0 && _currentPage > 1;
}


bool MScrollView::isCanSwitchToNextPage()
{
    return _page > 0 && _currentPage < _page;
}


void MScrollView::setContainer(CCNode * pContainer)
{
    CCScrollView::setContainer(pContainer);
    setContentOffset(ccp(0,0));
    _currentPage = 1;
    setScroll(_isPage);
}

cocos2d::CCSize MScrollView::getPageSize()
{
    if (_pageSize.equals(CCSizeZero))
    {
        _pageSize = getViewSize();
    }
    return _pageSize;
}

void MScrollView::setPageSize(cocos2d::CCSize pageSize)
{
    _pageSize = pageSize;
}
