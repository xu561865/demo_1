//
//  LayerColor.h
//  test_cocos2dx
//
//  Created by xuyi on 31/03/2017.
//
//

#ifndef LayerColor_h
#define LayerColor_h

#include "cocos2d.h"

USING_NS_CC;

class LayerColor : public CCLayerColor
{
public:
    static LayerColor * create(CCSize, ccColor4B);
    LayerColor();
    virtual ~LayerColor();
    
    virtual void registerWithTouchDispatcher() override;
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) override;
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) override;
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) override;
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) override;
};

#endif /* LayerColor_h */
