#include "LUtils.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#include "MUtils.h"
#include "mlib_ccext.h"
#include "LAlertView.h"
#include "IntroScene.h"
#include "LMessageCenter.h"
#include "LMemory.h"

#define TAG_SKIP_BUTTON 1201

#define UPDATE_STATUS_TYPE_DOWNLOAD  1
#define UPDATE_STATUS_TYPE_VERIFY  2
#define UPDATE_STATUS_TYPE_UNZIP  3

#define TEXT_ID_STATE_IDLE  1
#define TEXT_ID_STATE_FETCHING_URL  2
#define TEXT_ID_STATE_CONNECTING  3
#define TEXT_ID_STATE_DOWNLOADING  4
#define TEXT_ID_STATE_COMPLETED  5
#define TEXT_ID_STATE_PAUSED_NETWORK_UNAVAILABLE  6
#define TEXT_ID_STATE_PAUSED_BY_REQUEST  7
#define TEXT_ID_STATE_PAUSED_WIFI_DISABLED_NEED_CELLULAR_PERMISSION  8
#define TEXT_ID_STATE_PAUSED_NEED_CELLULAR_PERMISSION  9
#define TEXT_ID_STATE_PAUSED_WIFI_DISABLED  10
#define TEXT_ID_STATE_PAUSED_NEED_WIFI  11
#define TEXT_ID_STATE_PAUSED_ROAMING  12
#define TEXT_ID_STATE_PAUSED_NETWORK_SETUP_FAILURE  13
#define TEXT_ID_STATE_PAUSED_SDCARD_UNAVAILABLE  14
#define TEXT_ID_STATE_FAILED_UNLICENSED  15
#define TEXT_ID_STATE_FAILED_FETCHING_URL  16
#define TEXT_ID_STATE_FAILED_SDCARD_FULL  17
#define TEXT_ID_STATE_FAILED_CANCELED  18
#define TEXT_ID_STATE_FAILED  19

#define TEXT_ID_VERIFY_DOING  1
#define TEXT_ID_VERIFY_SUCCESS  2
#define TEXT_ID_VERIFY_FAIL  3

#define TEXT_ID_UNZIP_DOING  1
#define TEXT_ID_UNZIP_SUCCESS  2
#define TEXT_ID_UNZIP_FAIL  3


NS_LUTILS_BEGIN

void showLoginError();

