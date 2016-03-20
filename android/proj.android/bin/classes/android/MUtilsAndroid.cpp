//
//  MUtilsIOS.m
//  legend
//
//  Created by 张杰 on 14-2-13.
//
//

#include "MUtils.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#include "mlib_ccext.h"

MLIB_NS_BEGIN

void set_lock_screen(bool isLock)
{
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lo/legend", "sharedInstance", "()Ljava/lang/Object;");
	jobject jobj;
	if (isHave)
	{
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	}

	isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lo/legend","setLockScreen","(Z)V");
	jboolean result = false;
	if (isHave)
	{
		jboolean para = isLock;
		minfo.env->CallVoidMethod(jobj, minfo.methodID, para);
	} else {
		M_ERROR("do not exist----------");
	}
	minfo.env->DeleteLocalRef(jobj);
    return;
}


long long get_milliseconds_since_boot()
{
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lo/legend", "sharedInstance", "()Ljava/lang/Object;");
	jobject jobj;
	if (isHave)
	{
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	}

	isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lo/legend","getMillisecondsSinceBoot","()J");
	jlong time_since_boot = 0;
	if (isHave)
	{
		time_since_boot = minfo.env->CallLongMethod(jobj, minfo.methodID);
	} else {
		M_ERROR("do not exist----------");
	}
	minfo.env->DeleteLocalRef(jobj);
	return time_since_boot;
}


MLIB_NS_END
#endif
