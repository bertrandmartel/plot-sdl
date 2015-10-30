LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := plotsdl

SDL_PATH := ../SDL

LOCAL_C_INCLUDES := /home/abathur/Bureau/open_source/plot-sdl/release/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/$(SDL_PATH)/include

LOCAL_SRC_FILES := /home/abathur/Bureau/open_source/plot-sdl/plot-sdl-lib/plot.c \
	/home/abathur/Bureau/open_source/plot-sdl/plot-sdl-lib/linked_list.c

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_ttf

LOCAL_LDLIBS := -lm -llog

include $(BUILD_SHARED_LIBRARY)