extern "C"
{
	void Java_org_cocos2dx_lo_legend_funcOnScreenChanged()
	{
		mlib::initialize_resolution();
	}

	void Java_org_cocos2dx_lo_legend_funcLoginResult(JNIEnv *env, jobject thiz, jboolean isOK, jstring uid, jstring token, jboolean hasBind, jstring pltfmToken)
	{
		if (isOK)
		{
			M_DEBUG("Login OK");

			const char *str = env->GetStringUTFChars(uid, nullptr);
			M_ASSERT(str != nullptr);
			std::string strUid = str;
			env->ReleaseStringUTFChars(uid, str);

			str = env->GetStringUTFChars(token, nullptr);
			M_ASSERT(str != nullptr);
			std::string strToken = str;
			env->ReleaseStringUTFChars(token, str);

			str = env->GetStringUTFChars(pltfmToken, nullptr);
			M_ASSERT(str != nullptr);
			std::string strPlatformToken = str;
			env->ReleaseStringUTFChars(pltfmToken, str);

			bool isOasPlatform = strPlatformToken.empty();

			M_DEBUG("token " << token << ", uid " << uid << ", hasbind " << hasBind << ", platform token " << strPlatformToken);
			//临时修正，保证facebook登陆一次后可以自动登陆
			if (hasBind && !isOasPlatform)
			{
				MEM_INFO->hasBindThirdParty(false);
				MEM_INFO->isOasPlatform(false);
			}

			MEM_INFO->saveBasic();
			M_DEBUG("has bind = " << MEM_INFO->hasBindThirdParty() << ", is oas platfrom " << MEM_INFO->isOasPlatform());

			send_oas_login_msg(strToken, strUid);
		}
		else
		{
			if (MEM_INFO->accountId() == 0)
			{
				showLoginError();
			}
		}
	}

	void Java_org_cocos2dx_lo_legend_funcPayResult(JNIEnv *env, jobject thiz, jboolean isOK)
	{
		if (isOK)
		{
			M_DEBUG("Pay OK");

			auto req = MSG_CENTER->loadProfile(MEM_INFO->hostProfile()->userId());
			req->onSuccess([](mlib::MHttpRequest *r) {
						LAlertView::ShowAlertTextWithHighestPriority(MLIB_LOCALIZED_STRING("购买礼包成功！"));
						MEM_INFO->dispatchEvent(EVENT_IAP_COMPLETED);
					});
			req->send();
		}
		else
		{
			M_DEBUG("Pay FAILED");

			LAlertView::ShowAlertTextWithHighestPriority(MLIB_LOCALIZED_STRING("购买礼包失败，请重试！"));
		}
	}

	JNIEXPORT void JNICALL Java_org_cocos2dx_lo_DownloadHelper_update_1status(JNIEnv *env, jobject thiz, jint type, jint text_id)
	{
		auto evt = mlib::MEvent(LMemory::EVENT_OBB_UPDATE_STATUS);
		int32_t n_type = type;
    	int32_t n_text_id = text_id;
    	evt.buffer() << n_type;
    	evt.buffer() << n_text_id;
		MEM_INFO->dispatchEvent(evt);
	}

	JNIEXPORT void JNICALL Java_org_cocos2dx_lo_DownloadHelper_update_1progress(JNIEnv *env, jobject thiz, jfloat percent)
    {
    	auto evt = mlib::MEvent(LMemory::EVENT_OBB_UPDATE_PROGRESS);
    	uint32_t progress = percent * 100;
    	evt.buffer() << progress;
    	MEM_INFO->dispatchEvent(evt);
    }

	JNIEXPORT void JNICALL Java_org_cocos2dx_lo_DownloadHelper_start_1game()
    {
        CCDirector::sharedDirector()->replaceScene(IntroScene::scene());
    }
} // end of extern "C"

void copy_referral_code(std::string code, std::string serverName)
{
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lo/legend", "sharedInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if (isHave)
    {
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    }

    isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lo/legend","setClipboardTxt","(Ljava/lang/String;)V");
    if (isHave)
    {
    	auto text = SSTR(serverName << " " << MLIB_LOCALIZED_STRING("Referral Code") << ": " << code << MLIB_LOCALIZED_STRING(get_invit_friend_message_random()));
    	jstring str = minfo.env->NewStringUTF(text.c_str());
    	M_INFO("code --" << text);
        minfo.env->CallVoidMethod(jobj, minfo.methodID, str);
    }else{
    	M_INFO("do not exist----------");
    }

    std::string strText = MLIB_LOCALIZED_STRING("复制成功。是否去公布你的邀请码？");
#ifdef SDK_PP
	strText = "复制成功, 是否去PP助手评论页面发布你的邀请码?";
#endif

	LAlertView::ShowAlert(strText, MLIB_LOCALIZED_STRING("Copy Successfully"), ALERT_CANCEL, ALERT_OK, [] (std::string tag) {
				if (tag == ALERT_BUTTON2)
				{
					JniMethodInfo minfo;
					bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lo/legend", "sharedInstance", "()Ljava/lang/Object;");
					jobject jobj;
					if (isHave)
					{
						jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
					}

					isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lo/legend","openUrl","(Ljava/lang/String;)V");
					if (isHave)
					{
						jstring str = minfo.env->NewStringUTF(APP_MANAGER->appStoreUrl().c_str());
						minfo.env->CallVoidMethod(jobj, minfo.methodID, str);
					} else {
						//    	M_INFO("do not exist----------");
					}
				}
			});
}

std::string get_update_text(std::string version)
{
	std::string str;

#ifdef SDK_PP
	str = SSTR("游戏有新的版本(" << version << "), 请重进游戏更新.");
#else
	str = MLIB_LOCALIZED_STRING_FORMAT_P1("游戏有新的版本(%s), 请更新.", version.c_str());
#endif

	return str;
}

