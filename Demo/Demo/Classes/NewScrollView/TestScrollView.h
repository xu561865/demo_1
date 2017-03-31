//
//  TestScrollView.h
//  test_cocos2dx
//
//  Created by xuyi on 24/01/2017.
//
//

#ifndef TestScrollView_h
#define TestScrollView_h

#include "cocos2d.h"

USING_NS_CC;

class TestScrollView : public CCLayerColor
{
public:
    TestScrollView();
    virtual ~TestScrollView();
    
    static TestScrollView * create(CCSize, ccColor4B);
    
    virtual void onEnter();
    virtual void onExit();
  
private:
    CCSize m_size;
};

#endif /* TestScrollView_h */
