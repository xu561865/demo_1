//
//  MControl.cpp
//  legend
//
//  Created by Xu Jinyang on 4/12/13.
//
//

#include "MControl.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace mlib;

MLIB_REGISTER_CCBLOADER(MControl);

MControl::MControl()
{
    MLIB_PROPERTY_INIT(cocos2d::CCNode *, tooltip);
    MLIB_PROPERTY_INIT(mlib::MControlListener *, listener);
    
    _showTooltipOnTapped = false;
    _isDragable = false;
    _isMoved = false;
    _popupCenter = true;
    _popupBottom = false;
}

MControl::~MControl()
{
    if (_tooltip != nullptr && _tooltip->getParent() != nullptr)
    {
        MScreenManager::closePopup(_tooltip);
        _tooltip->removeFromParent();
    }
    MLIB_PROPERTY_RELEASE(cocos2d::CCNode *, tooltip);
}

bool MControl::init()
{
    if (!CCControl::init())
    {
        return false;
    }
    
    setTouchEnabled(true);
    return true;
}

void MControl::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, getTouchPriority(), true);
}

void MControl::onEnter()
{
    CCControl::onEnter();
}

void MControl::onExit()
{
    dispatchEvent(EVENT_MCONTROL_EXIT);
    CCControl::onExit();
}

bool MControl::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!isTouchInside(pTouch) || !isEnabled() || !isVisible())
    {
        return false;
    }
    
    for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent())
    {
        if (!c->isVisible())
        {
            return false;
        }
    }
    
    dispatchEvent(EVENT_MCONTROL_TOUCH_BEGAN);
    
    if (isDragable())
    {
        _oldZOrder = getZOrder();
        setZOrder(_oldZOrder + 1);
        _isMoved = false;
    }
    return true;
}

void MControl::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isDragable())
    {
        if (!_isMoved)
        {
            CCPoint lastTouchPos = pTouch->getStartLocationInView();
            CCPoint curTouchPos = pTouch->getLocationInView();
            if (ccpDistance(curTouchPos, lastTouchPos) > 5)
            {
                _isMoved = true;
                
                CCPoint pt = getPosition();
                pt.x += curTouchPos.x - lastTouchPos.x;
                pt.y -= curTouchPos.y - lastTouchPos.y;
                setPosition(pt);
            }
            else
            {
                return;
            }
        }
        else
        {
            CCPoint lastTouchPos = pTouch->getPreviousLocationInView();
            CCPoint curTouchPos = pTouch->getLocationInView();
            CCPoint pt = getPosition();
            pt.x += curTouchPos.x - lastTouchPos.x;
            pt.y -= curTouchPos.y - lastTouchPos.y;
            setPosition(pt);
        }
        
        dispatchEvent(EVENT_MCONTROL_MOVED);
    }
    return;
}

void MControl::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    dispatchEvent(EVENT_MCONTROL_TOUCH_ENDED);
 
    if (isDragable())
    {
        setZOrder(_oldZOrder);
        if (_isMoved)
        {
            dispatchEvent(EVENT_MCONTROL_DROPPED);
            return;
        }
    }
    
    if (isTouchInside(pTouch))
    {
        dispatchEvent(EVENT_MCONTROL_TAPPED);
    }
}

void MControl::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isDragable() && _isMoved)
    {
        dispatchEvent(EVENT_MCONTROL_DROPPED);
        setZOrder(0);
    }
    dispatchEvent(EVENT_MCONTROL_TOUCH_ENDED);
}

void MControl::tooltip(cocos2d::CCNode *atooltip)
{
    // 避免createWithCCB之后可能产生的内存泄漏
    if (atooltip != nullptr)
    {
        atooltip->stopAllActions();
    }
    
    MLIB_IMPL_RETAIN_PROPERTY_SETTER(tooltip);
    
    if (_tooltip != nullptr)
    {
        removeEventListenerFor(this);
        
        if (showTooltipOnTapped())
        {
            addEventListener(EVENT_MCONTROL_TAPPED, MLIB_EVENT_CALLBACK(&MControl::onTapped), this);
        }
        else
        {
            addEventListener(EVENT_MCONTROL_TOUCH_BEGAN, MLIB_EVENT_CALLBACK(&MControl::onTouchBegan), this);
            addEventListener(EVENT_MCONTROL_TOUCH_ENDED, MLIB_EVENT_CALLBACK(&MControl::onTouchEnded), this);
        }
    }
}

