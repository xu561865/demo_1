LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
../../Classes/AnimatePacker.cpp \
../../Classes/AppDelegate.cpp \
../../Classes/CData.cpp \
../../Classes/FireWork.cpp \
../../Classes/GameOver.cpp \
../../Classes/goodsItem.cpp \
../../Classes/HelloWorldScene.cpp \
../../Classes/LodingLayer.cpp \
../../Classes/next.cpp \
../../Classes/personalinfo.cpp \
../../Classes/PersonFire.cpp \
../../Classes/skillList.cpp \
../../Classes/TeXiao.cpp \
../../Classes/tiplayer.cpp \
../../Classes/UserDataModel.cpp \
../../Classes/buzhen/heroItem.cpp \
../../Classes/buzhen/newBuZhen.cpp \
../../Classes/employment/friendemploy.cpp \
../../Classes/employment/friendemploycell.cpp \
../../Classes/employment/friendemploylog.cpp \
../../Classes/employment/friendemploypop.cpp \
../../Classes/employment/friendinfo.cpp \
../../Classes/employment/friendmain.cpp \
../../Classes/extentionClass/MyMenu.cpp \
../../Classes/extentionClass/MyTableView.cpp \
../../Classes/hecheng/CHecheng.cpp \
../../Classes/hecheng/hcbag.cpp \
../../Classes/hecheng/hcgoodsitem.cpp \
../../Classes/hecheng/hctooltip.cpp \
../../Classes/json/src/lib_json/json_reader.cpp \
../../Classes/json/src/lib_json/json_value.cpp \
../../Classes/json/src/lib_json/json_writer.cpp \
../../Classes/loginregister/LayerLogin.cpp \
../../Classes/loginregister/logo.cpp \
../../Classes/loginregister/startAnimate.cpp \
../../Classes/loginregister/xuanren.cpp \
../../Classes/network/ByteBuffer.cpp \
../../Classes/network/MessageQueue.cpp \
../../Classes/network/SocketClient.cpp \
../../Classes/network/SocketManager.cpp \
../../Classes/network/Util.cpp \
../../Classes/network/message.cpp \
../../Classes/utils/language/LanguageManager.cpp \
../../Classes/utils/xml/MyXmlParser.cpp \
../../Classes/zhujiemian/ArenaWindow.cpp \
../../Classes/zhujiemian/CustomPop.cpp \
../../Classes/zhujiemian/LevelMap.cpp \
../../Classes/zhujiemian/MailInfo.cpp \
../../Classes/zhujiemian/MailRead.cpp \
../../Classes/zhujiemian/MailRenderer.cpp \
../../Classes/zhujiemian/MailWindow.cpp \
../../Classes/zhujiemian/MailWrite.cpp \
../../Classes/zhujiemian/homePage.cpp \
../../Classes/zhujiemian/popwindow.cpp \
../../Classes/zhujiemian/zhuangtaiLoad.cpp \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../Classes/buzhen \
					$(LOCAL_PATH)/../../Classes/employment \
					$(LOCAL_PATH)/../../Classes/extentionClass \
					$(LOCAL_PATH)/../../Classes/headFile \
					$(LOCAL_PATH)/../../Classes/hecheng \
					$(LOCAL_PATH)/../../Classes/json/include/json \
					$(LOCAL_PATH)/../../Classes/loginregister \
                    $(LOCAL_PATH)/../../Classes/network \
                    $(LOCAL_PATH)/../../Classes/utils \
                    $(LOCAL_PATH)/../../Classes/utils/xml \
                    $(LOCAL_PATH)/../../Classes/zhujiemian \


LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
