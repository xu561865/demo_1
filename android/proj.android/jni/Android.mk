
#LOCAL_CPPFLAGS := -std=c++11 -pthread -frtti -fexceptions

LOCAL_ARM_MODE := arm

LOCAL_PATH := $(call my-dir)

COCOS2DX_PATH := $(LOCAL_PATH)/../../../cocos2dx/cocos2dx

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := main/main.cpp 

SRC_DIRS = Classes \
           Classes/android \
           Classes/audio \
           Classes/data \
           Classes/data/fight \
           Classes/network \
           Classes/ui \
           mlib/ccext \
           mlib/ccext/android \
           mlib/c++ \
           mlib/c++/android \
           

SRCS := $(foreach d, $(SRC_DIRS), $(wildcard $(LOCAL_PATH)/../../$(d)/*.cpp))
LOCAL_SRC_FILES += $(patsubst $(LOCAL_PATH)/%,%,$(SRCS))

LOCAL_C_INCLUDES := $(foreach d, $(SRC_DIRS), $(LOCAL_PATH)/../../$(d))

#LOCAL_C_INCLUDES += $(COCOS2DX_PATH)/platform/third_party/emscripten/libxml2
#LOCAL_STATIC_LIBRARIES := libxml2

#$(info $(LOCAL_C_INCLUDES))

#vpath % $(foreach d, $(SRC_DIRS), :$(d))
#SRCS := $(foreach d, $(SRC_DIRS), $(wildcard $(d)/*.cpp))
#OBJS := $(patsubst %.cpp,.build/%.o, $(notdir $(SRCS)))
#INC_FLAGS := $(foreach d, $(SRC_DIRS), $(d))

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx) \
$(call import-module,CocosDenshion/android) \
$(call import-module,extensions)
