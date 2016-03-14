//
//  MAnimation.h
//  MAnimation
//
//  Created by Aeriro on 15/4/9.
//
//

#ifndef __MAnimation__MAnimation__
#define __MAnimation__MAnimation__

#include <stdio.h>
#include <string>
#include "cocos2d.h"
#include <vector>
#include <map>

/*
    使用方法：
    MAnimation* m = MAnimation::create();
    //m->retain(); //如果想自己保留一份MAnimation请自行retain()
    m->loadFile($ConfigFilePath); //例如"1.xml"
    m->play(parent); //parent为要执行动画的父节点， 类型为CCNode或者是其子类
 **/

class MAnimation : public cocos2d::CCObject
{
public:
    struct MAnimationProperty
    {
        std::string name;  //对应动画的名字
        
        float scaleX;
        float scaleY;
        
        float skewX;
        float skewY;
        
        float rotation;
        
        float positionX, positionY;
        float anchorPointX, anchorPointY;
        
        MAnimationProperty() : scaleX(1.f), scaleY(1.f), skewX(0.f), skewY(0.f),
        rotation(0.f), positionX(0.f), positionY(0.f), anchorPointX(0.5f), anchorPointY(0.5f)
        {
        }
        
    };
    
private:
    void detachAnimatedSprite(cocos2d::CCNode* animatedSprite);
    
private:
    std::string version;
    std::map<std::string, cocos2d::CCAnimation*>* nameToCCAnimation; //动画名字 对应 CCAnimation
    std::map<std::string, MAnimationProperty*>* nameToAnimationProperty; //动画名字 对应 动画属性
    std::multimap<cocos2d::CCSprite*, cocos2d::CCSprite*>* parentToAnimatedSprite; //运行动画的父节点 对应 动画执行节点s
    
public:
    MAnimation();
    ~MAnimation();
    
    CREATE_FUNC(MAnimation);
    
    bool init();
    
    void loadFile(const std::string& fileName);
    cocos2d::CCAnimation* getCCAnimationByName(const std::string& name);
    
    void setAnimationProperty(const MAnimationProperty& property);
    
    /*
        播放配置文件中的动画
        动画播放完成后回自动释放动画节点
     **/
    void play(cocos2d::CCSprite* parent);
    void stop(cocos2d::CCSprite* parent);
};


#endif /* defined(__MAnimation__MAnimation__) */
