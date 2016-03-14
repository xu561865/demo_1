//
//  MUtils.cpp
//  legend
//
//  Created by Xu Jinyang on 13-3-11.
//
//

#include "MUtils.h"
#include "mlib.h"
#include "mlib_ccext.h"
#include "CCBReader.h"

USING_NS_CC;

MLIB_NS_BEGIN

float get_content_scale_factor()
{
    CCSize frameSize = CCEGLView::sharedOpenGLView()->getFrameSize();
    if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    {
        switch (int(frameSize.width * frameSize.height))
        {
            case 480*320:
                return 0.5;
                break;
            case 960*640:
                return 1;
                break;
            case 1136*640:
                return 1;
                break;
            case 1024*768:
                return 1;
                break;
            case 2048*1536:
                return 2;
                break;
                
            default:
            {
                auto width = frameSize.width;
                if (width > frameSize.height)
                {
                    width = frameSize.height;
                }
                return width/640.f;
                break;
            }
        }
    }
    else
    {
        switch (int(frameSize.width * frameSize.height))
        {
            case 320*240:
                return 240/640.f;
                break;
            case 480*320:
                return 320/640.f;
                break;
            case 640*460:
                return 460/640.f;
                break;
            case 800*480:
                return 480/640.f;
                break;
            case 854*480:
                return 480/640.f;
                break;
            case 888*540:
                return 540/640.f;
                break;
            case 960*540:
                return 540/640.f;
                break;
            case 960*640:
            {
                if (frameSize.width > 1000 || frameSize.height > 1000)
                {
                    // 1024*600
                    return 600/640.f;
                }
                else
                {
                    return 640/640.f;
                }
                break;
            }
            case 1184*720:
                return 720/640.f;
                break;
            case 1280*720:
                return 720/640.f;
                break;
            case 1280*768:
                return 768/640.f;
                break;
            case 1280*800:
                return 800/640.f;
                break;
            case 1800*1080:
                return 1080/640.f;
                break;
            case 1920*1080:
                return 1080/640.f;
                break;
                
            default:
            {
                auto width = frameSize.width;
                if (width > frameSize.height)
                {
                    width = frameSize.height;
                }
                return width/640.f;
                break;
            }
        }
    }
    
    return 1;
}

CCSize get_design_resolution_size()
{
    CCSize frameSize = CCEGLView::sharedOpenGLView()->getFrameSize();
    float scaleFactor = get_content_scale_factor();
    
    auto size = CCSizeMake(frameSize.width/scaleFactor, frameSize.height/scaleFactor);
//    M_ERROR("resolution size: " << size.width << ", " << size.height);
    return size;
}

CCSpriteFrame * get_sprite_frame(const std::string & fileName)
{
    auto frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fileName.c_str());
    if (frame == nullptr)
    {
        CCTexture2D *pTexture = CCTextureCache::sharedTextureCache()->addImage(fileName.c_str());
        if (pTexture == nullptr)
        {
            return nullptr;
        }
        CCRect rect = CCRectZero;
        rect.size = pTexture->getContentSize();
        frame = CCSpriteFrame::createWithTexture(pTexture, rect);
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(frame, fileName.c_str());
    }
    
    return frame;
}

void change_sprite_frame(CCSprite *sprite, std::string fileName)
{
    auto mSprite = dynamic_cast<mlib::MSprite *>(sprite);
    if (nullptr != mSprite)
    {
        mSprite->srcPath(fileName);
        mSprite->reloadSrc();
    }
    else
    {
        auto frame = get_sprite_frame(fileName);
        if (frame != nullptr)
        {
            sprite->setDisplayFrame(frame);
        }
    }
}

int change_s9sprite_frame(cocos2d::extension::CCScale9Sprite *sprite, const std::string & fileName, const CCSize & size, const CCRect &inset)
{
    if (sprite == nullptr || fileName.empty())
    {
        return -1;
    }
    
    auto frame = get_sprite_frame(fileName);
    if (frame == nullptr)
    {
        return -2;
    }
    
    sprite->setSpriteFrame(frame);
    
    sprite->setInsetLeft(inset.origin.x );
    sprite->setInsetRight(inset.origin.y);
    sprite->setInsetTop(inset.size.width);
    sprite->setInsetBottom(inset.size.height);
    
    sprite->setPreferredSize(size);
    
    return 0;
}