void update_client()
{
#ifdef SDK_PP
    exit(0);
#else
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lo/legend", "sharedInstance", "()Ljava/lang/Object;");
	jobject jobj;
	if (isHave)
	{
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	}

	isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lo/legend","openUrl","(Ljava/lang/String;)V");
	if (isHave)
	{
		jstring str = minfo.env->NewStringUTF(APP_MANAGER->appStoreUrl().c_str());
		minfo.env->CallVoidMethod(jobj, minfo.methodID, str);
	} else {
		//    	M_INFO("do not exist----------");
	}

    CCDirector::sharedDirector()->replaceScene(IntroScene::scene());
#endif
}

void pause_play_music()
{
//    MPMusicPlayerController *player = [MPMusicPlayerController iPodMusicPlayer];
//    [player pause];
//
//    MPMusicPlayerController *myPlayer = [MPMusicPlayerController applicationMusicPlayer];
//    [myPlayer play];

}

void stop_my_music()
{
//    MPMusicPlayerController *myPlayer = [MPMusicPlayerController applicationMusicPlayer];
//    [myPlayer stop];
}

void stop_iPod_music()
{
//    MPMusicPlayerController *myPlayer = [MPMusicPlayerController iPodMusicPlayer];
//    [myPlayer stop];
}

void pause_my_music()
{
//    MPMusicPlayerController *myPlayer = [MPMusicPlayerController applicationMusicPlayer];
//    [myPlayer pause];
}

void play_my_music()
{
//    MPMusicPlayerController *myPlayer = [MPMusicPlayerController applicationMusicPlayer];
//    [myPlayer play];
}

void pause_iPod_music()
{
//    MPMusicPlayerController *player = [MPMusicPlayerController iPodMusicPlayer];
//    [player pause];
}

void play_iPod_music()
{
//    MPMusicPlayerController *player = [MPMusicPlayerController iPodMusicPlayer];
//    [player pause];
}

void add_skip_button()
{
//    UIColor *color = [UIColor colorWithRed:217/255.f green:205/255.f blue:170/255.f alpha:1];
//    float scaleFactor = mlib::get_content_scale_factor() / [[EAGLView sharedEGLView] contentScaleFactor];
//    float height = 30 * scaleFactor;
//    float paddingLeft = 20 * scaleFactor;
//    float paddingBottom = 20 * scaleFactor;
//    float underlineHeight = 2 * scaleFactor;
//
//    CGRect rect = [[EAGLView sharedEGLView] frame];
//    rect.origin.y = rect.size.height - height - paddingBottom;
//    rect.size.height = height;
//    UIView *view = [[[UIView alloc] initWithFrame:rect] autorelease];
//    view.backgroundColor = [UIColor clearColor];
//
//    rect.origin.y = 0;
//    UILabel *lbl = [[[UILabel alloc] initWithFrame:rect] autorelease];
//    lbl.font = [UIFont fontWithName:@"Copperplate-Bold" size:28 * scaleFactor];
//    lbl.text = [NSString stringWithUTF8String:SSTR(MLIB_LOCALIZED_STRING("跳过") << " >>").c_str()];
//    lbl.textAlignment = NSTextAlignmentRight;
//    lbl.textColor = color;
//    lbl.backgroundColor = [UIColor clearColor];
//    [lbl sizeToFit];
//    rect.origin.x = rect.size.width - lbl.bounds.size.width - paddingLeft;
//    rect.size = lbl.bounds.size;
//    lbl.frame = rect;
//
//    UIView *underline = [[[UIView alloc] init] autorelease];
//    underline.backgroundColor = color;
//
//    rect.origin.y = height - underlineHeight;
//    rect.size.height = underlineHeight;
//    underline.frame = rect;
//
//    [view addSubview:lbl];
//    [view addSubview:underline];
//    [view setTag:TAG_SKIP_BUTTON];
//    [[EAGLView sharedEGLView] addSubview:view];
}

void remove_skip_button()
{
//    [[[EAGLView sharedEGLView] viewWithTag:TAG_SKIP_BUTTON] removeFromSuperview];
}

