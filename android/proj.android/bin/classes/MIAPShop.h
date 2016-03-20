//
//  MIAPShop.h
//  legend
//
//  Created by Minhao Zhang on 8/7/13.
//
//

#ifndef __legend__MIAPShop__
#define __legend__MIAPShop__

#include <iostream>
#include "mlib.h"

MLIB_NS_BEGIN

MLIB_DEFINE_EVENT(EVENT_IAP_SHOP_ITEM_PURCHASE_FAILED);
MLIB_DEFINE_EVENT(EVENT_IAP_SHOP_ITEM_BOUGHT);
MLIB_DEFINE_EVENT(EVENT_IAP_SHOP_CLOSED);

class MIAPShop : public MEventDispatcher
{
public:
    static MIAPShop * sharedInstance();
    
    void show();
};

MLIB_NS_END

#endif /* defined(__legend__MIAPShop__) */
