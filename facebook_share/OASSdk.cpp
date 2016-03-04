//
//  OASSdk.cpp
//  Slots
//
//  Created by bbf on 15-1-8.
//
//

#include "../OASSdk.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>
#endif

NS_CC_LEVER_EXT_BEGIN;

jobject createJavaMapObject(cocos2d::ValueMap params)
{
    JNIEnv* env = cocos2d::JniHelper::getEnv();
    jclass class_Map = env->FindClass("java/util/HashMap");
    jmethodID construct_method = env->GetMethodID(class_Map, "<init>","()V");
    jobject obj_Map = env->NewObject(class_Map, construct_method, "");
    jmethodID add_method= env->GetMethodID(class_Map,"put","(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    for (auto it = params.cbegin(); it != params.cend(); ++it)
    {
        jstring first = env->NewStringUTF(it->first.c_str());
        jstring second = env->NewStringUTF(it->second.asString().c_str());
        env->CallObjectMethod(obj_Map, add_method, first, second);
        env->DeleteLocalRef(first);
        env->DeleteLocalRef(second);
    }
    env->DeleteLocalRef(class_Map);
    return obj_Map;
    
}

cocos2d::LuaValueDict getFrinedsDict(jobject jobj) //暂时写的，并不通用，只为取friend
{
    JNIEnv* env = cocos2d::JniHelper::getEnv();
    jclass class_Map = env->FindClass("java/util/HashMap");
//    jmethodID construct_method = env->GetMethodID(class_Map, "<init>","()V");
//    jobject obj_Map = env->NewObject(class_Map, construct_method, "");
    jmethodID get_method= env->GetMethodID(class_Map,"get","(Ljava/lang/Object;)Ljava/lang/Object;");
    
//    jmethodID get_Size = env->GetMethodID(class_Map, "size", "()I");
//    int jSize = env->CallIntMethod(jobj , get_Size);
    
    jstring id_key = env->NewStringUTF("id");
    jstring name_key = env->NewStringUTF("name");
    jstring picture_key = env->NewStringUTF("picture");
    
    jstring jid = (jstring)env->CallObjectMethod(jobj, get_method, id_key);
    jstring jname = (jstring)env->CallObjectMethod(jobj, get_method, name_key);
    jstring jpicture = (jstring)env->CallObjectMethod(jobj, get_method, picture_key);
    
    LuaValueDict dict;
    std::string id = JniHelper::jstring2string(jid);
    std::string name = JniHelper::jstring2string(jname);
    std::string picture = JniHelper::jstring2string(jpicture);
    dict.insert(dict.end(), LuaValueDict::value_type("id", LuaValue::stringValue(id)));
    dict.insert(dict.end(), LuaValueDict::value_type("name", LuaValue::stringValue(name)));
    dict.insert(dict.end(), LuaValueDict::value_type("picture", LuaValue::stringValue(picture)));
    
    env->DeleteLocalRef(id_key);
    env->DeleteLocalRef(name_key);
    env->DeleteLocalRef(picture_key);
    env->DeleteLocalRef(jid);
    env->DeleteLocalRef(jname);
    env->DeleteLocalRef(jpicture);
    env->DeleteLocalRef(class_Map);
    
    return dict;
    
}

static OASSdk* s_sharedOASSdkInstance = NULL;

OASSdk::OASSdk()
{
    
}

OASSdk::~OASSdk()
{
    
}

OASSdk* OASSdk::getInstance()
{
    if (s_sharedOASSdkInstance == NULL)
    {
        s_sharedOASSdkInstance = new OASSdk();
    }
    
    return s_sharedOASSdkInstance;
}
void OASSdk::autoLogin()
{
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lib/Cocos2dxActivity", "getInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if (isHave)
    {
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    }
    
    isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lib/Cocos2dxActivity","autoLogin","()V");
    if (isHave)
    {
        minfo.env->CallVoidMethod(jobj, minfo.methodID);
        minfo.env->DeleteLocalRef(jobj);
        minfo.env->DeleteLocalRef(minfo.classID);
    } else
    {
        CCLOG("Failed to get the function autoLogin");
    }
}

void OASSdk::share(cocos2d::ValueMap params)
{
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lib/Cocos2dxActivity", "getInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if (isHave)
    {
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    }
    
    isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lib/Cocos2dxActivity","share", "(Ljava/util/HashMap;)V");
    if (isHave)
    {
        CCLOG("Sucess to get the function share");
        jobject map = createJavaMapObject(params);
        minfo.env->CallVoidMethod(jobj, minfo.methodID, map);
        minfo.env->DeleteLocalRef(jobj);
        minfo.env->DeleteLocalRef(map);
        minfo.env->DeleteLocalRef(minfo.classID);
    } else
    {
        CCLOG("Failed to get the function share");
    }
}
void OASSdk::showMenu(bool isShow, int postion)
{
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lib/Cocos2dxActivity", "getInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if (isHave)
    {
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    }
    
    isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lib/Cocos2dxActivity","showMenu", "(ZI)V");
    if (isHave)
    {
        jboolean s = isShow;
        jint p = postion;
        minfo.env->CallVoidMethod(jobj, minfo.methodID, s, p);
        minfo.env->DeleteLocalRef(jobj);
        minfo.env->DeleteLocalRef(minfo.classID);
    } else
    {
        CCLOG("Failed to get the function showMenu");
    }
}


void OASSdk::pay(const char* identifier, double amountInCents, const char* serverID, const char* roleID, const char* extend)
{
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lib/Cocos2dxActivity", "getInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if (isHave)
    {
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    }
    
    isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lib/Cocos2dxActivity","toGoogleBillPayPage", "(Ljava/lang/String;DLjava/lang/String;)V");
    if (isHave)
    {
        jstring pid = minfo.env->NewStringUTF(identifier);
        jdouble money = amountInCents;
        jstring ext = minfo.env->NewStringUTF(extend);

        minfo.env->CallVoidMethod(jobj, minfo.methodID, pid, money, ext);
        minfo.env->DeleteLocalRef(jobj);
        minfo.env->DeleteLocalRef(pid);
        minfo.env->DeleteLocalRef(ext);
        minfo.env->DeleteLocalRef(minfo.classID);
    } else
    {
        CCLOG("Failed to get the function pay");
    }
}

void OASSdk::setUserInfo(const char* roleID, const char* serverID, const char* serverType, const char* serverName, const char* username)
{
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lib/Cocos2dxActivity", "getInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if (isHave)
    {
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    }
    
    isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lib/Cocos2dxActivity","setUserInfo", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if (isHave)
    {
        jstring rid = minfo.env->NewStringUTF(roleID);
        jstring sid = minfo.env->NewStringUTF(serverID);
        jstring st = minfo.env->NewStringUTF(serverType);
        jstring sn = minfo.env->NewStringUTF(serverName);
        jstring un = minfo.env->NewStringUTF(username);
        
        minfo.env->CallVoidMethod(jobj, minfo.methodID, sid, sn, st, un, rid);
        minfo.env->DeleteLocalRef(jobj);
        minfo.env->DeleteLocalRef(rid);
        minfo.env->DeleteLocalRef(sid);
        minfo.env->DeleteLocalRef(st);
        minfo.env->DeleteLocalRef(sn);
        minfo.env->DeleteLocalRef(un);
        minfo.env->DeleteLocalRef(minfo.classID);
    } else
    {
        CCLOG("Failed to get the function setUserInfo");
    }
}

void OASSdk::switchUser()
{
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lib/Cocos2dxActivity", "getInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if (isHave)
    {
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    }
    
    isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lib/Cocos2dxActivity","switchUser","()V");
    if (isHave)
    {
        minfo.env->CallVoidMethod(jobj, minfo.methodID);
        minfo.env->DeleteLocalRef(jobj);
        minfo.env->DeleteLocalRef(minfo.classID);
    } else
    {
        CCLOG("Failed to get the function switchUser");
    }
}

void OASSdk::cleanGameInfo()
{
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lib/Cocos2dxActivity", "getInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if (isHave)
    {
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    }
    
    isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lib/Cocos2dxActivity","cleanGameInfo","()V");
    if (isHave)
    {
        minfo.env->CallVoidMethod(jobj, minfo.methodID);
        minfo.env->DeleteLocalRef(jobj);
        minfo.env->DeleteLocalRef(minfo.classID);
    } else
    {
        CCLOG("Failed to get the function cleanGameInfo");
    }
}

void OASSdk::trackEvent(const char* eventToken, cocos2d::ValueMap params)
{
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lib/Cocos2dxActivity", "getInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if (isHave)
    {
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    }
    
    isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lib/Cocos2dxActivity","trackEvent", "(Ljava/lang/String;Ljava/util/HashMap;)V");
    if (isHave)
    {
        CCLOG("Sucess to get the function trackEvent");
        jstring et = minfo.env->NewStringUTF(eventToken);
        jobject p = createJavaMapObject(params);
        
        minfo.env->CallVoidMethod(jobj, minfo.methodID, et, p);
        minfo.env->DeleteLocalRef(jobj);
        minfo.env->DeleteLocalRef(et);
        minfo.env->DeleteLocalRef(p);
        minfo.env->DeleteLocalRef(minfo.classID);
    } else
    {
        CCLOG("Failed to get the function trackEvent");
    }
}

void OASSdk::trackForAdv(double amountInCents, const char* eventToken, cocos2d::ValueMap params)
{
}

void OASSdk::trackForPay(double amountInCents, const char* transactionId, const char* eventToken, cocos2d::ValueMap params)
{
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lib/Cocos2dxActivity", "getInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if (isHave)
    {
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    }
    
    isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lib/Cocos2dxActivity","trackRevenue", "(DLjava/lang/String;Ljava/lang/String;Ljava/util/HashMap;)V");
    if (isHave)
    {
        jdouble at = amountInCents;
        jstring td = minfo.env->NewStringUTF(eventToken);
        jstring et = minfo.env->NewStringUTF(transactionId);
        jobject p = createJavaMapObject(params);
        
        minfo.env->CallVoidMethod(jobj, minfo.methodID, at, td, et, p);
        minfo.env->DeleteLocalRef(jobj);
        minfo.env->DeleteLocalRef(td);
        minfo.env->DeleteLocalRef(et);
        minfo.env->DeleteLocalRef(p);
        minfo.env->DeleteLocalRef(minfo.classID);
    } else
    {
        CCLOG("Failed to get the function trackForPay");
    }
}

void OASSdk::registerHandler(OasHandlerType type, int handle)
{
    _mapHandlers[type] = handle;
    
}

void OASSdk::unregisterHandler(OasHandlerType type)
{
    auto iter = _mapHandlers.find(type);
    if (_mapHandlers.end() != iter) {
        _mapHandlers.erase(type);
    }
}

void OASSdk::sendEvent(OasHandlerType type, LuaValueDict info)
{
    
    auto iter = _mapHandlers.find(type);
    if (_mapHandlers.end() != iter)
    {
        LuaStack *pStack = LuaEngine::getInstance()->getLuaStack();
        pStack->pushLuaValueDict(info);
        pStack->executeFunctionByHandler(iter->second, 1);
        pStack->clean();
    }
}


// 新增
void OASSdk::postStatusUpdateWithShareDialogComplete()
{
    
}
     
void OASSdk::shareNewFeedWithMessaga(const char* messga)
{
    
    
}

void OASSdk::shareWithImage(const char* path)
{
    
}

void OASSdk::getFrindsWithLimit(int limit, bool isNext, bool frombegin)
{

    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lib/Cocos2dxActivity", "getInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if (isHave)
    {
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    }
    
    isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lib/Cocos2dxActivity","getFriends", "(IZ)V");
    if (isHave)
    {
        jboolean s = isNext;
        jint l = limit;
        minfo.env->CallVoidMethod(jobj, minfo.methodID, l, s);
        minfo.env->DeleteLocalRef(jobj);
        minfo.env->DeleteLocalRef(minfo.classID);
    } else
    {
        CCLOG("Failed to get the function getFriends");
    }
}
void OASSdk::getInvitedFriendsWithLimit(int limit, bool isNext, bool frombegin)
{
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lib/Cocos2dxActivity", "getInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if (isHave)
    {
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    }
    
    isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lib/Cocos2dxActivity","getInvitableFriends", "(IZ)V");
    if (isHave)
    {
        jboolean s = isNext;
        jint l = limit;
        minfo.env->CallVoidMethod(jobj, minfo.methodID, l, s);
        minfo.env->DeleteLocalRef(jobj);
        minfo.env->DeleteLocalRef(minfo.classID);
    } else
    {
        CCLOG("Failed to get the function getInvitableFriends");
    }
}
void OASSdk::setAppRequestWithActiontype(const char* actiontype, const char* uids, const char* message, const char* objectid)
{
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, "org/cocos2dx/lib/Cocos2dxActivity", "getInstance", "()Ljava/lang/Object;");
    jobject jobj;
    if (isHave)
    {
        jobj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
    }
    
    isHave = JniHelper::getMethodInfo(minfo,"org/cocos2dx/lib/Cocos2dxActivity","setAppRequest", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if (isHave)
    {
        jstring at = minfo.env->NewStringUTF(actiontype);
        jstring us = minfo.env->NewStringUTF(uids);
        jstring msg = minfo.env->NewStringUTF(message);
        jstring obj = minfo.env->NewStringUTF(objectid);

        
        minfo.env->CallVoidMethod(jobj, minfo.methodID, at, us, msg, obj);
        minfo.env->DeleteLocalRef(jobj);
        minfo.env->DeleteLocalRef(at);
        minfo.env->DeleteLocalRef(us);
        minfo.env->DeleteLocalRef(msg);
        minfo.env->DeleteLocalRef(obj);
        minfo.env->DeleteLocalRef(minfo.classID);
    } else
    {
        CCLOG("Failed to get the function setAppRequest");
    }
}

extern "C"
{
    
    void Java_org_cocos2dx_lib_Cocos2dxActivity_funcLoginResult(JNIEnv *env, jobject thiz, jboolean status, jstring uid, jstring token, jstring type, jstring platform, jstring pltfmToken)
    {
        LuaValueDict dict;
        
        std::string userID = JniHelper::jstring2string(uid);
        std::string to = JniHelper::jstring2string(token);
        std::string ty = JniHelper::jstring2string(type);
        std::string pl = JniHelper::jstring2string(platform);
        std::string plt = JniHelper::jstring2string(pltfmToken);
        
        dict.insert(dict.end(), LuaValueDict::value_type("userID", LuaValue::stringValue(userID)));
        dict.insert(dict.end(), LuaValueDict::value_type("token", LuaValue::stringValue(to)));
        dict.insert(dict.end(), LuaValueDict::value_type("type", LuaValue::stringValue(ty)));
        dict.insert(dict.end(), LuaValueDict::value_type("platform", LuaValue::stringValue(pl)));
        dict.insert(dict.end(), LuaValueDict::value_type("platformToken", LuaValue::stringValue(plt)));
        
        OASSdk::getInstance()->sendEvent(OASSdk::OasHandlerType::LOGIN, dict);
    }
    
    void Java_org_cocos2dx_lib_Cocos2dxActivity_funcPayResult(JNIEnv *env, jobject thiz, jboolean status, jstring error)
    {
        LuaValueDict dict;
        if (status) {
            dict.insert(dict.end(), LuaValueDict::value_type("success", LuaValue::intValue(1)));
        } else {
            std::string er = JniHelper::jstring2string(error);
            dict.insert(dict.end(), LuaValueDict::value_type("success", LuaValue::intValue(0)));
            dict.insert(dict.end(), LuaValueDict::value_type("errormsg", LuaValue::stringValue(er)));
        }
        OASSdk::getInstance()->sendEvent(OASSdk::OasHandlerType::PAY, dict);
    }
    
    void Java_org_cocos2dx_lib_Cocos2dxActivity_funcShareResult(JNIEnv *env, jobject thiz, jboolean status, jstring error)
    {

        LuaValueDict dict;
        if (status) {
            dict.insert(dict.end(), LuaValueDict::value_type("success", LuaValue::intValue(1)));
        } else {
            std::string er = JniHelper::jstring2string(error);
            dict.insert(dict.end(), LuaValueDict::value_type("success", LuaValue::intValue(0)));
            dict.insert(dict.end(), LuaValueDict::value_type("errormsg", LuaValue::stringValue(er)));
        }
        OASSdk::getInstance()->sendEvent(OASSdk::OasHandlerType::SHARE, dict);
    }
    
    void Java_org_cocos2dx_lib_Cocos2dxActivity_funcInvitedFriendsResult(JNIEnv *env, jobject thiz, jboolean status, jobjectArray friends)
    {
        LuaValueDict dict;
        if (status) {
            dict.insert(dict.end(), LuaValueDict::value_type("success", LuaValue::intValue(1)));
            
            LuaValueArray friendList;
            jint count = env->GetArrayLength(friends);
            for (int i = 0; i < count; i++){
                jobject jobj = env->GetObjectArrayElement(friends, i);
                LuaValueDict info = getFrinedsDict(jobj);
                friendList.push_back(LuaValue::dictValue(info));
            }
            dict.insert(dict.end(), LuaValueDict::value_type("data", LuaValue::arrayValue(friendList)));
            
        } else {
//            std::string er = JniHelper::jstring2string(error);
            dict.insert(dict.end(), LuaValueDict::value_type("success", LuaValue::intValue(0)));
        }
        
        OASSdk::getInstance()->sendEvent(OASSdk::OasHandlerType::GETINVITEDFRIENDS, dict);
    }
    
    void Java_org_cocos2dx_lib_Cocos2dxActivity_funcFrindsResult(JNIEnv *env, jobject thiz, jboolean status, jobjectArray friends)
    {
        LuaValueDict dict;
        if (status) {
            dict.insert(dict.end(), LuaValueDict::value_type("success", LuaValue::intValue(1)));
            
            LuaValueArray friendList;
            jint count = env->GetArrayLength(friends);
            for (int i = 0; i < count; i++){
                jobject jobj = env->GetObjectArrayElement(friends, i);
                LuaValueDict info = getFrinedsDict(jobj);
                friendList.push_back(LuaValue::dictValue(info));
            }
            dict.insert(dict.end(), LuaValueDict::value_type("data", LuaValue::arrayValue(friendList)));
            
        } else {
//            std::string er = JniHelper::jstring2string(error);
            dict.insert(dict.end(), LuaValueDict::value_type("success", LuaValue::intValue(0)));
        }
        
        OASSdk::getInstance()->sendEvent(OASSdk::OasHandlerType::GETFRIENDS, dict);

    }
    
    void Java_org_cocos2dx_lib_Cocos2dxActivity_funcSetAppRequestResult(JNIEnv *env, jobject thiz, jint action, jboolean status, jstring error)
    {
        std::string er = JniHelper::jstring2string(error);
        
        LuaValueDict dict;
        if (status) {
            dict.insert(dict.end(), LuaValueDict::value_type("success", LuaValue::intValue(1)));
        } else {
            dict.insert(dict.end(), LuaValueDict::value_type("success", LuaValue::intValue(0)));
            dict.insert(dict.end(), LuaValueDict::value_type("errormsg", LuaValue::stringValue(er)));
        }
        OASSdk::getInstance()->sendEvent(OASSdk::OasHandlerType::APPREQUESTS, dict);
    }
}

NS_CC_LEVER_EXT_END;