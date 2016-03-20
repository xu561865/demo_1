//
//  MButton.cpp
//  legend
//
//  Created by Xu Jinyang on 6/26/13.
//
//

#include "MButton.h"

#define PROPERTY_STROKE_COLOR "stroke_color"
#define PROPERTY_STROKE_SIZE "stroke_size"

#define PROPERTY_SHADOW_

MButton::MButton()
{
    _strokeColor = cocos2d::ccBLACK;
    _strokeSize = 0;
    
    _shadowOffset = cocos2d::CCSizeMake(0, 0);
    _shadowOpacity = 0;
    _shadowBlur = 0;
}

