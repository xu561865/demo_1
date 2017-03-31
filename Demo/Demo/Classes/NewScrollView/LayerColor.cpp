//
//  LayerColor.cpp
//  test_cocos2dx
//
//  Created by xuyi on 31/03/2017.
//
//

#include "LayerColor.h"

LayerColor * LayerColor::create(CCSize size, ccColor4B c4)
{
    LayerColor * lay = new LayerColor();
    if(lay)
    {
        lay->setContentSize(size);
        lay->initWithColor(c4, size.width, size.height);
        
        lay->setTouchEnabled(true);
        lay->autorelease();
    }
    
    return lay;
}

LayerColor::LayerColor()
{
    
}

LayerColor::~LayerColor()
{
    
}

void LayerColor::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, getTouchPriority(), false);
}

bool LayerColor::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void LayerColor::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    //
}

void LayerColor::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void LayerColor::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
