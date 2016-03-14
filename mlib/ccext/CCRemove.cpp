//
//  CCRemove.cpp
//  legend
//
//  Created by Xu Jinyang on 3/28/13.
//
//

#include "CCRemove.h"

NS_CC_BEGIN

void CCRemove::update(float time) {
    CC_UNUSED_PARAM(time);
    m_pTarget->removeFromParent();
}

CCObject* CCRemove::copyWithZone(CCZone *pZone) {
    
    CCZone *pNewZone = NULL;
    CCRemove *pRet = NULL;
    if (pZone && pZone->m_pCopyObject) {
        pRet = (CCRemove*) (pZone->m_pCopyObject);
    } else {
        pRet = new CCRemove();
        pZone = pNewZone = new CCZone(pRet);
    }
    
    CCActionInstant::copyWithZone(pZone);
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

NS_CC_END
