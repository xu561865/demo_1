//
//  CCProgressTimerLoader.cpp
//  legend
//
//  Created by mac on 13-4-6.
//
//

#include "CCProgressTimerLoader.h"
#include "mlib_ccext.h"

NS_CC_EXT_BEGIN

MLIB_REGISTER_CCBLOADER(CCProgressTimer);

CCProgressTimer * CCProgressTimerLoader::createCCNode(cocos2d::CCNode * pParent, cocos2d::extension::CCBReader * pCCBReader)
{
    return CCProgressTimer::create(nullptr);
}

NS_CC_EXT_END
