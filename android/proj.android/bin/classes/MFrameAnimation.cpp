//
//  MFrameAnimation.cpp
//  legend
//
//  Created by Xu Jinyang on 4/3/13.
//
//

#include "MFrameAnimation.h"
#include "mlib_log.h"
#include "MUtils.h"
#include "mlib_ccext.h"

USING_NS_CC;
using namespace mlib;

cocos2d::CCAnimation * MFrameAnimation::createAnimation(const char *plist, float frameRate/* = 12*/)
{
//    M_DEBUG("createAnimation: " << plist);
    
    CCAnimation *animation = CCAnimationCache::sharedAnimationCache()->animationByName(plist);
    if (animation == nullptr)
    {
        CCSpriteFrameCache *sfCache = CCSpriteFrameCache::sharedSpriteFrameCache();
        sfCache->addSpriteFramesWithFile(plist);
        
        CCArray *frames = CCArray::create();
        
        CCDictionary *dict = CCDictionary::createWithContentsOfFile(plist);
        CCDictionary *dictFrames = (CCDictionary *)dict->objectForKey("frames");
        CCArray *keys = dictFrames->allKeys();
        
        for (uint32_t i = 2; i < 10; i++)
        {
            std::string fileName = plist;
            std::string::size_type pos = fileName.find_last_of(".");
            std::string basename = fileName.substr(0, pos);
            std::string ext = fileName.substr(pos);
            
            fileName = SSTR(basename << "-" << i << ext);
            
            if (!FILE_UTILS->isFileExist(fileName))
            {
                break;
            }
            
            sfCache->addSpriteFramesWithFile(fileName.c_str());
            dict = CCDictionary::createWithContentsOfFile(fileName.c_str());
            dictFrames = (CCDictionary *)dict->objectForKey("frames");
            keys->addObjectsFromArray(dictFrames->allKeys());
        }
        
        mlib::sort_ccarray(keys, [] (const void * p1, const void * p2) -> int {
            auto str1 = * ((CCString **)p1);
            auto str2 = * ((CCString **)p2);
            
            if (str1->length() != str2->length())
            {
                return (int32_t)str1->length() - (int32_t)str2->length();
            }
            else
            {
                return str1->compare(str2->getCString());
            }
        });
        
        CCObject *obj;
        CCARRAY_FOREACH(keys, obj)
        {
            CCString *key = (CCString *)obj;
            
            CCSpriteFrame *frame = sfCache->spriteFrameByName(key->getCString());
            frames->addObject(frame);
//            M_DEBUG("fName: " << key->getCString());
        }
        
        animation = CCAnimation::createWithSpriteFrames(frames, 1/frameRate);
        CCAnimationCache::sharedAnimationCache()->addAnimation(animation, plist);
    }
    
//    M_DEBUG("createAnimation done: " << plist);
    
    return animation;
}