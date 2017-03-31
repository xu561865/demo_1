//
//  NewScrollView_1.h
//  test_cocos2dx
//
//  Created by xuyi on 24/01/2017.
//
//

#ifndef NewScrollView_1_h
#define NewScrollView_1_h

#include "cocos2d.h"

USING_NS_CC;

class NewScrollView_1 : public CCLayerColor
{
public:
    static NewScrollView_1 * create(CCSize, ccColor4B);
    NewScrollView_1();
    virtual ~NewScrollView_1();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    virtual void onEnterTransitionDidFinish();
    virtual void registerWithTouchDispatcher();
    virtual void visit();
    
    void setContainer(CCNode * );
    void setContentOffset(CCPoint offset);
    void beforeDraw();
    void afterDraw();
    CCRect getViewRect();
private:
    CCNode * m_pContainer;
    CCArray* m_pTouches;
    CCPoint m_tTouchPoint;
    bool m_bDragging;
    bool m_bTouchMoved;
    CCPoint m_tScrollDistance;
    float m_fTouchLength;
};

#endif /* NewScrollView_1_h */