void MControl::onTouchBegan(mlib::MEvent *evt)
{
    if( _popupCenter )
    {
        MScreenManager::popup(_tooltip, false);
//        M_DEBUG("poped up  " << (unsigned)_tooltip->getParent());
    }
    else
    {
        MToolTipPosition eMTipPos = MToolTipPosition_Default;
        CCSize contentSize = CCSizeZero;
        float fScaleX = 1;
        if (listener() != nullptr)
        {
            std::tuple<MToolTipPosition, cocos2d::CCSize, float> ret = listener()->onPrepareShowToolTip(this);
            
            eMTipPos = std::get<0>(ret);
            contentSize = std::get<1>(ret);
//            fScaleX = std::get<2>(ret);       //在下面用变换矩阵判断，不用从外面传入
        }
        
        if (eMTipPos == MToolTipPosition_Default)
        {
            CCSize drSize = mlib::get_design_resolution_size();
            
            CCPoint ptControl;
            ptControl = this->convertToWorldSpace(CCPointZero);
            ptControl.y=ptControl.y+this->getContentSize().height+12;
            
            CCPoint pt = CCPointMake(drSize.width / 2, drSize.height / 2);
            pt.y=ptControl.y;
            //pt.y+=200;
            
            MScreenManager::popup(_tooltip,pt,false);
        }
        else
        {
//            CCSize drSize = mlib::get_design_resolution_size();
            CCPoint position = getParent() ? getParent()->convertToWorldSpace(getPosition()) : this->convertToWorldSpace(CCPointZero);
            CCRect rect = this->boundingBox();
            CCRect actRect = CCRectApplyAffineTransform(rect, this->getParent()->nodeToWorldTransform());
            
            if (actRect.origin.x - rect.origin.x == position.x - actRect.size.width)
                fScaleX = -1.0f;

            if (eMTipPos == MToolTipPosition_UpAlignLeft
                || eMTipPos == MToolTipPosition_UpAlignRight
                || eMTipPos == MToolTipPosition_UpAlignCenter)
            {
                if (_popupBottom)
                    position.y += -12;
                else
                    position.y += (actRect.size.height + 12);
            }
            
            if (eMTipPos == MToolTipPosition_UpAlignCenter)
            {
                if ( fScaleX > 0 )
                {
                    position.x += (actRect.size.width * 0.5);
                }
                else
                {
                    position.x += (-actRect.size.width * 0.5);
                }
            }
            else if (eMTipPos == MToolTipPosition_UpAlignLeft)
            {
                if ( fScaleX > 0 )
                {
                    position.x += (contentSize.width * 0.5f);
                }
                else
                {
                    position.x += (-actRect.size.width + contentSize.width * 0.5f);
                }
            }
            else if (eMTipPos == MToolTipPosition_UpAlignRight)
            {
                if ( fScaleX > 0 )
                {
                    position.x += (actRect.size.width - contentSize.width * 0.5f);
                }
                else
                {
                    position.x += (-contentSize.width * 0.5f);
                }
            }
            
            MScreenManager::popup(_tooltip, position, false);
        }
    }

}

void MControl::onTouchEnded(mlib::MEvent *evt)
{
//    M_DEBUG((unsigned)_tooltip->getParent());

    MScreenManager::closePopup(_tooltip);
    _tooltip->removeFromParent();   // 避免低帧率时连续点击崩溃
    _tooltip->setParent(nullptr);
}

void MControl::onTapped(mlib::MEvent *evt)
{
    MScreenManager::popup(_tooltip, false, true, 2);
}