void show_login_third_party()
{
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lo/legend", "sharedInstance", "()Ljava/lang/Object;");
	jobject jobj;
	if (isHave)
	{
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	}

	isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lo/legend","switchUser","()V");
	if (isHave)
	{
		minfo.env->CallVoidMethod(jobj, minfo.methodID);
	} else {
		M_ERROR("do not exist----------");
	}
}

void show_center_third_party()
{
//#ifdef SDK_PP
//    [[PPAppPlatformKit sharedInstance] showCenter];
//#endif
}

void track_level_for_adjust(uint32_t level)
{
	std::string token;
	if (level >= 25)
	{
		token = "level25";
	}
	else if (level >= 20)
	{
		token = "level20";
	}
	else if (level >= 10)
	{
		token = "level10";
	}
	else if (level >= 3)
	{
		token = "level3";
	}
	else
	{
		token = "level1";
	}

	if (token != "")
	{
		JniMethodInfo minfo;
		bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lo/legend", "sharedInstance", "()Ljava/lang/Object;");
		jobject jobj;
		if (isHave)
		{
			jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
		}

		isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lo/legend","AdjustTrackEvent","(Ljava/lang/String;)V");
		if (isHave)
		{
			jstring str = minfo.env->NewStringUTF(token.c_str());
			minfo.env->CallVoidMethod(jobj, minfo.methodID, str);
		} else {
			M_ERROR("do not exist----------");
		}
	}
}

void track_revenue_for_adjust(float amount)
{
//#ifdef LOBR
//    [Adjust trackRevenue:amount*100];
//#elif defined LOTR
//    [Adjust trackRevenue:amount*100];
//#endif
}

void oas_deposit(LIAPProductSpec *product)
{
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lo/legend", "sharedInstance", "()Ljava/lang/Object;");
	jobject jobj;
	if (isHave)
	{
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	}

	isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lo/legend","GoogleBillPay","(Ljava/lang/String;D)V");
	if (isHave)
	{
		jstring str = minfo.env->NewStringUTF(product->productId.c_str());
		jdouble revenue = product->cost;
		minfo.env->CallVoidMethod(jobj, minfo.methodID, str, revenue);
	} else {
		M_ERROR("do not exist----------");
	}
}

void oas_login(uint32_t serverId, std::string serverName, std::string username, uint32_t userId)
{
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lo/legend", "sharedInstance", "()Ljava/lang/Object;");
	jobject jobj;
	if (isHave)
	{
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	}

	isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lo/legend","SetUserInfo","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
	if (isHave)
	{
		jstring strServerId = minfo.env->NewStringUTF(SSTR(serverId).c_str());
		jstring strServerName = minfo.env->NewStringUTF(serverName.c_str());
		jstring strUsername = minfo.env->NewStringUTF(username.c_str());
		jstring strUserId = minfo.env->NewStringUTF(SSTR(userId).c_str());
		minfo.env->CallVoidMethod(jobj, minfo.methodID, strServerId, strServerName, strUsername, strUserId);
	} else {
		M_ERROR("do not exist----------");
	}
}

bool has_oas_login()
{
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lo/legend", "sharedInstance", "()Ljava/lang/Object;");
	jobject jobj;
	if (isHave)
	{
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	}

	isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lo/legend","hasOasLogin","()Z");
	jboolean result = false;
	if (isHave)
	{
		result = minfo.env->CallBooleanMethod(jobj, minfo.methodID);
	} else {
		M_ERROR("do not exist----------");
	}

	return result;
}

bool has_bind_third_party()
{
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lo/legend", "sharedInstance", "()Ljava/lang/Object;");
	jobject jobj;
	if (isHave)
	{
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	}

	isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lo/legend","hasBind","()Z");
	jboolean hasBind = false;
	if (isHave)
	{
		hasBind = minfo.env->CallBooleanMethod(jobj, minfo.methodID);
	} else {
		M_ERROR("do not exist----------");
	}

	return hasBind;
}

