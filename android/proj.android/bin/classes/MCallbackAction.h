//
//  MCallbackAction.h
//  legend
//
//  Created by Minhao Zhang on 5/10/13.
//
//

#ifndef __legend__MCallbackAction__
#define __legend__MCallbackAction__


#include <iostream>
#include "cocos2d.h"
#include "mlib_ccext.h"
#include <functional>

MLIB_NS_BEGIN

class MCallbackAction : public cocos2d::CCActionInstant
{
public:
    MCallbackAction() {}
    virtual ~MCallbackAction() {}
    //super methods
    virtual void update(float time);
    virtual cocos2d::CCObject* copyWithZone(cocos2d::CCZone *pZone);
public:
    static MCallbackAction * create(std::function<void(void)> callback);
    
private:
    std::function<void(void)> _callback;
};

MLIB_NS_END

#endif /* defined(__legend__MCallbackAction__) */
