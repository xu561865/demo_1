//
//  MSpriteLoader.cpp
//  legend
//
//  Created by 张杰 on 14-8-12.
//
//

#include "MSpriteLoader.h"
#include "mlib_ccext.h"
NS_CC_EXT_BEGIN;

MLIB_REGISTER_CCBLOADER(MSprite);

void MSpriteLoader::onHandleSpriteFile(CCNode * pNode, std::string spriteFile)
{
    ((mlib::MSprite *)pNode)->srcPath(spriteFile);
}

NS_CC_EXT_END;