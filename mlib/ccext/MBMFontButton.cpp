//
//  MBMFontButton.cpp
//  legend
//
//  Created by Minhao Zhang on 6/14/13.
//
//

#include "MBMFontButton.h"
#include "mlib_ccext.h"

#define PROPERTY_BMFONT "bmfont"
#define PROPERTY_BMFONT_SCALE "bmfont_scale"

MBMFontButton::MBMFontButton()
{
    _bmFontScale = 1.0f;
}

MBMFontButton * MBMFontButton::create()
{
    MBMFontButton *pControlButton = new MBMFontButton();
    if (pControlButton && pControlButton->init())
    {
        pControlButton->autorelease();
        return pControlButton;
    }
    CC_SAFE_DELETE(pControlButton);
    return NULL;
}

NS_CC_EXT_BEGIN

MLIB_REGISTER_CCBLOADER(MBMFontButton)

void MBMFontButtonLoader::onHandlePropTypeString(cocos2d::CCNode *pNode, cocos2d::CCNode *pParent, const char *pPropertyName, const char *pString, cocos2d::extension::CCBReader *pCCBReader)
{
    M_DEBUG("string p " << pPropertyName);

    if(strcmp(pPropertyName, PROPERTY_BMFONT) == 0)
    {
        ((CCControlButton *)pNode)->setTitleBMFontForState(pString, CCControlStateNormal);
        auto label = dynamic_cast<CCLabelBMFont *>(((CCControlButton *)pNode)->getTitleLabelForState(CCControlStateNormal));
        label->setScale(((MBMFontButton *)pNode)->bmFontScale());
    }
    else
    {
        CCControlButtonLoader::onHandlePropTypeString(pNode, pParent, pPropertyName, pString, pCCBReader);
    }
}

void MBMFontButtonLoader::onHandlePropTypeFontTTF(cocos2d::CCNode *pNode, cocos2d::CCNode *pParent, const char *pPropertyName, const char *pFontTTF, cocos2d::extension::CCBReader *pCCBReader)
{
    CCControlButtonLoader::onHandlePropTypeFontTTF(pNode, pParent, pPropertyName, pFontTTF, pCCBReader);
}


void MBMFontButtonLoader::onHandlePropTypeFloat(cocos2d::CCNode *pNode, cocos2d::CCNode *pParent, const char *pPropertyName, float pFloat, cocos2d::extension::CCBReader *pCCBReader)
{
    M_DEBUG("float p " << pPropertyName);
    if(strcmp(pPropertyName, PROPERTY_BMFONT_SCALE) == 0)
    {
        auto label = dynamic_cast<CCLabelBMFont *>(((CCControlButton *)pNode)->getTitleLabelForState(CCControlStateNormal));
        ((MBMFontButton *)pNode)->bmFontScale() = pFloat;
        label->setScale(((MBMFontButton *)pNode)->bmFontScale());
    }
    else
    {
        CCControlButtonLoader::onHandlePropTypeFloat(pNode, pParent, pPropertyName, pFloat, pCCBReader);
    }
}

NS_CC_EXT_END

