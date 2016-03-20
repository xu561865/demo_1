//
//  MSprite.h
//  legend
//
//  Created by Xu Jinyang on 3/26/13.
//
//

#ifndef __legend__MSprite__
#define __legend__MSprite__

#include <iostream>
#include "cocos2d.h"
#include "mlib_helpers.h"

MLIB_NS_BEGIN

class MSprite : public cocos2d::CCSprite
{
public:
    MSprite();
    static MSprite* create();
    
    static MSprite* create(const char *pszFileName);
    static MSprite* create(const char *pszFileName, const cocos2d::CCRect& rect);
    static MSprite* createWithTexture(cocos2d::CCTexture2D *pTexture);
    static MSprite* createWithTexture(cocos2d::CCTexture2D *pTexture, const cocos2d::CCRect& rect);
    static MSprite* createWithSpriteFrame(cocos2d::CCSpriteFrame *pSpriteFrame);
    static MSprite* createWithSpriteFrameName(const char *pszSpriteFrameName);
    void reloadSrc();
    
    MLIB_DECLARE_PROPERTY_SETTER_TO_IMPL(bool, isGray);
    MLIB_DECLARE_PROPERTY_SETTER_TO_IMPL(std::string, srcPath);
protected:
    void initScale();
private:
    bool _isGrayChange;
    bool _isSrcChange;
};

MLIB_NS_END

#endif /* defined(__legend__MSprite__) */
