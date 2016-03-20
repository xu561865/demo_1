//
//  MBMFontButton.h
//  legend
//
//  Created by Minhao Zhang on 6/14/13.
//
//

#ifndef __legend__MBMFontButton__
#define __legend__MBMFontButton__

#include "CCControlButton.h"
#include "CCControlButtonLoader.h"

#include "mlib_helpers.h"

class MBMFontButton :
public cocos2d::extension::CCControlButton
{
public:
    MBMFontButton();
    static MBMFontButton * create();
    
public:
    MLIB_DECLARE_PROPERTY(float, bmFontScale);
};

NS_CC_EXT_BEGIN

class MBMFontButtonLoader :
public cocos2d::extension::CCControlButtonLoader
{
public:
    virtual ~MBMFontButtonLoader() {};
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(MBMFontButtonLoader, loader);

    virtual void onHandlePropTypeString(CCNode * pNode, CCNode * pParent, const char * pPropertyName, const char * pString, CCBReader * pCCBReader);
    virtual void onHandlePropTypeFontTTF(CCNode * pNode, CCNode * pParent, const char * pPropertyName, const char * pFontTTF, CCBReader * pCCBReader);
    virtual void onHandlePropTypeFloat(CCNode * pNode, CCNode * pParent, const char * pPropertyName, float pFloat, CCBReader * pCCBReader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(MBMFontButton);
};

NS_CC_EXT_END

#endif /* defined(__legend__MBMFontButton__) */
