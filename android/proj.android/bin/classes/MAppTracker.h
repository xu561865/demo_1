//
//  MAppTracker.h
//  legend
//
//  Created by Xu Jinyang on 9/11/13.
//
//

#ifndef __legend__MAppTracker__
#define __legend__MAppTracker__

#include <iostream>

class MAppTracker
{
public:
    MAppTracker() {}
    virtual ~MAppTracker() {}
    
    static MAppTracker * sharedTracker();
    
    // 删除 MAT
//    void startTracker(std::string advertiserId, std::string conversionKey);
//    void trackInstall();
//    void trackEvent(std::string eventName);
//    void trackRevenue(float amount, std::string currencyCode = "USD");
    
    std::string getDeviceId();
};

#define APP_TRACKER (MAppTracker::sharedTracker())

#endif /* defined(__legend__MAppTracker__) */
