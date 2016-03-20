//
//  CCRemove.h
//  legend
//
//  Created by Xu Jinyang on 3/28/13.
//
//

#ifndef __legend__CCRemove__
#define __legend__CCRemove__

#include <iostream>
#include "cocos2d.h"
#include "mlib_ccext.h"

NS_CC_BEGIN

class CCRemove : public CCActionInstant
{
public:
    CCRemove(){}
    virtual ~CCRemove(){}
    //super methods
    virtual void update(float time);
    virtual CCObject* copyWithZone(CCZone *pZone);
public:
    MLIB_CREATE_CCOBJECT(CCRemove);
};

NS_CC_END

#endif /* defined(__legend__CCRemove__) */
