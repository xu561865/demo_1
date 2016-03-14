//
//  MCallbackAction.cpp
//  legend
//
//  Created by Minhao Zhang on 5/10/13.
//
//

#include "MCallbackAction.h"

USING_NS_CC;

MLIB_NS_BEGIN

void MCallbackAction::update(float time)
{
    _callback();
}

CCObject* MCallbackAction::copyWithZone(CCZone *pZone) {
    
    CCZone *pNewZone = NULL;
    MCallbackAction *pRet = NULL;
    if (pZone && pZone->m_pCopyObject) {
        pRet = (MCallbackAction*) (pZone->m_pCopyObject);
    } else {
        pRet = new MCallbackAction();
        pZone = pNewZone = new CCZone(pRet);
    }
    
    CCActionInstant::copyWithZone(pZone);
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

MCallbackAction * MCallbackAction::create(std::function<void(void)> callback)
{
    auto ret = new MCallbackAction();
    if (ret)
    {
        ret->_callback = callback;
        ret->autorelease();
    }
    
    return ret;
}

MLIB_NS_END
