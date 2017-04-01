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
#include "cocos-ext.h"
#include "NewScrollView_3.h"

USING_NS_CC;


class TestScrollView : public CCLayerColor
{
public:
    TestScrollView();
    virtual ~TestScrollView();
    
    static TestScrollView * create(CCSize, ccColor4B);
    
    virtual void onEnter() override;
    virtual void onExit() override;
  
private:
    CCSize m_size;
};

class NewScrollView_3DelegateNew : public NewScrollView_3Delegate
{
public:
    virtual void scrollViewDidScroll(NewScrollView_3* view) override;
};

#endif /* TestScrollView_h */
