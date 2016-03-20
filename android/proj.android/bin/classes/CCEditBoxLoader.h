#ifndef _CCB_EDITBOXLOADER_H_
#define _CCB_EDITBOXLOADER_H_

#include "CCControlButtonLoader.h"
#include "GUI/CCEditBox/CCEditBox.h"

NS_CC_EXT_BEGIN

/* Forward declaration. */
class CCBReader;

class CCEditBoxLoader : public CCControlButtonLoader {
public:
    virtual ~CCEditBoxLoader() {};
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCEditBoxLoader, loader);
    
protected:
    virtual CCEditBox * createCCNode(cocos2d::CCNode * pParent, cocos2d::extension::CCBReader * pCCBReader);
    
    virtual void onHandlePropTypeCheck(CCNode * pNode, CCNode * pParent, const char * pPropertyName, bool pCheck, CCBReader * pCCBReader);
    virtual void onHandlePropTypeString(CCNode * pNode, CCNode * pParent, const char * pPropertyName, const char * pString, CCBReader * pCCBReader);
    virtual void onHandlePropTypeFontTTF(CCNode * pNode, CCNode * pParent, const char * pPropertyName, const char * pFontTTF, CCBReader * pCCBReader);
    virtual void onHandlePropTypeFloatScale(CCNode * pNode, CCNode * pParent, const char * pPropertyName, float pFloatScale, CCBReader * pCCBReader);
    virtual void onHandlePropTypePoint(CCNode * pNode, CCNode * pParent, const char * pPropertyName, CCPoint pPoint, CCBReader * pCCBReader);
    virtual void onHandlePropTypeSize(CCNode * pNode, CCNode * pParent, const char * pPropertyName, CCSize pSize, CCBReader * pCCBReader);
    virtual void onHandlePropTypeSpriteFrame(CCNode * pNode, CCNode * pParent, const char * pPropertyName, CCSpriteFrame * pCCSpriteFrame, CCBReader * pCCBReader);
    virtual void onHandlePropTypeColor3(CCNode * pNode, CCNode * pParent, const char * pPropertyName, ccColor3B pCCColor3B, CCBReader * pCCBReader);
};

NS_CC_EXT_END

#endif
