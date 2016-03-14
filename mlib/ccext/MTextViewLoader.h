//
//  MTextViewLoader.h
//  legend
//
//  Created by Xu Jinyang on 11/28/13.
//
//

#ifndef __legend__MTextViewLoader__
#define __legend__MTextViewLoader__

#include <iostream>

#include "CCControlButtonLoader.h"
#include "MTextView.h"

NS_CC_EXT_BEGIN

class MTextViewLoader : public CCControlButtonLoader
{
public:
    virtual ~MTextViewLoader() {}
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(MTextViewLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(mlib::MTextView);
    
    virtual void onHandlePropTypeCheck(CCNode * pNode, CCNode * pParent, const char * pPropertyName, bool pCheck, CCBReader * pCCBReader);
    virtual void onHandlePropTypeString(CCNode * pNode, CCNode * pParent, const char * pPropertyName, const char * pString, CCBReader * pCCBReader);
    virtual void onHandlePropTypeFontTTF(CCNode * pNode, CCNode * pParent, const char * pPropertyName, const char * pFontTTF, CCBReader * pCCBReader);
    virtual void onHandlePropTypeFloatScale(CCNode * pNode, CCNode * pParent, const char * pPropertyName, float pFloatScale, CCBReader * pCCBReader);
    virtual void onHandlePropTypeSize(CCNode * pNode, CCNode * pParent, const char * pPropertyName, CCSize pSize, CCBReader * pCCBReader);
    virtual void onHandlePropTypeSpriteFrame(CCNode * pNode, CCNode * pParent, const char * pPropertyName, CCSpriteFrame * pCCSpriteFrame, CCBReader * pCCBReader);
    virtual void onHandlePropTypeColor3(CCNode * pNode, CCNode * pParent, const char * pPropertyName, ccColor3B pCCColor3B, CCBReader * pCCBReader);
};

NS_CC_EXT_END

#endif /* defined(__legend__MTextViewLoader__) */
