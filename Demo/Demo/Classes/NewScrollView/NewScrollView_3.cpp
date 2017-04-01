//
//  NewScrollView_3.cpp
//  test_cocos2dx
//
//  Created by xuyi on 24/01/2017.
//
//

#include "NewScrollView_3.h"

#define SCROLL_DEACCEL_RATE  0.95f
#define SCROLL_DEACCEL_DIST  1.0f
#define BOUNCE_DURATION      0.15f

NewScrollView_3 * NewScrollView_3::create(CCSize size, ccColor4B c4)
{
    NewScrollView_3 * scroll = new NewScrollView_3();
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

NewScrollView_3::NewScrollView_3()
{
    m_pContainer = nullptr;
}

NewScrollView_3::~NewScrollView_3()
{
    
}

void NewScrollView_3::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, CCLayer::getTouchPriority(), false);
}

void NewScrollView_3::onEnterTransitionDidFinish()
{
    CCLayer::onEnterTransitionDidFinish();
}

bool NewScrollView_3::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
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

void NewScrollView_3::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
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
                switch (m_eDirection)
                {
                    case ScrollViewDirection3::kScrollViewDirectionVertical:
                        moveDistance = ccp(0.0f, moveDistance.y);
                        break;
                    case ScrollViewDirection3::kScrollViewDirectionHorizontal:
                        moveDistance = ccp(moveDistance.x, 0.0f);
                        break;
                    default:
                        break;
                }

                
                newX     = m_pContainer->getPosition().x + moveDistance.x;
                newY     = m_pContainer->getPosition().y + moveDistance.y;
                
                m_tScrollDistance = moveDistance;
                this->setContentOffset(ccp(newX, newY));
            }
        }
    }
}

void NewScrollView_3::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!this->isVisible())
    {
        return;
    }
    if (m_pTouches->containsObject(pTouch))
    {
        if (m_pTouches->count() == 1 && m_bTouchMoved)
        {
            this->schedule(schedule_selector(NewScrollView_3::deaccelerateScrolling));
        }
        m_pTouches->removeObject(pTouch);
    }
    
    if (m_pTouches->count() == 0)
    {
        m_bDragging = false;
        m_bTouchMoved = false;
    }
}

void NewScrollView_3::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void NewScrollView_3::deaccelerateScrolling(float dt)
{
    if (m_bDragging)
    {
        this->unschedule(schedule_selector(NewScrollView_3::deaccelerateScrolling));
        return;
    }
    
    float newX, newY;
    CCPoint maxInset, minInset;
    
    m_pContainer->setPosition(ccpAdd(m_pContainer->getPosition(), m_tScrollDistance));
    

    maxInset = this->maxContainerOffset();
    minInset = this->minContainerOffset();
    
    //check to see if offset lies within the inset bounds
    /*
    newX     = MIN(m_pContainer->getPosition().x, maxInset.x);
    newX     = MAX(newX, minInset.x);
    newY     = MIN(m_pContainer->getPosition().y, maxInset.y);
    newY     = MAX(newY, minInset.y);
     */
    
    
    newX = m_pContainer->getPosition().x;
    newY = m_pContainer->getPosition().y;
    
    m_tScrollDistance     = ccpMult(m_tScrollDistance, SCROLL_DEACCEL_RATE);
    
    
//    this->setContentOffset(ccp(newX,newY));
    
    
    if ((fabsf(m_tScrollDistance.x) <= SCROLL_DEACCEL_DIST && fabsf(m_tScrollDistance.y) <= SCROLL_DEACCEL_DIST) ||
        newY > maxInset.y || newY < minInset.y ||
        newX > maxInset.x || newX < minInset.x ||
        newX == maxInset.x || newX == minInset.x ||
        newY == maxInset.y || newY == minInset.y)
    {
        this->unschedule(schedule_selector(NewScrollView_3::deaccelerateScrolling));
        this->relocateContainer(true);
    }
    
}

