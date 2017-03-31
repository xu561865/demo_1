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

class NewScrollView : public CCLayerColor
{
public:
    static NewScrollView * create(CCSize, ccColor4B);
    NewScrollView();
    virtual ~NewScrollView();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    void setContainer(CCNode * );
    void setContentOffset(CCPoint offset);
    virtual void registerWithTouchDispatcher();
private:
    CCNode * m_pContainer;
    CCArray* m_pTouches;
    CCPoint m_tTouchPoint;
    bool m_bDragging;
    bool m_bTouchMoved;
    CCPoint m_tScrollDistance;
    float m_fTouchLength;
};

#endif /* NewScrollView_h */
