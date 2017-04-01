//
//  NewScrollView_2.h
//  test_cocos2dx
//
//  Created by xuyi on 24/01/2017.
//
//

#ifndef NewScrollView_2_h
#define NewScrollView_2_h

#include "cocos2d.h"

USING_NS_CC;

enum class ScrollViewDirection2
{
    kScrollViewDirectionNone = -1,
    kScrollViewDirectionHorizontal = 0,
    kScrollViewDirectionVertical,
    kScrollViewDirectionBoth
} ;

class NewScrollView_2 : public CCLayerColor
{
public:
    static NewScrollView_2 * create(CCSize, ccColor4B);
    NewScrollView_2();
    virtual ~NewScrollView_2();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onEnterTransitionDidFinish();
    virtual void registerWithTouchDispatcher();
    virtual void visit();
    
    void setContainer(CCNode * );
    void setContentOffset(CCPoint offset, bool animated = false);
    void beforeDraw();
    void afterDraw();
    CCRect getViewRect();
    void setDirection(ScrollViewDirection2 eDirection) { m_eDirection = eDirection; }
    
private:
    void deaccelerateScrolling(float dt);
    CCPoint maxContainerOffset();
    CCPoint minContainerOffset();
    void relocateContainer(bool animated);
    void setContentOffsetInDuration(CCPoint offset, float dt);
    void performedAnimatedScroll(float dt);
    void stoppedAnimatedScroll(CCNode * node);
    
private:
    CCNode * m_pContainer;
    CCArray* m_pTouches;
    CCPoint m_tTouchPoint;
    bool m_bDragging;
    bool m_bTouchMoved;
    CCPoint m_tScrollDistance;
    float m_fTouchLength;
    
    ScrollViewDirection2 m_eDirection;
};

#endif /* NewScrollView_2_h */
