//
//  MSpriteLoader.h
//  legend
//
//  Created by 张杰 on 14-8-12.
//
//

#ifndef __legend__MSpriteLoader__
#define __legend__MSpriteLoader__

#include <iostream>
#include "CCSpriteLoader.h"
#include "MSprite.h"

NS_CC_EXT_BEGIN

class MSpriteLoader : public CCSpriteLoader
{
public:
    virtual ~MSpriteLoader() {}
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(MSpriteLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(mlib::MSprite);
    virtual void onHandleSpriteFile(CCNode * pNode, std::string spriteFile);
};

NS_CC_EXT_END
#endif /* defined(__legend__MSpriteLoader__) */
