LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := plotsdl_wrapper

SDL_PATH := ../SDL

LOCAL_C_INCLUDES := /home/abathur/Bureau/open_source/plot-sdl/release/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SDL_PATH)/include

# Add your application source files here...
LOCAL_SRC_FILES := SDL_android_main.c \
	main.c

LOCAL_SHARED_LIBRARIES := SDL2 plotsdl

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)
