//
//  MSprite.cpp
//  legend
//
//  Created by Xu Jinyang on 3/26/13.
//
//

#include "MSprite.h"
#include "MUtils.h"

USING_NS_CC;

MLIB_NS_BEGIN

MSprite::MSprite()
{
    _isGray = false;
    _isGrayChange = false;
    _isSrcChange = false;
}

MSprite* MSprite::createWithTexture(CCTexture2D *pTexture)
{
    MSprite *pobSprite = new MSprite();
    if (pobSprite && pobSprite->initWithTexture(pTexture))
    {
        pobSprite->autorelease();
        pobSprite->initScale();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

MSprite* MSprite::createWithTexture(CCTexture2D *pTexture, const CCRect& rect)
{
    MSprite *pobSprite = new MSprite();
    if (pobSprite && pobSprite->initWithTexture(pTexture, rect))
    {
        pobSprite->autorelease();
        pobSprite->initScale();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

MSprite* MSprite::create(const char *pszFileName)
{
    MSprite *pobSprite = new MSprite();
    if (pobSprite && pobSprite->initWithFile(pszFileName))
    {
        pobSprite->autorelease();
        pobSprite->initScale();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

MSprite* MSprite::create(const char *pszFileName, const CCRect& rect)
{
    MSprite *pobSprite = new MSprite();
    if (pobSprite && pobSprite->initWithFile(pszFileName, rect))
    {
        pobSprite->autorelease();
        pobSprite->initScale();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

MSprite* MSprite::createWithSpriteFrame(CCSpriteFrame *pSpriteFrame)
{
    MSprite *pobSprite = new MSprite();
    if (pSpriteFrame && pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame))
    {
        pobSprite->autorelease();
        pobSprite->initScale();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

MSprite* MSprite::createWithSpriteFrameName(const char *pszSpriteFrameName)
{
    CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszSpriteFrameName);
    
#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", pszSpriteFrameName);
    CCAssert(pFrame != NULL, msg);
#endif
    
    return createWithSpriteFrame(pFrame);
}

MSprite* MSprite::create()
{
    MSprite *pSprite = new MSprite();
    if (pSprite && pSprite->init())
    {
        pSprite->autorelease();
        pSprite->initScale();
        return pSprite;
    }
    CC_SAFE_DELETE(pSprite);
    return NULL;
}

void MSprite::initScale()
{
    setScale(CC_CONTENT_SCALE_FACTOR());
}

void MSprite::isGray(bool isGray)
{
    if (_isGray == isGray)
    {
        _isGrayChange = false;
        return;
    }
    
    _isGray = isGray;
    _isGrayChange = true;
}

void MSprite::srcPath(std::string srcPath)
{
    if (_srcPath == srcPath)
    {
        _isSrcChange = false;
        return;
    }
    _srcPath = srcPath;
    _isSrcChange = true;
}

void MSprite::reloadSrc()
{
    if ("" == _srcPath)
    {
        return;
    }
    
    if (!_isSrcChange && !_isGrayChange)
    {
        return;
    }
    
    auto image = get_ccimage(_srcPath);
    CCTexture2D *texture = nullptr;
    if (isGray())
    {
        texture = mlib::create_gray_texture(image);
    }
    else
    {
        texture = new CCTexture2D();
        texture->autorelease();
        
        if(texture && texture->initWithImage(image))
        {
        }
    }
    
    if (texture != nullptr)
    {
        CCRect rect = CCRectZero;
        rect.size = texture->getContentSize();
        auto frame = CCSpriteFrame::createWithTexture(texture, rect);
        this->setDisplayFrame(frame);
    }
    
    _isGrayChange = false;
    _isSrcChange = false;
}
MLIB_NS_END