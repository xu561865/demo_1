APP_STL := gnustl_static
APP_CPPFLAGS := -frtti -DCOCOS2D_DEBUG=1 -std=c++11 -pthread -DLOBR
APP_CPPFLAGS += -Wno-error=format-security
APP_CPPFLAGS += -fexceptions

APP_ABI=armeabi

#APP_PLATFORM := android-8

NDK_TOOLCHAIN_VERSION := 4.8