int change_sprite_texture_to_gray(cocos2d::CCSprite *sprite, const std::string & fileName)
{
    if (sprite == nullptr || fileName.empty())
    {
        return -1;
    }
    
    auto image = get_ccimage(fileName);
    if (image == nullptr)
    {
        return -2;
    }
    
    auto texture = mlib::create_gray_texture(image);
    if (texture == nullptr)
    {
        return -3;
    }
    
    CCRect rect = CCRectZero;
    rect.size = texture->getContentSize();
    auto frame = CCSpriteFrame::createWithTexture(texture, rect);
    if (frame != nullptr)
    {
        sprite->setDisplayFrame(frame);
    }
    
    return 0;
}

cocos2d::CCImage * get_ccimage(std::string file)
{
    CCImage* pImage = NULL;
    
    std::string pathKey = file;
    
    pathKey = CCFileUtils::sharedFileUtils()->fullPathForFilename(pathKey.c_str());
    if (pathKey.size() == 0)
    {
        return NULL;
    }
    
    std::string fullpath = pathKey;
    
    std::string lowerCase(pathKey);
    for (unsigned int i = 0; i < lowerCase.length(); ++i)
    {
        lowerCase[i] = tolower(lowerCase[i]);
    }
    // all images are handled by UIImage except PVR extension that is handled by our own handler
    do
    {
        if (std::string::npos != lowerCase.find(".pvr"))
        {
            M_ASSERT(false);
            // texture = this->addPVRImage(fullpath.c_str());
        }
        else
        {
            CCImage::EImageFormat eImageFormat = CCImage::kFmtUnKnown;
            if (std::string::npos != lowerCase.find(".png"))
            {
                eImageFormat = CCImage::kFmtPng;
            }
            else if (std::string::npos != lowerCase.find(".jpg") || std::string::npos != lowerCase.find(".jpeg"))
            {
                eImageFormat = CCImage::kFmtJpg;
            }
            else if (std::string::npos != lowerCase.find(".tif") || std::string::npos != lowerCase.find(".tiff"))
            {
                eImageFormat = CCImage::kFmtTiff;
            }
            else if (std::string::npos != lowerCase.find(".webp"))
            {
                eImageFormat = CCImage::kFmtWebp;
            }
            
            pImage = new CCImage();
            CC_BREAK_IF(NULL == pImage);
            pImage->autorelease();
            
            unsigned long nSize = 0;
            unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData(fullpath.c_str(), "rb", &nSize);
            
            bool bRet = pImage->initWithImageData((void*)pBuffer, nSize, eImageFormat);
            CC_SAFE_DELETE_ARRAY(pBuffer);
            if (!bRet)
            {
                return nullptr;
            }
        }
    }
    while (0);
    
    return pImage;
}

void image_to_gray(cocos2d::CCImage *pImage)
{
    unsigned char *buffer = pImage->getData();
    uint32_t height = pImage->getHeight();
    uint32_t width = pImage->getWidth();
    
    int bitsPerPixel = pImage->hasAlpha() ? 32 : 24;
    int bytesPerRow    = (bitsPerPixel/8) * width;
    
    uint32_t  x, y;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            uint8_t*  tmp;
            tmp = buffer + y * bytesPerRow + x * (bitsPerPixel/8); // RGBAの4つ値をもっているので、1ピクセルごとに*4してずらす
            
            // RGB値を取得
            uint8_t red,green,blue;
            red = *(tmp + 0);
            green = *(tmp + 1);
            blue = *(tmp + 2);
            
            uint8_t brightness;
            
            switch (1) {
                case 1://モノクロ
                    // 輝度計算
                    brightness = (77 * red + 28 * green + 151 * blue) / 256;
                    
                    *(tmp + 0) = brightness;
                    *(tmp + 1) = brightness;
                    *(tmp + 2) = brightness;
                    break;
                    
                case 2://セピア
                    *(tmp + 0) = red;
                    *(tmp + 1) = green * 0.7;
                    *(tmp + 2) = blue * 0.4;
                    break;
                    
                case 3://色反転
                    *(tmp + 0) = 255 - red;
                    *(tmp + 1) = 255 - green;
                    *(tmp + 2) = 255 - blue;
                    break;
                    
                default:
                    *(tmp + 0) = red;
                    *(tmp + 1) = green;
                    *(tmp + 2) = blue;
                    break;
            }
        }
    }
}

