//
//  MDragDownView.cpp
//  legend
//
//  Created by Chen Wei on 14-10-10.
//
//

#include "MDragDownView.h"


MLIB_REGISTER_CCBLOADER(MDragDownView)


MDragDownView* MDragDownView::create(float fViewWith, float fViewMinHeight, float fViewMaxHeight, CCNode *pContainer)
{
    MDragDownView *pRet = new MDragDownView();
    if (pRet && pRet->init(fViewWith, fViewMinHeight, fViewMaxHeight, pContainer))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}


MDragDownView::MDragDownView()
{
    m_fViewWith = 0.0f;
    m_fViewMinHeight = 0.0f;
    m_fViewMaxHeight = 0.0f;
    m_viewSize.setSize(0, 0);
    m_viewOldSize.setSize(0, 0);
    m_viewRect.setRect(0, 0, 0, 0);
    m_dragBarRect.setRect(0, 0, 0, 0);
    m_isShow = false;
    m_isTouchDragBar = false;
    m_eState = State::NONE;
    m_fDefaultDuration = DRAGDOWNVIEW_DURATION;
    m_fDuration = m_fDefaultDuration;
    m_fDurationTimer = 0.0f;
    m_fViewOffset = 0.0f;
    m_touchBeginPosition.setPoint(0, 0);
    
    m_pContainer = nullptr;
    m_pDragBar = nullptr;
    
    _listener = nullptr;
}


MDragDownView::~MDragDownView()
{
    
}


bool MDragDownView::init()
{
    return init(200, 20, 200, nullptr);
}


bool MDragDownView::init(float fViewWith, float fViewMinHeight, float fViewMaxHeight, CCNode *pContainer)
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    setTouchMode(kCCTouchesOneByOne);
    setTouchEnabled(true);
    
    setViewSize(fViewWith, fViewMinHeight, fViewMaxHeight);
    setContainer(pContainer);
    
    schedule(schedule_selector(MDragDownView::step));
    
    return true;
}


void MDragDownView::setViewSize(float fViewWith, float fViewMinHeight, float fViewMaxHeight)
{
    m_fViewWith = fViewWith;
    m_fViewMinHeight = fViewMinHeight;
    m_fViewMaxHeight = fViewMaxHeight;
    if (m_fViewWith < 0)
    {
        m_fViewWith = 0;
    }
    if (m_fViewMinHeight < 0)
    {
        m_fViewMinHeight = 0;
    }
    if (m_fViewMaxHeight < 0)
    {
        m_fViewMaxHeight = 0;
    }
    if (m_fViewMinHeight > m_fViewMaxHeight)
    {
        m_fViewMinHeight = m_fViewMaxHeight;
    }
    m_viewSize.setSize(m_fViewWith, m_isShow ? m_fViewMaxHeight : m_fViewMinHeight);
    
    CCLayer::setContentSize(m_viewSize);
    
    refresh();
}


float MDragDownView::getViewWidth()
{
    return m_fViewWith;
}


float MDragDownView::getViewMinHeight()
{
    return m_fViewMinHeight;
}


float MDragDownView::getViewMaxHeight()
{
    return m_fViewMaxHeight;
}


void MDragDownView::setContainer(CCNode *pContainer)
{
    if (pContainer)
    {
        if (m_pContainer)
        {
            m_pContainer->removeFromParentAndCleanup(true);
        }
        m_pContainer = pContainer;
        addChild(pContainer, 0);
    }
    else if(m_pContainer == nullptr)
    {
        m_pContainer = CCLayer::create();
        addChild(pContainer, 0);
    }
    
    refresh();
}


CCNode *MDragDownView::getContainer()
{
    return m_pContainer;
}


void MDragDownView::setDragBar(CCNode *pDragBar)
{
    if (pDragBar == nullptr)
    {
        return ;
    }
    
    if (m_pDragBar)
    {
        m_pDragBar->removeFromParentAndCleanup(true);
    }
    m_pDragBar = pDragBar;
    addChild(pDragBar, 1);
    
    refresh();
}


