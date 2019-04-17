

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
        adxl345.cpp

LOCAL_MODULE_RELATIVE_PATH := hw

LOCAL_MODULE := adxl345.$(TARGET_BOARD_PLATFORM)

LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)