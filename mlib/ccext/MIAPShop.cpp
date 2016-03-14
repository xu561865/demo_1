//
//  MIAPShop.cpp
//  legend
//
//  Created by Minhao Zhang on 8/7/13.
//
//

#include "MIAPShop.h"

MLIB_NS_BEGIN

MIAPShop * MIAPShop::sharedInstance()
{
    static MIAPShop * inst = nullptr;
    if (inst == nullptr)
    {
        inst = new MIAPShop;
    }
    return inst;
}

MLIB_NS_END
