//
//  MRemoteNotificationManager.h
//  legend
//
//  Created by Minhao Zhang on 8/16/13.
//
//

#ifndef __legend__MRemoteNotificationManager__
#define __legend__MRemoteNotificationManager__

#include <string>
#include "mlib_event.h"

MLIB_DEFINE_EVENT(EVENT_SUBSCRIBED_TO_NOTIF_CHANNEL);
MLIB_DEFINE_EVENT(EVENT_FAILED_TO_SUBSCRIBE_TO_NOTIF_CHANNEL);
MLIB_DEFINE_EVENT(EVENT_UNSUBSCRIBED_FROM_NOTIF_CHANNEL);
MLIB_DEFINE_EVENT(EVENT_FAILED_TO_UNSUBSCRIBE_TO_NOTIF_CHANNEL);

class MRemoteNotificationManager : public mlib::MEventDispatcher
{
public:
    static MRemoteNotificationManager * sharedInstance();
    
    void subscribeToChannelInBackground(const std::string& channelName);
    void unsubscribeFromChannelInBackground(const std::string& channelName);
};

#endif /* defined(__legend__MRemoteNotificationManager__) */