cocos2d::CCTexture2D * create_gray_texture(cocos2d::CCImage *pImage)
{
    CCTexture2D * texture = NULL;
    
    do
    {
        image_to_gray(pImage);
        
        texture = new CCTexture2D();
        texture->autorelease();
        
        if( texture &&
           texture->initWithImage(pImage) )
        {
        }
    }
    while (0);

    //pthread_mutex_unlock(m_pDictLock);
    return texture;
}


cocos2d::CCTexture2D * create_mask_texture_by_opacity(const std::string & file, const std::string & opacity_file, bool isGray)
{
    static std::map<std::string, bool> s_mapFile2IsGray;
    if (s_mapFile2IsGray.find(file) == s_mapFile2IsGray.end())
        s_mapFile2IsGray[file] = isGray;
    else
        if (s_mapFile2IsGray[file] != isGray)
        {
            s_mapFile2IsGray[file] = isGray;
            CCTextureCache::sharedTextureCache()->removeTextureForKey(file.c_str());
        }
    
    CCTexture2D * texture = nullptr;
    CCImage *image = nullptr;

    if (!isGray)
    {
        texture = CCTextureCache::sharedTextureCache()->textureForKey(file.c_str());
        if(texture == nullptr)
        {
            image = get_special_ccimage(file, opacity_file);
            if (image == nullptr)
            {
                return nullptr;
            }
            texture = CCTextureCache::sharedTextureCache()->addUIImage(image, file.c_str());
        }
        
        return texture;
    }
    else
    {
//        const char* key = SSTR(file << "_gray").c_str();
        const char* key = file.c_str();
        
        auto grayTexture = CCTextureCache::sharedTextureCache()->textureForKey(key);
        if (grayTexture == nullptr)
        {
            image = get_special_ccimage(file, opacity_file);
            if (image == nullptr)
            {
                return nullptr;
            }
            mlib::image_to_gray(image);
            grayTexture = CCTextureCache::sharedTextureCache()->addUIImage(image, key);
        }
        
        
        return grayTexture;
//        return mlib::create_gray_texture(image);
    }
    
    /*
    CCProfilingBeginTimingBlock("initWithImage");
    auto texture = new CCTexture2D();
    texture->autorelease();
    
    if(texture && texture->initWithImage(image))
    {
        CCProfilingEndTimingBlock("initWithImage");
        return texture;
    }
     */
    
    return texture;
}


int change_sprite_texture_by_mask(cocos2d::CCSprite * sprite, const std::string & file, const std::string & opacity_file, bool isGray)
{
    if (sprite == nullptr)
    {
        return -1;
    }
    
    auto texture = create_mask_texture_by_opacity(file, opacity_file, isGray);
    if (texture == nullptr)
    {
        return -2;
    }
    
    sprite->setTexture(texture);
    return 0;
}


