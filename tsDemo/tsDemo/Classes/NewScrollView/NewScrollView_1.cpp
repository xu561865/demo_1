//
//  NewScrollView_1.cpp
//  test_cocos2dx
//
//  Created by xuyi on 24/01/2017.
//
//

#include "NewScrollView_1.h"

NewScrollView_1 * NewScrollView_1::create(CCSize size, ccColor4B c4)
{
    NewScrollView_1 * scroll = new NewScrollView_1();
    if(scroll)
    {
        scroll->setContentSize(size);
        scroll->initWithColor(c4, size.width, size.height);
        scroll->setTouchEnabled(true);
        
        scroll->m_pTouches = new CCArray();
        
        scroll->autorelease();
    }
    
    return scroll;
}

NewScrollView_1::NewScrollView_1()
{
    m_pContainer = nullptr;
}

NewScrollView_1::~NewScrollView_1()
{
    
}

void NewScrollView_1::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, CCLayer::getTouchPriority(), false);
}

void NewScrollView_1::onEnterTransitionDidFinish()
{
    CCLayer::onEnterTransitionDidFinish();
}

bool NewScrollView_1::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
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

void NewScrollView_1::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
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

void NewScrollView_1::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
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

void NewScrollView_1::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void NewScrollView_1::setContainer(cocos2d::CCNode * pc)
{
    if(nullptr != pc)
    {
        m_pContainer = pc;
        m_pContainer->setAnchorPoint(ccp(0, 0));
        m_pContainer->setPosition(ccp(0, 0));
        this->addChild(m_pContainer);
    }
}

void NewScrollView_1::setContentOffset(CCPoint offset)
{
    m_pContainer->setPosition(offset);
}

void NewScrollView_1::beforeDraw()
{
    CCRect frame = getViewRect();
    
    glEnable(GL_SCISSOR_TEST);
    
    CCEGLView::sharedOpenGLView()->setScissorInPoints(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
}

void NewScrollView_1::afterDraw()
{
    glDisable(GL_SCISSOR_TEST);
}

void NewScrollView_1::visit()
{
    // quick return if not visible
    if (!isVisible())
    {
        return;
    }
    
    kmGLPushMatrix();
    
    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->beforeDraw();
        this->transformAncestors();
    }
    
    this->transform();
    this->beforeDraw();
    
    if(m_pChildren)
    {
        ccArray *arrayData = m_pChildren->data;
        unsigned int i=0;
        
        // draw children zOrder < 0
        for( ; i < arrayData->num; i++ )
        {
            CCNode *child =  (CCNode*)arrayData->arr[i];
            if ( child->getZOrder() < 0 )
            {
                child->visit();
            }
            else
            {
                break;
            }
        }
        
        // this draw
        this->draw();
        
        // draw children zOrder >= 0
        for( ; i < arrayData->num; i++ )
        {
            CCNode* child = (CCNode*)arrayData->arr[i];
            child->visit();
        }
        
    }
    else
    {
        this->draw();
    }
    
    this->afterDraw();
    if ( m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->afterDraw(this);
    }
    
    kmGLPopMatrix();
}

CCRect NewScrollView_1::getViewRect()
{
    CCPoint screenPos = this->convertToWorldSpace(CCPointZero);
    
    return CCRectMake(screenPos.x, screenPos.y, getContentSize().width, getContentSize().height);
}
