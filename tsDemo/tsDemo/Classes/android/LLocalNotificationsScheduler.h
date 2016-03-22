//
//  LLocalNotificationsScheduler.h
//  legend
//
//  Created by lizhaocheng on 13-10-15.
//
//

#ifndef __legend__LLocalNotificationsScheduler__
#define __legend__LLocalNotificationsScheduler__

#include <iostream>
#include <string>

class LLocalNotificationsScheduler
{
public:
    static LLocalNotificationsScheduler * sharedInstance();
public:
    void cancelAllNotifications();

    void scheduleNotification(uint32_t timestamp, std::string text);
    void scheduleNotificationWithTimeIntervalSinceNow(uint32_t timeInterval, std::string text);
};

#define LOCAL_NOTIF (LLocalNotificationsScheduler::sharedInstance())

#endif /* defined(__legend__LLocalNotificationsScheduler__) */