void auto_login_oas()
{
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lo/legend", "sharedInstance", "()Ljava/lang/Object;");
	jobject jobj;
	if (isHave)
	{
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	}

	isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lo/legend","loginGame","(Z)V");
	if (isHave)
	{
		jboolean isAutoLogin = true;
		if (MEM_INFO->hasBindThirdParty() && !MEM_INFO->isOasPlatform())
		{
			isAutoLogin = false;
		}
		minfo.env->CallVoidMethod(jobj, minfo.methodID, isAutoLogin);
	} else {
		M_ERROR("autoLogin do not exist----------");
	}
}

void send_oas_login_msg(std::string token, std::string uid)
{
	LHttpRequest * req = MSG_CENTER->oasLogin(token, uid);
	req->onSuccess([](mlib::MHttpRequest *r)
			{
				if (MEM_INFO->shouldWaitingForThirdPartyLogin())
				{
					M_DEBUG("continue login after oas login");
					MEM_INFO->dispatchEvent(EVENT_THIRD_PARTY_LOGGED_IN);
					MEM_INFO->shouldWaitingForThirdPartyLogin(false);
				}
				else
				{
					M_DEBUG("replace with introscene after oas login");
					CCDirector::sharedDirector()->replaceScene(IntroScene::scene());
				}
			});
	req->onError([req](mlib::MHttpRequest *r) {
				showLoginError();
				req->response()->isErrorHandled(true);
			});
	req->isBackground(true);
	req->send();
}

void login_game_after_has_login_oas()
{
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lo/legend", "sharedInstance", "()Ljava/lang/Object;");
	jobject jobj;
	if (isHave)
	{
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	}

	isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lo/legend","getUserInfo","()Ljava/lang/String;");
	if (isHave)
	{
		jstring uidToken = (jstring)minfo.env->CallObjectMethod(jobj, minfo.methodID);
		const char *str = minfo.env->GetStringUTFChars(uidToken, nullptr);
		M_ASSERT(str != nullptr);
		std::string strUidToken = str;
		minfo.env->ReleaseStringUTFChars(uidToken, str);

		auto vec = mlib::split_string(strUidToken, ",");
		M_DEBUG("login game after has login oas, token " << vec[1] << ", uid " << vec[0]);
		send_oas_login_msg(vec[1], vec[0]);
	} else {
		M_ERROR("getUserInfo do not exist----------");
	}
}

void showLoginError()
{
    LAlertView::ShowAlertTextWithHighestPriority(MLIB_LOCALIZED_STRING("登录验证失败, 请重试!"), [](std::string btnTag)
    {
    	MEM_INFO->accountId(0);
    	MEM_INFO->saveBasic();
    	M_DEBUG("Login error account id " << MEM_INFO->accountId());
    	CCDirector::sharedDirector()->replaceScene(IntroScene::scene());
    });
}


//============================== obb check [start] ==============================
bool is_need_check_obb()
{
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lo/legend", "sharedInstance", "()Ljava/lang/Object;");
	jobject jobj;
	if (isHave)
	{
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	}

	isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lo/legend","isNeedCheckOBB","()Z");
	jboolean result = false;
	if (isHave)
	{
		result = minfo.env->CallBooleanMethod(jobj, minfo.methodID);
	} else {
		M_ERROR("do not exist----------");
	}

	return result;
}

void obb_check()
{
	JniMethodInfo minfo;
	bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lo/legend", "sharedInstance", "()Ljava/lang/Object;");
	jobject jobj;
	if (isHave)
	{
		jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	}

	isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lo/legend","startOBBCheck","()V");
	if (isHave)
	{
		minfo.env->CallVoidMethod(jobj, minfo.methodID);
	}
	else
	{

	}
}

