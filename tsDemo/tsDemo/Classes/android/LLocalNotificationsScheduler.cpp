//
//  LLocalNotificationsScheduler.cpp
//  legend
//
//  Created by lizhaocheng on 13-10-15.
//
//

#include "LLocalNotificationsScheduler.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#include "MUtils.h"
#include "mlib_ccext.h"

LLocalNotificationsScheduler * LLocalNotificationsScheduler::sharedInstance()
{
    static LLocalNotificationsScheduler inst;
    return &inst;
}

void LLocalNotificationsScheduler::scheduleNotification(uint32_t timestamp, std::string text)
{
}

void LLocalNotificationsScheduler::cancelAllNotifications()
{
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lo/legend", "sharedInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if (isHave)
    {
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
//        M_INFO("正确获取到 jobj, jobj = " << jobj);
    }

    isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lo/legend","cancelAllNotifications","()V");
    if (isHave)
    {
//    	M_INFO("cancelAllNotifications----------");
        minfo.env->CallVoidMethod(jobj, minfo.methodID);
    }else{
//    	M_INFO("do not exist----------");
    }
}

void LLocalNotificationsScheduler::scheduleNotificationWithTimeIntervalSinceNow(uint32_t timeInterval, std::string text)
{
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lo/legend", "sharedInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if (isHave)
    {
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
//        M_INFO("正确获取到 jobj, jobj = " << jobj);
    }

    isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lo/legend","pushLocalNotification","(ILjava/lang/String;)V");
    if (isHave)
    {
//    	M_INFO("push local notification-- " << text);
    	jint time = timeInterval;
    	jstring str = minfo.env->NewStringUTF(text.c_str());
        minfo.env->CallVoidMethod(jobj, minfo.methodID, time, str);
    }else{
//    	M_INFO("do not exist----------");
    }
}
#endif