void NewScrollView_3::relocateContainer(bool animated)
{
    CCPoint oldPoint, min, max;
    float newX, newY;
    
    min = this->minContainerOffset();
    max = this->maxContainerOffset();
    
    oldPoint = m_pContainer->getPosition();
    
    newX     = oldPoint.x;
    newY     = oldPoint.y;
    if (m_eDirection == ScrollViewDirection3::kScrollViewDirectionBoth || m_eDirection == ScrollViewDirection3::kScrollViewDirectionHorizontal)
    {
        newX     = MAX(newX, min.x);
        newX     = MIN(newX, max.x);
    }
    
    if (m_eDirection == ScrollViewDirection3::kScrollViewDirectionBoth || m_eDirection == ScrollViewDirection3::kScrollViewDirectionVertical)
    {
        newY     = MIN(newY, max.y);
        newY     = MAX(newY, min.y);
    }
    
    if (newY != oldPoint.y || newX != oldPoint.x)
    {
        this->setContentOffset(ccp(newX, newY), animated);
    }
}

void NewScrollView_3::setContainer(cocos2d::CCNode * pc)
{
    if(nullptr != pc)
    {
        m_pContainer = pc;
        m_pContainer->setAnchorPoint(ccp(0, 0));
        m_pContainer->setPosition(ccp(0, 0));
        this->addChild(m_pContainer);
    }
}

void NewScrollView_3::setContentOffset(CCPoint offset, bool animated/* = false*/)
{
    if(animated)
    {
        this->setContentOffsetInDuration(offset, BOUNCE_DURATION);
    }
    else
    {
        m_pContainer->setPosition(offset);
        
        if(m_pDelegate)
        {
            m_pDelegate->scrollViewDidScroll(this);
        }
    }
}

void NewScrollView_3::setContentOffsetInDuration(CCPoint offset, float dt)
{
    CCFiniteTimeAction *scroll, *expire;
    
    scroll = CCMoveTo::create(dt, offset);
    expire = CCCallFuncN::create(this, callfuncN_selector(NewScrollView_3::stoppedAnimatedScroll));
    m_pContainer->runAction(CCSequence::create(scroll, expire, NULL));
    this->schedule(schedule_selector(NewScrollView_3::performedAnimatedScroll));
}

void NewScrollView_3::performedAnimatedScroll(float dt)
{
    if (m_bDragging)
    {
        this->unschedule(schedule_selector(NewScrollView_3::performedAnimatedScroll));
        return;
    }
    
    if(m_pDelegate)
    {
        m_pDelegate->scrollViewDidScroll(this);
    }
}

void NewScrollView_3::stoppedAnimatedScroll(CCNode * node)
{
    this->unschedule(schedule_selector(NewScrollView_3::performedAnimatedScroll));
    
    if(m_pDelegate)
    {
        m_pDelegate->scrollViewDidScroll(this);
    }
}

void NewScrollView_3::beforeDraw()
{
    CCRect frame = getViewRect();
    
    glEnable(GL_SCISSOR_TEST);
    
    CCEGLView::sharedOpenGLView()->setScissorInPoints(frame.origin.x, frame.origin.y, frame.size.width, frame.size.height);
}

void NewScrollView_3::afterDraw()
{
    glDisable(GL_SCISSOR_TEST);
}

void NewScrollView_3::visit()
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

CCRect NewScrollView_3::getViewRect()
{
    CCPoint screenPos = this->convertToWorldSpace(CCPointZero);
    
    return CCRectMake(screenPos.x, screenPos.y, getContentSize().width, getContentSize().height);
}

CCPoint NewScrollView_3::maxContainerOffset()
{
    return ccp(0.0f, 0.0f);
}

CCPoint NewScrollView_3::minContainerOffset()
{
    return ccp(getContentSize().width - m_pContainer->getContentSize().width,
               getContentSize().height - m_pContainer->getContentSize().height);
}

CCPoint NewScrollView_3::getContainerPos()
{
    return m_pContainer->getPosition();
}

