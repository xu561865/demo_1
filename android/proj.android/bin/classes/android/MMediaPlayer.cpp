//
//  MMediaPlayer.cpp
//  legend
//
//  Created by adminstrator on 13-11-18.
//
//

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "MMediaPlayer.h"
#include "LConstants.h"

#include "MFileUtils.h"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"

MLIB_NS_BEGIN

MMediaPlayer::MMediaPlayer()
{
    strNameMark="";

    _callbackVideoEnded = [this] () {
		this->dispatchEvent(MEDIA_PLAYER_ENDED);
	};
}
MMediaPlayer::~MMediaPlayer()
{
    
}


MMediaPlayer * MMediaPlayer::SharedInstance()
{
    static MMediaPlayer inst;
    return &inst;
}



void MMediaPlayer::playVideo(std::string strName)
{
	strName = FILE_UTILS->getFullPath(strName);
	// @to be optimized
	auto pos = strName.find("/");
	if (pos != std::string::npos)
	{
		strName = strName.substr(pos+1);
	}

	cocos2d::JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lo/legend", "sharedInstance", "()Ljava/lang/Object;");
	jobject jobj;
	if (isHave)
	{
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
		M_INFO("正确获取到 jobj");
	}

	isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lo/legend","playVideo","(Ljava/lang/String;I)V");
	if (isHave)
	{
		M_INFO("exists----------");
		jstring str = minfo.env->NewStringUTF(strName.c_str());
		minfo.env->CallVoidMethod(jobj, minfo.methodID, str, &_callbackVideoEnded);
		minfo.env->DeleteLocalRef(str);
	}
	else
	{
		M_INFO("do not exist----------");
	}
	minfo.env->DeleteLocalRef(jobj);

}

void MMediaPlayer::continueIfPlaying()
{
	return;
}

void MMediaPlayer::stopVideo(  )
{
//	cocos2d::JniMethodInfo minfo;
//	bool isHave = cocos2d::JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lo/VideoDemo", "onVideoFinish", "()V");
//	if(!isHave)
//	{
//		M_INFO("jni:method not exist");
//	}
//	else
//	{
//		M_INFO("jni:ok");
//		minfo.env->
//	}
}


MLIB_NS_END
#endif