MDragDownView::State MDragDownView::getState()
{
    return m_eState;
}


bool MDragDownView::isShow()
{
    return m_isShow;
}


void MDragDownView::setDefaultDuration(float fDuration)
{
    m_fDefaultDuration = fDuration;
    if (m_fDefaultDuration < 0)
    {
        m_fDefaultDuration = DRAGDOWNVIEW_DURATION;
    }
}


void MDragDownView::refresh()
{
    updateRect();
    adjustContainer();
    adjustDragBar();
}


void MDragDownView::show(float fDuration)
{
    action(true, fDuration);
}


void MDragDownView::hide(float fDuration)
{
    action(false, fDuration);
}


void MDragDownView::action(bool isShow, float fDuration)
{
    m_isShow = isShow;
    
    
    if (fDuration <= 0.0f)
    {
        m_eState = m_isShow ? State::SHOW_BEGIN : State::HIDE_BEGIN;
        if (listener())
        {
            listener()->dragDownViewAction(this);
        }
        dispatchEvent(m_isShow ? EVENT_MDRAGDOWNVIEW_SHOW_BEGIN : EVENT_MDRAGDOWNVIEW_HIDE_BEGIN);
        
        setViewVisibleOffset(m_isShow ? m_fViewMaxHeight : m_fViewMinHeight);
        
        m_eState = m_isShow ? State::SHOW_END : State::HIDE_END;
        if (listener())
        {
            listener()->dragDownViewAction(this);
        }
        dispatchEvent(m_isShow ? EVENT_MDRAGDOWNVIEW_SHOW_END : EVENT_MDRAGDOWNVIEW_HIDE_END);
        
        m_eState = State::NONE;
        return ;
    }
    
    
    m_fDuration = fDuration;
    m_fDurationTimer = 0.0f;
    m_eState = m_isShow ? State::SHOW_BEGIN : State::HIDE_BEGIN;
    if (listener())
    {
        listener()->dragDownViewAction(this);
    }
    dispatchEvent(m_isShow ? EVENT_MDRAGDOWNVIEW_SHOW_BEGIN : EVENT_MDRAGDOWNVIEW_HIDE_BEGIN);
    
    m_eState = m_isShow ? State::SHOW_DOING : State::HIDE_DOING;
    m_viewOldSize = m_viewSize;
    m_fViewOffset = fabs(m_isShow ? (m_fViewMaxHeight - m_viewSize.height) : (m_viewSize.height - m_fViewMinHeight));
}


void MDragDownView::adjustByTouch(CCTouch *pTouch)
{
    if (pTouch == nullptr)
    {
        return ;
    }
    
    auto curPosition = pTouch->getLocation();
    const float fOffset = curPosition.y - m_touchBeginPosition.y;
    setViewVisibleOffset(m_viewOldSize.height - fOffset);
}


