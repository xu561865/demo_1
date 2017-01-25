//
//  NewScrollView.h
//  test_cocos2dx
//
//  Created by xuyi on 24/01/2017.
//
//

#ifndef NewScrollView_h
#define NewScrollView_h

#include "cocos2d.h"

USING_NS_CC;

class NewScrollView : public CCLayer
{
public:
    NewScrollView();
    virtual ~NewScrollView();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    
};

#endif /* NewScrollView_h */
