//
//  AndroidSdk.cpp
//  tsDemo
//
//  Created by xuyi on 3/24/16.
//
//

#include "AndroidSdk.h"

static AndroidSdk* s_SharedAndroidSdk = nullptr;

AndroidSdk::AndroidSdk()
{
    
}

AndroidSdk* AndroidSdk::SharedInstance()
{
    if(!s_SharedAndroidSdk)
    {
        s_SharedAndroidSdk = new AndroidSdk();
    }
    
    return s_SharedAndroidSdk;
}