void MDragDownView::updateRect()
{
    CCPoint worldPosition = this->convertToWorldSpace(CCPointZero);
    
    float fScaleX = this->getScaleX();
    float fScaleY = this->getScaleY();
    
    for (CCNode *p = m_pParent; p != NULL; p = p->getParent())
    {
        fScaleX *= p->getScaleX();
        fScaleY *= p->getScaleY();
    }
    
    if(fScaleX < 0.f)
    {
        worldPosition.x += m_viewSize.width * fScaleX;
        fScaleX = -fScaleX;
    }
    if(fScaleY < 0.f)
    {
        worldPosition.y += m_viewSize.height * fScaleY;
        fScaleY = -fScaleY;
    }
    
    m_viewRect.setRect(worldPosition.x,
                       worldPosition.y + (m_fViewMaxHeight - m_viewSize.height) * fScaleY,
                       m_viewSize.width * fScaleX,
                       m_viewSize.height * fScaleY);
    
    if (m_pDragBar)
    {
        auto dragSize = m_pDragBar->getContentSize();
        auto dragFixSize = CCSizeMake(dragSize.width * m_pDragBar->getScaleX() * fScaleX,
                                      dragSize.height * m_pDragBar->getScaleY() * fScaleY);
        float fWidth = dragFixSize.width < m_viewRect.size.width ? dragFixSize.width : m_viewRect.size.width;
        float fHeight = dragFixSize.height < m_viewRect.size.height ? dragFixSize.height : m_viewRect.size.height;
        m_dragBarRect.setRect(m_viewRect.origin.x + (m_viewRect.size.width-fWidth)*0.5,
                              m_viewRect.origin.y, fWidth, fHeight);
    }
    /*
    CCLog("[%.0f, %.0f, %.0f, %.0f], [%.0f, %.0f, %.0f, %.0f]",
          m_viewRect.origin.x, m_viewRect.origin.y,
          m_viewRect.size.width, m_viewRect.size.height,
          m_dragBarRect.origin.x, m_dragBarRect.origin.y,
          m_dragBarRect.size.width, m_dragBarRect.size.height);//*/
}


void MDragDownView::setViewVisibleOffset(float fOffset)
{
    m_viewSize.height = fOffset;
    if (m_viewSize.height < 0 || m_viewSize.height < m_fViewMinHeight)
    {
        m_viewSize.height = m_fViewMinHeight;
    }
    if (m_viewSize.height > m_fViewMaxHeight)
    {
        m_viewSize.height = m_fViewMaxHeight;
    }
    
    refresh();
}


void MDragDownView::adjustContainer()
{
    if (m_pContainer)
    {
        m_pContainer->ignoreAnchorPointForPosition(false);
        m_pContainer->setAnchorPoint(CCPointZero);
        m_pContainer->setPosition(CCPointZero);
    }
}


void MDragDownView::adjustDragBar()
{
    if (m_pDragBar)
    {
        m_pDragBar->ignoreAnchorPointForPosition(false);
        m_pDragBar->setAnchorPoint(CCPointZero);
        m_pDragBar->setPosition(convertToNodeSpace(ccp(m_dragBarRect.origin.x, m_dragBarRect.origin.y)));
    }
}


void MDragDownView::step(float dt)
{
    if (m_eState == State::SHOW_DOING)
    {
        m_fDurationTimer += dt;
        
        float fHeight = m_viewOldSize.height + m_fDurationTimer / m_fDuration * m_fViewOffset;
        if (fHeight >= m_fViewMaxHeight)
        {
            fHeight = m_fViewMaxHeight;
            setViewVisibleOffset(fHeight);
            
            m_eState = State::SHOW_END;
            if (listener())
            {
                listener()->dragDownViewAction(this);
            }
            dispatchEvent(EVENT_MDRAGDOWNVIEW_SHOW_END);
            
            m_eState = State::NONE;
        }
        else
        {
            setViewVisibleOffset(fHeight);
            
            if (listener())
            {
                listener()->dragDownViewAction(this);
            }
            dispatchEvent(EVENT_MDRAGDOWNVIEW_SHOW_DOING);
        }
    }
    else if (m_eState == State::HIDE_DOING)
    {
        m_fDurationTimer += dt;
        
        float fHeight = m_viewOldSize.height - m_fDurationTimer / m_fDuration * m_fViewOffset;
        if (fHeight <= m_fViewMinHeight)
        {
            fHeight = m_fViewMinHeight;
            setViewVisibleOffset(fHeight);
            
            m_eState = State::HIDE_END;
            if (listener())
            {
                listener()->dragDownViewAction(this);
            }
            dispatchEvent(EVENT_MDRAGDOWNVIEW_HIDE_END);
            
            m_eState = State::NONE;
        }
        else
        {
            setViewVisibleOffset(fHeight);
            
            if (listener())
            {
                listener()->dragDownViewAction(this);
            }
            dispatchEvent(EVENT_MDRAGDOWNVIEW_HIDE_DOING);
        }
    }
}

