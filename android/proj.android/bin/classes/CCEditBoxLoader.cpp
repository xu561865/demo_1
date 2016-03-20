#include "CCEditBoxLoader.h"
#include "mlib_ccext.h"
#include "cocos-ext.h"

NS_CC_EXT_BEGIN;

MLIB_REGISTER_CCBLOADER(CCEditBox);

#define PROPERTY_ZOOMONTOUCHDOWN "zoomOnTouchDown"
#define PROPERTY_TITLE_NORMAL "title|1"
#define PROPERTY_TITLE_HIGHLIGHTED "title|2"
#define PROPERTY_TITLE_DISABLED "title|3"
#define PROPERTY_TITLECOLOR_NORMAL "titleColor|1"
#define PROPERTY_TITLECOLOR_HIGHLIGHTED "titleColor|2"
#define PROPERTY_TITLECOLOR_DISABLED "titleColor|3"
#define PROPERTY_TITLETTF_NORMAL "titleTTF|1"
#define PROPERTY_TITLETTF_HIGHLIGHTED "titleTTF|2"
#define PROPERTY_TITLETTF_DISABLED "titleTTF|3"
#define PROPERTY_TITLETTFSIZE_NORMAL "titleTTFSize|1"
#define PROPERTY_TITLETTFSIZE_HIGHLIGHTED "titleTTFSize|2"
#define PROPERTY_TITLETTFSIZE_DISABLED "titleTTFSize|3"
#define PROPERTY_LABELANCHORPOINT "labelAnchorPoint"
#define PROPERTY_PREFEREDSIZE "preferedSize" // TODO Should be "preferredSize". This is a typo in cocos2d-iphone, cocos2d-x and CocosBuilder!
#define PROPERTY_BACKGROUNDSPRITEFRAME_NORMAL "backgroundSpriteFrame|1"
#define PROPERTY_BACKGROUNDSPRITEFRAME_HIGHLIGHTED "backgroundSpriteFrame|2"
#define PROPERTY_BACKGROUNDSPRITEFRAME_DISABLED "backgroundSpriteFrame|3"

CCEditBox * CCEditBoxLoader::createCCNode(cocos2d::CCNode * pParent, cocos2d::extension::CCBReader * pCCBReader)
{
//    int curByte = pCCBReader->getCurrentByte();
    
//    pCCBReader->setCurrentByte(curByte+1); // hack for numSequence(1 byte)
    CCControlButtonLoader *btnLoader = CCControlButtonLoader::loader();
    CCControlButton *btn = (CCControlButton *)pCCBReader->preLoadNode(btnLoader);
    
    CCScale9Sprite *normalSprite = btn->getBackgroundSpriteForState(CCControlStateNormal);
    CCScale9Sprite *highlightedSprite = btn->getBackgroundSpriteForState(CCControlStateHighlighted);
    CCScale9Sprite *disabledSprite = btn->getBackgroundSpriteForState(CCControlStateDisabled);
    
    normalSprite->retain();
    highlightedSprite->retain();
    disabledSprite->retain();
    
    normalSprite->removeFromParent();
    highlightedSprite->removeFromParent();
    disabledSprite->removeFromParent();
    
    CCEditBox *editBox = CCEditBox::create(btn->getPreferredSize(), btn->getBackgroundSpriteForState(CCControlStateNormal), btn->getBackgroundSpriteForState(CCControlStateHighlighted), btn->getBackgroundSpriteForState(CCControlStateDisabled));
    editBox->setZoomOnTouchDown(false);
    editBox->setFont(btn->getTitleTTFForState(CCControlStateNormal), btn->getTitleTTFSizeForState(CCControlStateNormal));
    editBox->setFontColor(btn->getTitleColorForState(CCControlStateNormal));
    editBox->setAnchorPoint(btn->getAnchorPoint());
    
//    editBox->setPlaceholderFontColor(btn->getTitleColorForState(CCControlStateHighlighted));
    
    normalSprite->release();
    highlightedSprite->release();
    disabledSprite->release();
//    pCCBReader->setCurrentByte(curByte);
    
    return editBox;
}

void CCEditBoxLoader::onHandlePropTypeCheck(CCNode * pNode, CCNode * pParent, const char * pPropertyName, bool pCheck, CCBReader * pCCBReader) {
//    if(strcmp(pPropertyName, PROPERTY_ZOOMONTOUCHDOWN) == 0) {
//        ((CCControlButton *)pNode)->setZoomOnTouchDown(pCheck);
//    } else {
//        CCControlLoader::onHandlePropTypeCheck(pNode, pParent, pPropertyName, pCheck, pCCBReader);
//    }
}

void CCEditBoxLoader::onHandlePropTypeString(CCNode * pNode, CCNode * pParent, const char * pPropertyName, const char * pString, CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_TITLE_NORMAL) == 0) {
        ((CCEditBox *)pNode)->setPlaceHolder(pString);
    }
//    } else if(strcmp(pPropertyName, PROPERTY_TITLE_HIGHLIGHTED) == 0) {
//        ((CCControlButton *)pNode)->setTitleForState(CCString::create(pString), CCControlStateHighlighted);
//    } else if(strcmp(pPropertyName, PROPERTY_TITLE_DISABLED) == 0) {
//        ((CCControlButton *)pNode)->setTitleForState(CCString::create(pString), CCControlStateDisabled);
//    } else {
//        CCControlLoader::onHandlePropTypeString(pNode, pParent, pPropertyName, pString, pCCBReader);
//    }
}

