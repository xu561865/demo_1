//
//  MRemoteSprite.cpp
//  legend
//
//  Created by Xu Jinyang on 3/28/13.
//
//

#include "MRemoteSprite.h"

MLIB_NS_BEGIN

MLIB_REGISTER_CCBLOADER(MRemoteSprite);

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;

MRemoteSprite::MRemoteSprite()
{
    _sprite = nullptr;
    _color = ccWHITE;
    _opacity = 255;
    _fileDownloader = nullptr;
    
    isGray(false);
    _sizeShow = CCSizeZero;
    _isGrayChange = false;
    _alignment = RemoteSpritAlignmentCenter;
}

MRemoteSprite::~MRemoteSprite()
{
    if (_fileDownloader != nullptr)
    {
        delete _fileDownloader;
        _fileDownloader = nullptr;
    }
}

bool MRemoteSprite::isSpecialSrc(std::string srcPath)
{
    return (string::npos != srcPath.find("_sp.jpg"));
}

std::string MRemoteSprite::getOpacitySrc(std::string srcPath)
{
    return mlib::string_replace(srcPath, "_sp", "_spo");
}

bool MRemoteSprite::isSpecialSrcSmall(std::string srcPath)
{
    return (string::npos != srcPath.find("_spt.jpg"));
}

std::string MRemoteSprite::getOpacitySrcSmall(std::string srcPath)
{
    return "card_small_mask.jpg";
}

void MRemoteSprite::isGray(bool isGray)
{
    if (_isGray == isGray)
    {
        _isGrayChange = false;
        return;
    }
    
    _isGray = isGray;
    _isGrayChange = true;
}

void MRemoteSprite::setSrc(std::string srcPath)
{
//    M_DEBUG("this: " << (long)this << " setSrc: " << srcPath);
    
    if (_srcPath == srcPath)
    {
        if (_isGrayChange && "" != srcPath)
        {
            if (isSpecialSrc(srcPath))
            {
                string src1 = srcPath;
                string src2 = getOpacitySrc(srcPath);
                
                if (!MFileDownloader::isFileExist(src1)
                    || !MFileDownloader::isFileExist(src2))
                {
                    return;
                }
            }
            else
            {
                if (!MFileDownloader::isFileExist(srcPath))
                {
                    return;
                }
            }
            setLocalSrc(srcPath);
        }
        return;
    }

    this->removeChild(_sprite);
    _sprite = nullptr;
    _srcPath = "";
    
    if (_fileDownloader != nullptr)
    {
        _fileDownloader->removeEventListenerFor(this);
        _fileDownloader = nullptr;
    }
    
    if (srcPath == "")
    {
        return;
    }
    
    vector<string> urls;
    if (isSpecialSrc(srcPath))
    {
        string src1 = srcPath;
        string src2 = getOpacitySrc(srcPath);
        
        if (!MFileDownloader::isFileExist(src1))
        {
            urls.push_back(src1);
        }
        if (!MFileDownloader::isFileExist(src2))
        {
            urls.push_back(src2);
        }
    }
    else
    {
        if (!MFileDownloader::isFileExist(srcPath))
        {
            urls.push_back(srcPath);
        }
    }
    
    if (urls.empty())
    {
        setLocalSrc(srcPath);
    }
    else
    {
        // need to download

        //if (_fileDownloader != nullptr)
        //{
           // delete _fileDownloader;
////            M_DEBUG("downloader deleted: " << (long)_fileDownloader << " this=" << (long)this);
            //_fileDownloader = nullptr;
        //}
        M_DEBUG("need to download ::::::");
        this->dispatchEvent(EVENT_CARD_SKETCH);
        _fileDownloader = new MFileDownloader;
        _fileDownloader->addEventListener(EVENT_FILE_DOWNLOADED, [this, srcPath](mlib::MEvent *evt) {
            this->setLocalSrc(srcPath);
            _fileDownloader = nullptr;
        }, this);
        _fileDownloader->addEventListener(EVENT_FILE_DOWNLOAD_FAILED, [this](mlib::MEvent *evt) {
            // download error
            _srcPath = "";
            _fileDownloader = nullptr;
        }, this);
        
        _fileDownloader->download(urls);
        
        _srcPath = srcPath;
    }
}

