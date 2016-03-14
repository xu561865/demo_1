//
//  MRemoteSprite.h
//  legend
//
//  Created by Xu Jinyang on 3/28/13.
//
//

#ifndef __legend__MRemoteSprite__
#define __legend__MRemoteSprite__

#include <iostream>
#include "cocos2d.h"
#include "mlib_ccext.h"
#include "MFileDownloader.h"

MLIB_NS_BEGIN
#define EVENT_CARD_SKETCH "event_card_sketch"
#define EVENT_CARD_DOWNLOAD_OK "event_card_download_ok"
class MRemoteSprite : public cocos2d::CCLayer, public cocos2d::CCRGBAProtocol, public mlib::MEventDispatcher
{
public:
    enum RemoteSpritAlignment
    {
        RemoteSpritAlignmentCenter,
        RemoteSpritAlignmentBottom
    };
public:
    CREATE_FUNC(MRemoteSprite);
    MRemoteSprite();
    ~MRemoteSprite();
    
    void setSrc(std::string srcPath);
    
    void saveCurrentOpacity();

    
    // CCRGBAProtocol
    virtual void setColor(const cocos2d::ccColor3B& color);
    virtual void setOpacity(GLubyte opacity);
    virtual const cocos2d::ccColor3B& getColor(void);
    virtual const cocos2d::ccColor3B& getDisplayedColor(void);
    virtual GLubyte getDisplayedOpacity(void);
    virtual GLubyte getOpacity(void);
    virtual bool isOpacityModifyRGB(void);
    virtual bool isCascadeColorEnabled(void);
    virtual bool isCascadeOpacityEnabled(void);
    
    virtual void setOpacityModifyRGB(bool bValue) {}
    virtual void setCascadeColorEnabled(bool cascadeColorEnabled) {}
    virtual void updateDisplayedColor(const cocos2d::ccColor3B& color) {}
    virtual void setCascadeOpacityEnabled(bool cascadeOpacityEnabled) {}
    virtual void updateDisplayedOpacity(GLubyte opacity) {}
    
    
    MLIB_DECLARE_PROPERTY_SETTER_TO_IMPL(bool, isGray);
    MLIB_DECLARE_PROPERTY(CCSize, sizeShow);
    MLIB_DECLARE_PROPERTY(RemoteSpritAlignment, alignment);
    
public:
    
    MSprite* getSprite() const { return _sprite; }
    
private:
    void setLocalSrc(std::string srcLocalPath);
    bool isSpecialSrc(std::string srcPath);
    std::string getOpacitySrc(std::string srcPath);
    bool isSpecialSrcSmall(std::string srcPath);
    std::string getOpacitySrcSmall(std::string srcPath);

    MSprite *_sprite;
    std::string _srcPath;
    cocos2d::ccColor3B _color;
    float _opacity;
    MFileDownloader *_fileDownloader;
    bool _isGrayChange;
};

MLIB_DECLARE_CCBLOADER(MRemoteSprite, CCLayer);

MLIB_NS_END

#endif /* defined(__legend__MRemoteSprite__) */
