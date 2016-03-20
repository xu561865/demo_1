//
//  MWebViewImplAndroid.cpp
//  legend
//
//  Created by xiaofuan on 14-3-5.
//
//

#include "MWebViewImplAndroid.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#include "MUtils.h"
#include "mlib_ccext.h"

MWebViewImpl* __createSystemWebView(MWebView* pWebView)
{
    return new MWebViewImplAndroid(pWebView);
}

MWebViewImplAndroid::MWebViewImplAndroid(MWebView* pWebView)
: MWebViewImpl(pWebView)
{
//    m_webViewIOS = [[WebView_IOS alloc] init];
//    m_Scale = mlib::get_content_scale_factor() / [[EAGLView sharedEGLView] contentScaleFactor];
}

MWebViewImplAndroid::~MWebViewImplAndroid()
{
//    [m_webViewIOS release];
//    m_webViewIOS = NULL;
}

void MWebViewImplAndroid::showWebView(const std::string url)
{
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lo/legend", "sharedInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if (isHave)
    {
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
        M_INFO("正确获取到 jobj, jobj = " << jobj);
    }

    isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lo/legend","openWebView","(Ljava/lang/String;)V");
    if (isHave)
    {
    	M_INFO("exists----------");
        jstring str = minfo.env->NewStringUTF(url.c_str());
        minfo.env->CallVoidMethod(jobj, minfo.methodID, str);
        minfo.env->DeleteLocalRef(str);
    }else{
    	M_INFO("do not exist----------");
    }
    minfo.env->DeleteLocalRef(jobj);
}

void MWebViewImplAndroid::openURL(std::string url)
{
//    [m_webViewIOS openURL:url.c_str()];
}

void MWebViewImplAndroid::removeWebView()
{
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lo/legend", "sharedInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if (isHave)
    {
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
        M_INFO("正确获取到 jobj");
    }

    isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lo/legend","removeWebView","()V");
    if (isHave)
    {
    	M_INFO("exists----------");
        minfo.env->CallVoidMethod(jobj, minfo.methodID);
    }else{
    	M_INFO("do not exist----------");
    }

    minfo.env->DeleteLocalRef(jobj);
}

void MWebViewImplAndroid::setPosition(const CCPoint& pos)
{
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lo/legend", "sharedInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if (isHave)
    {
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
//        M_INFO("正确获取到 jobj");
    }

    isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lo/legend","setPos","(II)V");
    if (isHave)
    {
//    	M_INFO("webview pos x = " << pos.x << ", pos y = " << pos.y);
    	jint jPosX = pos.x;
    	jint jPosY = pos.y;
        minfo.env->CallVoidMethod(jobj, minfo.methodID, jPosX, jPosY);
    }else{
//    	M_INFO("do not exist----------");
    }
    minfo.env->DeleteLocalRef(jobj);
}

void MWebViewImplAndroid::setContentSize(const CCSize& size)
{
	uint32_t width = size.width * mlib::get_content_scale_factor();
	uint32_t height = size.height * mlib::get_content_scale_factor();

    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lo/legend", "sharedInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if (isHave)
    {
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
        M_INFO("正确获取到 jobj");
    }

    isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lo/legend","setWebViewSize","(II)V");
    if (isHave)
    {
    	M_INFO("webview width = " << width << ", height = " << height);
    	jint jWidth = width;
    	jint jHeight = height;
        minfo.env->CallVoidMethod(jobj, minfo.methodID, jWidth, jHeight);
    }else{
    	M_INFO("do not exist----------");
    }
    minfo.env->DeleteLocalRef(jobj);
}

bool MWebViewImplAndroid::initWithSize(const cocos2d::CCSize &size)
{
    return true;
}

void MWebViewImplAndroid::setVisible(bool visible)
{
    
}

void MWebViewImplAndroid::setWebViewFrame()
{
    CCPoint pos = this->m_pWebView->getPosition();
    pos.y += m_pWebView->getContentSize().height;
    pos = this->m_pWebView->getParent()->convertToWorldSpace(pos);
    pos.x *= mlib::get_content_scale_factor();
    pos.y *= mlib::get_content_scale_factor();

    this->setPosition(pos);
}

void MWebViewImplAndroid::visit()
{
    if (this->m_pWebView)
    {
        setWebViewFrame();
    }
}
#endif
