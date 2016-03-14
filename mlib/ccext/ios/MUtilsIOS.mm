//
//  MUtilsIOS.m
//  legend
//
//  Created by 张杰 on 14-2-13.
//
//

#include "MUtils.h"

MLIB_NS_BEGIN


long long get_milliseconds_since_boot()
{
    auto boot_time = [[NSProcessInfo processInfo] systemUptime];
    return (long long)(boot_time * 1e3);
}


void set_lock_screen(bool isLock)
{
    [UIApplication sharedApplication].idleTimerDisabled = !isLock;
    return;
}

MLIB_NS_END