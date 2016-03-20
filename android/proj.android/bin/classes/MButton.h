//
//  MButton.h
//  legend
//
//  Created by Xu Jinyang on 6/26/13.
//
//

#ifndef __legend__MButton__
#define __legend__MButton__

#include <iostream>
#include "CCControlButton.h"
#include "CCControlButtonLoader.h"

#include "mlib_helpers.h"

class MButton : public cocos2d::extension::CCControlButton
{
public:
    MButton();
    
    CREATE_FUNC(MButton);

    MLIB_DECLARE_PROPERTY(cocos2d::ccColor3B, strokeColor);
    MLIB_DECLARE_PROPERTY(float, strokeSize);
    
    MLIB_DECLARE_PROPERTY(cocos2d::CCSize, shadowOffset);
    MLIB_DECLARE_PROPERTY(float, shadowOpacity);
    MLIB_DECLARE_PROPERTY(float, shadowBlur);
};

NS_CC_EXT_BEGIN

class MButtonLoader : public cocos2d::extension::CCControlButtonLoader
{
public:
    virtual ~MButtonLoader() {};
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(MButtonLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(MButton);

};

NS_CC_EXT_END

#endif /* defined(__legend__MButton__) */
