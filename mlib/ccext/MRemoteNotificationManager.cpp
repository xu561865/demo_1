//
//  MRemoteNotificationManager.cpp
//  legend
//
//  Created by Minhao Zhang on 8/16/13.
//
//

#include "MRemoteNotificationManager.h"

MRemoteNotificationManager * MRemoteNotificationManager::sharedInstance()
{
    static MRemoteNotificationManager * inst = nullptr;
    if (inst == nullptr)
    {
        inst = new MRemoteNotificationManager;
    }
    return inst;
}
