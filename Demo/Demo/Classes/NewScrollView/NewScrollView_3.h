//
//  NewScrollView_3.h
//  test_cocos2dx
//
//  Created by xuyi on 24/01/2017.
//
//

#ifndef NewScrollView_3_h
#define NewScrollView_3_h

#include "cocos2d.h"

USING_NS_CC;

enum class ScrollViewDirection3
{
    kScrollViewDirectionNone = -1,
    kScrollViewDirectionHorizontal = 0,
    kScrollViewDirectionVertical,
    kScrollViewDirectionBoth
};

class NewScrollView_3;

class NewScrollView_3Delegate
{
public:
    virtual ~NewScrollView_3Delegate() {}
    virtual void scrollViewDidScroll(NewScrollView_3* view) = 0;
};

class NewScrollView_3 : public CCLayerColor
{
public:
    static NewScrollView_3 * create(CCSize, ccColor4B);
    NewScrollView_3();
    virtual ~NewScrollView_3();
    
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
    void setDirection(ScrollViewDirection3 eDirection) { m_eDirection = eDirection; }
    NewScrollView_3Delegate* getDelegate() { return m_pDelegate; }
    void setDelegate(NewScrollView_3Delegate* pDelegate) { m_pDelegate = pDelegate; }
    CCPoint getContainerPos();
    
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
    
    ScrollViewDirection3 m_eDirection;
    NewScrollView_3Delegate* m_pDelegate;
};

#endif /* NewScrollView_3_h */
