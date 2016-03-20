#include "MRemoteNotificationManager.h"
//#include <Parse/Parse.h>

void MRemoteNotificationManager::subscribeToChannelInBackground(const std::string &channelName)
{
//    [PFPush subscribeToChannelInBackground:[NSString stringWithUTF8String:channelName.c_str()]
//                                     block:^(BOOL succeeded, NSError *error) {
//                                         if (succeeded)
//                                         {
//                                             mlib::MEvent event(EVENT_SUBSCRIBED_TO_NOTIF_CHANNEL);
//                                             event.buffer() << channelName;
//                                             this->dispatchEvent(event);
//                                         }
//                                         else
//                                         {
//                                             NSLog(@"Failed to subscribe to parse channel! %@", error);
//                                             mlib::MEvent event(EVENT_FAILED_TO_SUBSCRIBE_TO_NOTIF_CHANNEL);
//                                             event.buffer() << channelName;
//                                             this->dispatchEvent(event);
//                                         }
//                                     }];
}

void MRemoteNotificationManager::unsubscribeFromChannelInBackground(const std::string &channelName)
{
//    [PFPush unsubscribeFromChannelInBackground:[NSString stringWithUTF8String:channelName.c_str()]
//                                         block:^(BOOL succeeded, NSError *error) {
//                                             if (succeeded)
//                                             {
//                                                 mlib::MEvent event(EVENT_UNSUBSCRIBED_FROM_NOTIF_CHANNEL);
//                                                 event.buffer() << channelName;
//                                                 this->dispatchEvent(event);
//                                             }
//                                             else
//                                             {
//                                                 NSLog(@"Failed to unsubscribe from parse channel! %@", error);
//                                                 mlib::MEvent event(EVENT_FAILED_TO_UNSUBSCRIBE_TO_NOTIF_CHANNEL);
//                                                 event.buffer() << channelName;
//                                                 this->dispatchEvent(event);
//                                             }
//                                         }];
}