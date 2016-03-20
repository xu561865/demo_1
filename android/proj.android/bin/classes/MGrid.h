//
//  MGrid.h
//  legend
//
//  Created by Xu Jinyang on 4/10/13.
//
//

#ifndef __legend__MGrid__
#define __legend__MGrid__

#include <iostream>
#include "mlib_ccext.h"

MLIB_NS_BEGIN

class MGrid : public cocos2d::CCLayer, public mlib::MEventDispatcher
{
public:
    CREATE_FUNC(MGrid);
    MGrid();
    ~MGrid();
    
public:
    virtual void onEnter();
    void reloadData();
    
    MLIB_DECLARE_RETAIN_PROPERTY_SETTER_TO_IMPL(cocos2d::CCArray *, nodes);
    MLIB_DECLARE_PROPERTY(float, padding);
    MLIB_DECLARE_PROPERTY(float, gapHorizontal);
    MLIB_DECLARE_PROPERTY(float, gapVertical);
};

MLIB_DECLARE_CCBLOADER(MGrid, CCLayer);

MLIB_NS_END

#endif /* defined(__legend__MGrid__) */