cocos2d::CCImage * get_special_ccimage(std::string file, std::string opacityFile)
{
    CCImage *image = get_ccimage(file);
    CCImage *imageOpacity = get_ccimage(opacityFile);
    
    if (image == nullptr || imageOpacity == nullptr
        || image->getDataLen() != imageOpacity->getDataLen())
    {
        M_ERROR("error file");
        return nullptr;
    }
    
    CCImage *imageSpecial = new CCImage();
    unsigned char *buffer = new unsigned char[4 * image->getWidth() * image->getHeight()];
    
    do
    {
        CC_BREAK_IF(NULL == imageSpecial);
        imageSpecial->autorelease();
        
        unsigned char *bufferOri = image->getData();
        uint32_t height = image->getHeight();
        uint32_t width = image->getWidth();
        
        int bitsPerPixel = image->hasAlpha() ? 32 : 24;
        int bytesPerRow    = (bitsPerPixel/8) * width;
        
        unsigned char *bufferOpacity = imageOpacity->getData();
        uint32_t offset_x = bitsPerPixel>>3;
        uint32_t  x, y;
        for (y = 0; y < height; y++) {
            uint32_t offset_y = y * bytesPerRow;
            for (x = 0; x < width; x++) {
                uint8_t *tmp = buffer + ((y * width + x)<<2) ;
                uint8_t *tmpOri = bufferOri + offset_y + x * offset_x;
                uint8_t *tmpOpacity = bufferOpacity + offset_y + x * offset_x;
                
                uint8_t alpha = *(tmpOpacity);
                
                memcpy(tmp, tmpOri, 3);
//                *(tmp + 0) = *(tmpOri + 0);
//                *(tmp + 1) = *(tmpOri + 1);
//                *(tmp + 2) = *(tmpOri + 2);
                *(tmp + 3) = alpha;
            }
        }
        
        if (!imageSpecial->initWithImageData(buffer, 4 * width * height, CCImage::kFmtRawData, width, height, 8))
        {
            imageSpecial = nullptr;
            break;
        }
    }
    while (0);
    
    delete [] buffer;
    
    return imageSpecial;
}

cocos2d::CCArray * create_ccarray_without_copy(cocos2d::CCArray * array)
{
    CCArray * ret = CCArray::createWithCapacity(array->capacity());
    CCObject * obj = nullptr;
    CCARRAY_FOREACH(array, obj)
    {
        ret->addObject(obj);
    }
    return ret;
}

cocos2d::CCSet * create_ccset_without_copy(cocos2d::CCSet * set)
{
    CCSet * ret = CCSet::create();
    for (auto obj : *set)
    {
        ret->addObject(obj);
    }
    return ret;
}

void sort_ccarray(cocos2d::CCArray * array,  CCArray_Comparator_t comparator)
{
    if (array == nullptr) return;
    qsort(
          array->data->arr,
          array->data->num,
          sizeof(CCObject *),
          comparator
          );
}

void initialize_resolution()
{
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCEGLView *pEGLView = CCEGLView::sharedOpenGLView();
    pDirector->setOpenGLView(pEGLView);
    
    CCSize drSize = mlib::get_design_resolution_size();
    pEGLView->setDesignResolutionSize(drSize.width, drSize.height, kResolutionShowAll);
}

cocos2d::CCNode * get_root_parent_of_node(cocos2d::CCNode * node)
{
    if (node == nullptr)
    {
        return nullptr;
    }
    
    cocos2d::CCNode * rootParent = nullptr;
    
    for (CCNode *tmpNode = node->getParent(); tmpNode != nullptr; tmpNode = tmpNode->getParent())
    {
        rootParent = tmpNode;
    }
    
    return rootParent;
}

float get_real_scale(cocos2d::CCNode * node)
{
    if (node == nullptr)
    {
        return 0.0f;
    }
    
    float scale = node->getScale();
    for (CCNode *tmpNode = node->getParent(); tmpNode != nullptr; tmpNode = tmpNode->getParent())
    {
        scale *= tmpNode->getScale();
    }
    
    return scale;
}

float get_angle(const CCPoint & startPosition, const CCPoint & endPosition)
{
    CCPoint pt = ccpSub(endPosition, startPosition);
    return CC_RADIANS_TO_DEGREES(ccpToAngle(pt));
}

CCSize get_content_size(CCNode * node)
{
    if (node == nullptr)
    {
        return CCSizeZero;
    }
    
    auto content_size = node->getContentSize();
    
    if (typeid(*node) == typeid(CCSprite)
        || typeid(*node) == typeid(CCControlButton)
        || typeid(*node) == typeid(CCScale9Sprite)
        || typeid(*node) == typeid(CCLabelBMFont))
    {
        content_size.width *= node->getScale();
        content_size.height *= node->getScale();
    }
    
    return content_size;
}


MLIB_NS_END