//
//  MTabButton.h
//  legend
//
//  Created by Xu Jinyang on 6/28/13.
//
//

#ifndef __legend__MTabButton__
#define __legend__MTabButton__

#include <iostream>
#include "cocos-ext.h"

#include "mlib_ccext_helpers.h"

class MTabButton : public cocos2d::extension::CCControlButton
{
public:
    CREATE_FUNC(MTabButton);
    
    virtual void setSelected(bool enabled);
    virtual void setHighlighted(bool enabled);
    
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

};

MLIB_DECLARE_CCBLOADER(MTabButton, CCControlButton)

#endif /* defined(__legend__MTabButton__) */