void MRemoteSprite::setLocalSrc(std::string srcLocalPath)
{
    srcLocalPath = MFileDownloader::getLocalFilePath(srcLocalPath);
 
    if (_sprite != nullptr)
    {
        _sprite->removeFromParent();
        _sprite = nullptr;
    }
    
    CCImage *image = nullptr;
    if (isSpecialSrc(srcLocalPath))
    {
        image = get_special_ccimage(srcLocalPath, getOpacitySrc(srcLocalPath));
    }
    else if (isSpecialSrcSmall(srcLocalPath))
    {
        image = get_special_ccimage(srcLocalPath, getOpacitySrcSmall(srcLocalPath));
    }
    else
    {
        image = get_ccimage(srcLocalPath);
    }
    
    if (image == nullptr)
    {
        M_DEBUG( "!!! get image failed, file >> " << srcLocalPath );
        this->dispatchEvent(EVENT_CARD_DOWNLOAD_OK);
        return ;
    }
    
    CCTexture2D *texture = nullptr;
    if (isGray())
    {
        texture = mlib::create_gray_texture(image);
    }
    else
    {
        texture = new CCTexture2D();
        texture->autorelease();
        
        if( texture &&
           texture->initWithImage(image) )
        {
        }
    }
    
    if (texture != nullptr)
    {
        if (sizeShow().equals(CCSizeZero))
        {
            _sprite = MSprite::createWithTexture(texture);
        }
        else
        {
            CCSize cSize = texture->getContentSize();
            _sprite = MSprite::createWithTexture(texture, CCRectMake((cSize.width - sizeShow().width) / 2, (cSize.height - sizeShow().height) / 2, sizeShow().width, sizeShow().height));
        }
    }
    
    if (_sprite != nullptr)
    {
        if (alignment() == RemoteSpritAlignmentBottom)
        {
            _sprite->setAnchorPoint(ccp(0.5, 0));
            _sprite->setPosition(ccp(getContentSize().width/2, 0));
        }
        else    // default alignment: center
        {
            _sprite->setAnchorPoint(ccp(0.5, 0.5));
            _sprite->setPosition(ccp(getContentSize().width/2, getContentSize().height/2));
        }
        
        _sprite->setColor(_color);
        _sprite->setOpacity(_opacity);
        this->addChild(_sprite);
        _srcPath = srcLocalPath;
    }
    _isGrayChange = false;
    this->dispatchEvent(EVENT_CARD_DOWNLOAD_OK);
}

void MRemoteSprite::setColor(const cocos2d::ccColor3B &color)
{
    _color = color;
    if (_sprite != nullptr)
    {
        _sprite->setColor(_color);
    }
}

void MRemoteSprite::setOpacity(GLubyte opacity)
{
    _opacity = opacity;
    if (_sprite != nullptr)
    {
        _sprite->setOpacity(_opacity);
    }
}

const cocos2d::ccColor3B& MRemoteSprite::getColor(void)
{
    if (_sprite == nullptr)
    {
        return ccWHITE;
    }
    
    return _sprite->getColor();
}

const cocos2d::ccColor3B& MRemoteSprite::getDisplayedColor(void)
{
    if (_sprite == nullptr)
    {
        return ccWHITE;
    }
    
    return _sprite->getDisplayedColor();
}

GLubyte MRemoteSprite::getDisplayedOpacity(void)
{
    if (_sprite == nullptr)
    {
        return 255;
    }
    
    return _sprite->getDisplayedOpacity();
}

GLubyte MRemoteSprite::getOpacity(void)
{
    if (_sprite == nullptr)
    {
        return 255;
    }
    
    return _sprite->getOpacity();
}

bool MRemoteSprite::isOpacityModifyRGB(void)
{
    if (_sprite == nullptr)
    {
        return false;
    }
    
    return _sprite->isOpacityModifyRGB();
}

bool MRemoteSprite::isCascadeColorEnabled(void)
{
    if (_sprite == nullptr)
    {
        return false;
    }
    
    return _sprite->isCascadeColorEnabled();
}

bool MRemoteSprite::isCascadeOpacityEnabled(void)
{
    if (_sprite == nullptr)
    {
        return false;
    }
    
    return _sprite->isCascadeOpacityEnabled();
}

void MRemoteSprite::saveCurrentOpacity()
{
    if (_sprite != nullptr)
    {
        _opacity = _sprite->getOpacity();
    }
}

MLIB_NS_END
