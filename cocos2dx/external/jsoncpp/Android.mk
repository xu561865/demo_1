LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := jsoncpp_static
LOCAL_MODULE_FILENAME := libjsoncpp
           
LOCAL_SRC_FILES := json_reader.cpp json_value.cpp json_writer.cpp
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/json

include $(BUILD_STATIC_LIBRARY)