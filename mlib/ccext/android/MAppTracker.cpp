//
//  MAppTracker.cpp
//  legend
//
//  Created by Xu Jinyang on 9/11/13.
//
//

#include "MAppTracker.h"

static MAppTracker *AppTrackerInstance = nullptr;

MAppTracker * MAppTracker::sharedTracker()
{
    if (AppTrackerInstance == NULL)
    {
        AppTrackerInstance = new MAppTracker();
    }
    return AppTrackerInstance;
}

std::string MAppTracker::getDeviceId()
{
    // @todo
    return "";
//    NSString *strDid = [ASIdentifierManager sharedManager].advertisingIdentifier.UUIDString;
//    return [strDid UTF8String];
}
