//
//  CCProgressTimerLoader.h
//  legend
//
//  Created by mac on 13-4-6.
//
//

#ifndef __legend__CCProgressTimerLoader__
#define __legend__CCProgressTimerLoader__

#include <iostream>
#include "CCNodeLoader.h"
#include "misc_nodes/CCProgressTimer.h"

NS_CC_EXT_BEGIN

class CCProgressTimerLoader : public CCNodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCProgressTimerLoader, loader);
    
protected:
    CCProgressTimer * createCCNode(cocos2d::CCNode * pParent, cocos2d::extension::CCBReader * pCCBReader);
};

NS_CC_EXT_END

#endif /* defined(__legend__CCProgressTimerLoader__) */
