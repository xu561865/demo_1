//
//  MTabButton.cpp
//  legend
//
//  Created by Xu Jinyang on 6/28/13.
//
//

#include "MTabButton.h"

#define ZORDER_SELECTED 1
#define ZORDER_UNSELECTED 0

MLIB_REGISTER_CCBLOADER(MTabButton)

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

void MTabButton::setSelected(bool enabled)
{
    if (enabled == true)
    {
        m_eState = CCControlStateHighlighted;
//        setZOrder(ZORDER_SELECTED);
    }
    else
    {
        m_eState = CCControlStateNormal;
//        setZOrder(ZORDER_UNSELECTED);
    }
    
    CCControlButton::setSelected(enabled);
}

void MTabButton::setHighlighted(bool enabled)
{
    return;
}

bool MTabButton::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isSelected())
    {
        return false;
    }
    
    return CCControlButton::ccTouchBegan(pTouch, pEvent);
}