std::string get_obb_status_text(int32_t type, int32_t text_id)
{
	if(type == UPDATE_STATUS_TYPE_DOWNLOAD)
	{
		switch(text_id)
		{
		case TEXT_ID_STATE_IDLE: return MLIB_LOCALIZED_STRING("等待下载扩展文件");//"Waiting for download to start"
		case TEXT_ID_STATE_FETCHING_URL: return MLIB_LOCALIZED_STRING("正在查找需要下载的扩展文件");//"Looking for resources to download"
		case TEXT_ID_STATE_CONNECTING: return MLIB_LOCALIZED_STRING("正在连接扩展文件的下载服务器");//"Connecting to the download server"
		case TEXT_ID_STATE_DOWNLOADING: return MLIB_LOCALIZED_STRING("正在下载扩展文件");//"Downloading resources"
		case TEXT_ID_STATE_COMPLETED: return MLIB_LOCALIZED_STRING("扩展文件下载完成");//"Download finished"
		case TEXT_ID_STATE_PAUSED_NETWORK_UNAVAILABLE: return MLIB_LOCALIZED_STRING("暂停下载：没有可用的网络");//"Download paused because no network is available"
		case TEXT_ID_STATE_PAUSED_BY_REQUEST: return MLIB_LOCALIZED_STRING("暂停下载");//"Download paused"
		case TEXT_ID_STATE_PAUSED_WIFI_DISABLED_NEED_CELLULAR_PERMISSION:
		case TEXT_ID_STATE_PAUSED_WIFI_DISABLED: return MLIB_LOCALIZED_STRING("暂停下载：WIFI禁用");//"Download paused because wifi is disabled"
		case TEXT_ID_STATE_PAUSED_NEED_CELLULAR_PERMISSION:
		case TEXT_ID_STATE_PAUSED_NEED_WIFI:return MLIB_LOCALIZED_STRING("暂停下载：WIFI不可用");//"Download paused because wifi is unavailable"
		case TEXT_ID_STATE_PAUSED_ROAMING: return MLIB_LOCALIZED_STRING("暂停下载：您正在漫游");//"Download paused because you are roaming"
		case TEXT_ID_STATE_PAUSED_NETWORK_SETUP_FAILURE: return MLIB_LOCALIZED_STRING("暂停下载：网络设置失败，请检查您的网络");//"Download paused. Test a website in browser"
		case TEXT_ID_STATE_PAUSED_SDCARD_UNAVAILABLE: return MLIB_LOCALIZED_STRING("暂停下载：存储卡不可用");//"Download paused because the external storage is unavailable"
		case TEXT_ID_STATE_FAILED_UNLICENSED: return MLIB_LOCALIZED_STRING("下载失败：请购买游戏");//"Download failed because you may not have purchased this app"
		case TEXT_ID_STATE_FAILED_FETCHING_URL: return MLIB_LOCALIZED_STRING("下载失败：扩展文件没有找到");//"Download failed because the resources could not be found"
		case TEXT_ID_STATE_FAILED_SDCARD_FULL: return MLIB_LOCALIZED_STRING("下载失败：存储卡空间不足");//"Download failed because the external storage is full"
		case TEXT_ID_STATE_FAILED_CANCELED: return MLIB_LOCALIZED_STRING("取消下载");//"Download cancelled"
		case TEXT_ID_STATE_FAILED: return MLIB_LOCALIZED_STRING("下载失败");//"Download failed"
		}
	}
	else if(type == UPDATE_STATUS_TYPE_VERIFY)
	{
		switch(text_id)
		{
		case TEXT_ID_VERIFY_DOING: return MLIB_LOCALIZED_STRING("正在校验扩展文件");
		case TEXT_ID_VERIFY_SUCCESS: return MLIB_LOCALIZED_STRING("校验扩展文件成功");
		case TEXT_ID_VERIFY_FAIL: return MLIB_LOCALIZED_STRING("校验扩展文件失败");
		}
	}
	else if(type == UPDATE_STATUS_TYPE_UNZIP)
	{
		switch(text_id)
		{
		case TEXT_ID_UNZIP_DOING: return MLIB_LOCALIZED_STRING("正在解压扩展文件");
		case TEXT_ID_UNZIP_SUCCESS: return MLIB_LOCALIZED_STRING("解压扩展文件成功");
		case TEXT_ID_UNZIP_FAIL: return MLIB_LOCALIZED_STRING("解压扩展文件失败");
		}
	}

	return "";
}
//============================== obb check [end] ==============================


NS_LUTILS_END
#endif
