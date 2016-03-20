//
//  MAnimation.cpp
//  MAnimation
//
//  Created by Aeriro on 15/4/9.
//
//

#include "MAnimation.h"
#include "support/tinyxml2/tinyxml2.h"
#include <cstdarg>
#include <vector>
#include <cstdio>
#include <string>
#include <cstring>

USING_NS_CC;

MAnimation::MAnimation() : version("1.0.1")
{
    nameToCCAnimation = new std::map<std::string, CCAnimation*>;
    nameToAnimationProperty = new std::map<std::string, MAnimationProperty*>;
    parentToAnimatedSprite = new std::multimap<cocos2d::CCSprite*, cocos2d::CCSprite*>;
}
MAnimation::~MAnimation()
{
    std::map<std::string, CCAnimation*>::iterator it0 = nameToCCAnimation->begin();
    while(it0 != nameToCCAnimation->end())
    {
        CC_SAFE_RELEASE((*it0).second);
        ++it0;
    }
    delete  nameToCCAnimation;
    
    std::map<std::string, MAnimationProperty*>::iterator it1 = nameToAnimationProperty->begin();
    while(it1 != nameToAnimationProperty->end())
    {
        CC_SAFE_DELETE((*it1).second);
        ++it1;
    }
    delete nameToAnimationProperty;
    
    
    std::multimap<cocos2d::CCSprite*, cocos2d::CCSprite*>::iterator it2 = parentToAnimatedSprite->begin();
    while(it2 != parentToAnimatedSprite->end())
    {
        CC_SAFE_RELEASE((*it2).first);
        CC_SAFE_RELEASE((*it2).second);
        
        ++it2;
    }
    delete parentToAnimatedSprite;
}

bool MAnimation::init()
{
    return true;
}

void MAnimation::detachAnimatedSprite(CCNode* animatedSprite)
{
    if(animatedSprite != NULL)
    {
        animatedSprite->removeFromParentAndCleanup(true);
    }
}


void MAnimation::loadFile(const std::string& fileName)
{
    tinyxml2::XMLDocument doc;
    //std::string s = CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName.c_str());
    //doc.LoadFile(s.c_str());
    doc.LoadFile(fileName.c_str());
    
    tinyxml2::XMLElement* root = doc.RootElement();
    if(std::strcmp(version.c_str(), root->FirstAttribute()->Value()) != 0)
    {
        CCAssert(0, "版本不匹配");
    }

    tinyxml2::XMLElement* animation;
    animation = root->FirstChildElement("animation");
    while(animation != NULL)
    {
        tinyxml2::XMLElement* ele = animation->FirstChildElement("animationName");
        std::string animationName = ele->GetText();
        
        //属性
        MAnimationProperty* property = new MAnimationProperty;
        float scale; float rotate;
        CCPoint position = CCPointZero;
        std::string strPosition = animation->FirstAttribute()->Value(); //position
        std::string::size_type pos = strPosition.find(",");
        std::string strX, strY;
        strX = strPosition.substr(0, pos);
        strY = strPosition.substr(pos + 1, strPosition.length());
        float x = std::atof(strX.c_str()), y = std::atof(strY.c_str());
        position.setPoint(x, y);
        animation->QueryFloatAttribute("scale", &scale);
        animation->QueryFloatAttribute("rotation", &rotate);
        property->positionX = x;
        property->positionY = y;
        property->scaleX = property->scaleY = scale;
        property->rotation = rotate;
        nameToAnimationProperty->insert(std::make_pair(animationName, property));
        
        int frameCount;
        ele = animation->FirstChildElement("frameCount");
        ele->QueryIntText(&frameCount);
        
        float delayPerUnit;
        ele = animation->FirstChildElement("delayPerUnit");
        ele->QueryFloatText(&delayPerUnit);
        
        bool restoreOriginFrame;
        ele = animation->FirstChildElement("restoreOriginFrame");
        ele->QueryBoolText(&restoreOriginFrame);
        
        int loops;
        ele = animation->FirstChildElement("loops");
        ele->QueryIntText(&loops);
        
        //创建CCAnimation
        CCArray* arr = CCArray::create();
        CCAnimation* ccanimation;
        ccanimation = CCAnimationCache::sharedAnimationCache()->animationByName(animationName.c_str());
        if(ccanimation == NULL)
        {
            std::string plistName = animationName;
            plistName += ".plist";
            CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
            cache->addSpriteFramesWithFile(plistName.c_str());
            char c[100];
            for(int i = 0; i < frameCount; ++i)
            {
                sprintf(c, "%s_%02d.png", animationName.c_str(), i);
                CCSpriteFrame* frame = cache->spriteFrameByName(c);
                arr->addObject(frame);
            }
            ccanimation = CCAnimation::createWithSpriteFrames(arr);
            ccanimation->setDelayPerUnit(delayPerUnit);
            ccanimation->setRestoreOriginalFrame(restoreOriginFrame);
            ccanimation->setLoops(loops);
            CCAnimationCache::sharedAnimationCache()->addAnimation(ccanimation, animationName.c_str()); //加入缓存
        }
        ccanimation->retain();
        nameToCCAnimation->insert(std::make_pair(animationName, ccanimation));
        
        animation = animation->NextSiblingElement("animation");
    }
    
}

cocos2d::CCAnimation* MAnimation::getCCAnimationByName(const std::string& name)
{
    if(nameToCCAnimation->find(name) == nameToCCAnimation->end())
        return NULL;
    
    return nameToCCAnimation->operator[](name);
}

void MAnimation::setAnimationProperty(const MAnimationProperty& property)
{}

void MAnimation::play(cocos2d::CCSprite* parent)
{
    std::map<std::string, cocos2d::CCAnimation*>::iterator it = nameToCCAnimation->begin();
    while(it != nameToCCAnimation->end())
    {
        CCSprite* s = CCSprite::create();
        MAnimationProperty* property = nameToAnimationProperty->operator[]((*it).first);
        s->setPositionX(property->positionX);
        s->setPositionY(property->positionY);
        s->setScaleX(property->scaleX);
        s->setScaleY(property->scaleY);
        s->setRotation(property->rotation);
        parent->addChild(s);
        
        parent->retain(); //保留parent与animatedSprite的映射
        s->retain();
        parentToAnimatedSprite->insert(std::make_pair(parent, s));
        
        CCAnimate* ani = CCAnimate::create((*it).second);
        CCSequence* seq = CCSequence::create(ani, CCCallFuncN::create(this, callfuncN_selector(MAnimation::detachAnimatedSprite)), NULL);
        s->runAction(seq);
        ++it;
    }
}

void MAnimation::stop(CCSprite* parent)
{
}
