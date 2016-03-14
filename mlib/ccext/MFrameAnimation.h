//
//  MFrameAnimation.h
//  legend
//
//  Created by Xu Jinyang on 4/3/13.
//
//

#ifndef __legend__MFrameAnimation__
#define __legend__MFrameAnimation__

#include <iostream>
#include "cocos2d.h"

class MFrameAnimation
{
public:
    static cocos2d::CCAnimation * createAnimation(const char *plist, float frameRate = 12);
};

#endif /* defined(__legend__MFrameAnimation__) */