void CCEditBoxLoader::onHandlePropTypeFontTTF(CCNode * pNode, CCNode * pParent, const char * pPropertyName, const char * pFontTTF, CCBReader * pCCBReader) {
//    if(strcmp(pPropertyName, PROPERTY_TITLETTF_NORMAL) == 0) {
//        ((CCControlButton *)pNode)->setTitleTTFForState(pFontTTF, CCControlStateNormal);
//    } else if(strcmp(pPropertyName, PROPERTY_TITLETTF_HIGHLIGHTED) == 0) {
//        ((CCControlButton *)pNode)->setTitleTTFForState(pFontTTF, CCControlStateHighlighted);
//    } else if(strcmp(pPropertyName, PROPERTY_TITLETTF_DISABLED) == 0) {
//        ((CCControlButton *)pNode)->setTitleTTFForState(pFontTTF, CCControlStateDisabled);
//    } else {
//        CCControlLoader::onHandlePropTypeFontTTF(pNode, pParent, pPropertyName, pFontTTF, pCCBReader);
//    }
}

void CCEditBoxLoader::onHandlePropTypeFloatScale(CCNode * pNode, CCNode * pParent, const char * pPropertyName, float pFloatScale, CCBReader * pCCBReader) {
//    if(strcmp(pPropertyName, PROPERTY_TITLETTFSIZE_NORMAL) == 0) {
//        ((CCControlButton *)pNode)->setTitleTTFSizeForState(pFloatScale, CCControlStateNormal);
//    } else if(strcmp(pPropertyName, PROPERTY_TITLETTFSIZE_HIGHLIGHTED) == 0) {
//        ((CCControlButton *)pNode)->setTitleTTFSizeForState(pFloatScale, CCControlStateHighlighted);
//    } else if(strcmp(pPropertyName, PROPERTY_TITLETTFSIZE_DISABLED) == 0) {
//        ((CCControlButton *)pNode)->setTitleTTFSizeForState(pFloatScale, CCControlStateDisabled);
//    } else {
//        CCControlLoader::onHandlePropTypeFloatScale(pNode, pParent, pPropertyName, pFloatScale, pCCBReader);
//    }
}

void CCEditBoxLoader::onHandlePropTypePoint(CCNode * pNode, CCNode * pParent, const char * pPropertyName, CCPoint pPoint, CCBReader * pCCBReader) {
//    if(strcmp(pPropertyName, PROPERTY_LABELANCHORPOINT) == 0) {
//        ((CCControlButton *)pNode)->setLabelAnchorPoint(pPoint);
//    } else {
//        CCControlLoader::onHandlePropTypePoint(pNode, pParent, pPropertyName, pPoint, pCCBReader);
//    }
}

void CCEditBoxLoader::onHandlePropTypeSize(CCNode * pNode, CCNode * pParent, const char * pPropertyName, CCSize pSize, CCBReader * pCCBReader) {
//    if(strcmp(pPropertyName, PROPERTY_PREFEREDSIZE) == 0) {
//        //((CCControlButton *)pNode)->setPreferredSize(pSize);
//        ((CCControlButton *)pNode)->setPreferredSize(CCSizeMake(pSize.width/CC_CONTENT_SCALE_FACTOR(), pSize.height/CC_CONTENT_SCALE_FACTOR()));
//    } else {
//        CCControlLoader::onHandlePropTypeSize(pNode, pParent, pPropertyName, pSize, pCCBReader);
//    }
}

void CCEditBoxLoader::onHandlePropTypeSpriteFrame(CCNode * pNode, CCNode * pParent, const char * pPropertyName, CCSpriteFrame * pCCSpriteFrame, CCBReader * pCCBReader) {
//    if(strcmp(pPropertyName, PROPERTY_BACKGROUNDSPRITEFRAME_NORMAL) == 0) {
//        if(pCCSpriteFrame != NULL) {
//            ((CCControlButton *)pNode)->setBackgroundSpriteFrameForState(pCCSpriteFrame, CCControlStateNormal);
//        }
//    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDSPRITEFRAME_HIGHLIGHTED) == 0) {
//        if(pCCSpriteFrame != NULL) {
//            ((CCControlButton *)pNode)->setBackgroundSpriteFrameForState(pCCSpriteFrame, CCControlStateHighlighted);
//        }
//    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDSPRITEFRAME_DISABLED) == 0) {
//        if(pCCSpriteFrame != NULL) {
//            ((CCControlButton *)pNode)->setBackgroundSpriteFrameForState(pCCSpriteFrame, CCControlStateDisabled);
//        }
//    } else {
//        CCControlLoader::onHandlePropTypeSpriteFrame(pNode, pParent, pPropertyName, pCCSpriteFrame, pCCBReader);
//    }
}

void CCEditBoxLoader::onHandlePropTypeColor3(CCNode * pNode, CCNode * pParent, const char * pPropertyName, ccColor3B pCCColor3B, CCBReader * pCCBReader) {
//    if(strcmp(pPropertyName, PROPERTY_TITLECOLOR_NORMAL) == 0) {
//        ((CCControlButton *)pNode)->setTitleColorForState(pCCColor3B, CCControlStateNormal);
//    } else if(strcmp(pPropertyName, PROPERTY_TITLECOLOR_HIGHLIGHTED) == 0) {
//        ((CCControlButton *)pNode)->setTitleColorForState(pCCColor3B, CCControlStateHighlighted);
//    } else if(strcmp(pPropertyName, PROPERTY_TITLECOLOR_DISABLED) == 0) {
//        ((CCControlButton *)pNode)->setTitleColorForState(pCCColor3B, CCControlStateDisabled);
//    } else {
//        CCControlLoader::onHandlePropTypeColor3(pNode, pParent, pPropertyName, pCCColor3B, pCCBReader);
//    }
}

NS_CC_EXT_END;