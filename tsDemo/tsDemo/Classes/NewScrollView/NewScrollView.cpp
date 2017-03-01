//
//  NewScrollView.cpp
//  test_cocos2dx
//
//  Created by xuyi on 24/01/2017.
//
//

#include "NewScrollView.h"

NewScrollView * NewScrollView::create()
{
    NewScrollView * scroll = new NewScrollView();
    if(scroll)
    {
        scroll->setAnchorPoint(ccp(0, 0));
        scroll->autorelease();
        scroll->setTouchEnabled(true);
        
        scroll->m_pTouches = new CCArray();
    }
    
    return scroll;
}

NewScrollView::NewScrollView()
{
    m_pContainer = nullptr;
}

NewScrollView::~NewScrollView()
{
    
}

void NewScrollView::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, CCLayer::getTouchPriority(), false);
}

void NewScrollView::onEnterTransitionDidFinish()
{
    CCLayer::onEnterTransitionDidFinish();
}

bool NewScrollView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!m_pTouches->containsObject(pTouch))
    {
        m_pTouches->addObject(pTouch);
    }
    
    if (m_pTouches->count() == 1)
    { // scrolling
        m_tTouchPoint     = this->convertTouchToNodeSpace(pTouch);
        m_bTouchMoved     = false;
        m_bDragging     = true; //dragging started
        m_tScrollDistance = ccp(0.0f, 0.0f);
        m_fTouchLength    = 0.0f;
    }
    
    return true;
}

void NewScrollView::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_pTouches->containsObject(pTouch))
    {
        if (m_pTouches->count() == 1 && m_bDragging)
        { // scrolling
            CCPoint moveDistance, newPoint, maxInset, minInset;
            CCRect  frame;
            float newX, newY;
            
//            frame = getViewRect();
            CCPoint screenPos = this->convertToWorldSpace(CCPointZero);
            frame = CCRectMake(screenPos.x, screenPos.y, this->getContentSize().width, this->getContentSize().height);
            
            newPoint     = this->convertTouchToNodeSpace((CCTouch*)m_pTouches->objectAtIndex(0));
            moveDistance = ccpSub(newPoint, m_tTouchPoint);
            
            
            if (!m_bTouchMoved)
            {
                moveDistance = CCPointZero;
            }
            
            m_tTouchPoint = newPoint;
            m_bTouchMoved = true;
            
            if (frame.containsPoint(this->convertToWorldSpace(newPoint)))
            {
                newX     = m_pContainer->getPosition().x + moveDistance.x;
                newY     = m_pContainer->getPosition().y + moveDistance.y;
                
                m_tScrollDistance = moveDistance;
                this->setContentOffset(ccp(newX, newY));
            }
        }
    }
}

void NewScrollView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_pTouches->containsObject(pTouch))
    {
        m_pTouches->removeObject(pTouch);
    }
    
    if (m_pTouches->count() == 0)
    {
        m_bDragging = false;
        m_bTouchMoved = false;
    }
}

void NewScrollView::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void NewScrollView::setContainer(cocos2d::CCNode * pc)
{
    if(nullptr != pc)
    {
        m_pContainer = pc;
        m_pContainer->setAnchorPoint(ccp(0, 0));
        m_pContainer->setPosition(ccp(0, 0));
        this->addChild(m_pContainer);
        
        this->setContentSize(m_pContainer->getContentSize());
    }
}

void NewScrollView::setContentOffset(CCPoint offset)
{
    m_pContainer->setPosition(offset);
}
