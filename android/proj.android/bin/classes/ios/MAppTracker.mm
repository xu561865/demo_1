//
//  MAppTracker.mm
//  legend
//
//  Created by Xu Jinyang on 9/11/13.
//
//

#include "MAppTracker.h"
#import "AdSupport/ASIdentifierManager.h"

static MAppTracker *AppTrackerInstance = nullptr;

MAppTracker * MAppTracker::sharedTracker()
{
    if (AppTrackerInstance == NULL)
    {
        AppTrackerInstance = new MAppTracker();
    }
    return AppTrackerInstance;
}

//void MAppTracker::startTracker(std::string advertiserId, std::string conversionKey)
//{
//    NSString *aId = [NSString stringWithUTF8String:advertiserId.c_str()];
//    NSString *cKey = [NSString stringWithUTF8String:conversionKey.c_str()];
//    [[MobileAppTracker sharedManager] startTrackerWithMATAdvertiserId:aId MATConversionKey:cKey];
//}
//
//void MAppTracker::trackInstall()
//{
//    [[MobileAppTracker sharedManager] trackInstall];
//}
//
//void MAppTracker::trackEvent(std::string eventName)
//{
//    NSString *eName = [NSString stringWithUTF8String:eventName.c_str()];
//    [[MobileAppTracker sharedManager] trackActionForEventIdOrName:eName eventIsId:NO];
//}
//
//void MAppTracker::trackRevenue(float amount, std::string currencyCode)
//{
//    NSString *cCode = [NSString stringWithUTF8String:currencyCode.c_str()];
//    [[MobileAppTracker sharedManager] trackActionForEventIdOrName:@"purchase" eventIsId:NO revenueAmount:amount currencyCode:cCode];
//}

std::string MAppTracker::getDeviceId()
{
    NSString *strDid = [ASIdentifierManager sharedManager].advertisingIdentifier.UUIDString;
    return [strDid UTF8String];
}