void MDragDownView::setContentSize(const CCSize & size)
{
    if (m_pContainer != nullptr)
    {
        m_pContainer->setContentSize(size);
    }
}


const CCSize & MDragDownView::getContentSize() const
{
    if (m_pContainer == nullptr)
    {
        return getContentSize();
    }
	return m_pContainer->getContentSize();
}


void MDragDownView::registerWithTouchDispatcher()
{
    CCLayer::registerWithTouchDispatcher();
}


bool MDragDownView::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isVisible() == false)
    {
        return false;
    }
    
    for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent())
    {
        if (c->isVisible() == false)
        {
            return false;
        }
    }

    updateRect();
    m_touchBeginPosition = pTouch->getLocation();
    
    if (m_viewRect.containsPoint(m_touchBeginPosition) == false)
    {
        return false;
    }
    
    
    if (m_eState != State::NONE)
    {
        return true;
    }
    
    m_isTouchDragBar = m_pDragBar ? m_dragBarRect.containsPoint(m_touchBeginPosition) : false;
    m_viewOldSize = m_viewSize;
    
    return true;
}


void MDragDownView::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_eState == State::NONE && (m_isTouchDragBar || m_pDragBar == nullptr))
    {
        adjustByTouch(pTouch);
        
        m_eState = State::DRAGING;
        if (listener())
        {
            listener()->dragDownViewAction(this);
        }
        m_eState = State::NONE;
    }
}


void MDragDownView::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_eState == State::NONE && (m_isTouchDragBar || m_pDragBar == nullptr))
    {
        auto curPosition = pTouch->getLocation();
        const CCPoint offset = curPosition - m_touchBeginPosition;
        const float fDistance = sqrt(offset.x*offset.x + offset.y*offset.y);
        bool isShow = true;
        if (fDistance < DRAGDOWNVIEW_TOUCH_CLICK_DIS)
        {
            isShow = !m_isShow;
        }
        else
        {
            isShow = offset.y < 0;
        }
        
        action(isShow, m_fDefaultDuration);
    }
}


void MDragDownView::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_eState == State::NONE)
    {
        action(m_isShow, 0.0f);
    }
}


void MDragDownView::visit()
{
	if (!isVisible())
    {
		return;
    }
    
    updateRect();
    
    glEnable(GL_SCISSOR_TEST);
    CCEGLView::sharedOpenGLView()->setScissorInPoints(m_viewRect.origin.x, m_viewRect.origin.y,
                                                      m_viewRect.size.width, m_viewRect.size.height + 5);
    
    CCLayer::visit();
    
    glDisable(GL_SCISSOR_TEST);
}


void MDragDownView::onEnter()
{
    refresh();
    
    CCLayer::onEnter();
}


void MDragDownView::onEnterTransitionDidFinish()
{
    CCLayer::onEnterTransitionDidFinish();
}


void MDragDownView::onExit()
{
    CCLayer::onExit();
}


void MDragDownView::onExitTransitionDidStart()
{
    CCLayer::onExitTransitionDidStart();
}


void MDragDownView::addChild(CCNode *pChild, int nZOrder, int nTag)
{
    if (pChild == nullptr || m_pContainer == nullptr)
    {
        return ;
    }
    
    pChild->ignoreAnchorPointForPosition(false);
    pChild->setAnchorPoint(CCPointZero);
    if (m_pContainer != pChild)
    {
        m_pContainer->addChild(pChild, nZOrder, nTag);
    }
    else
    {
        CCLayer::addChild(pChild, nZOrder, nTag);
    }
}


void MDragDownView::addChild(CCNode *pChild, int nZOrder)
{
    if (pChild == nullptr)
    {
        return ;
    }
    this->addChild(pChild, nZOrder, pChild->getTag());
}


void MDragDownView::addChild(CCNode *pChild)
{
    if (pChild == nullptr)
    {
        return ;
    }
    this->addChild(pChild, pChild->getZOrder(), pChild->getTag());
}

