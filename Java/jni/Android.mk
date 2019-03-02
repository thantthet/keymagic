# Copyright (C) 2011 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LOCAL_PATH := $(call my-dir)

KEYMAGIC_CORE_SOURCE_DIR := $(LOCAL_PATH)/../../cross-platform/libkeymagic/

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := $(KEYMAGIC_CORE_SOURCE_DIR)

KEYMAGIC_CORE_SRC_FILES := \
	KeyCodes.cpp \
	KeyMagicEngine.cpp \
	KeyMagicErrorLogger.cpp \
	KeyMagicKeyboard.cpp \
	KeyMagicLogger.cpp \
	KeyMagicString.cpp \
	KeyMagicTypes.cpp \
	LogFileWriter.cpp \
	RuleInfo.cpp \
	Util.cpp

LOCAL_SRC_FILES := $(addprefix $(KEYMAGIC_CORE_SOURCE_DIR)/, $(KEYMAGIC_CORE_SRC_FILES))

LOCAL_MODULE := keymagic_core

include $(BUILD_STATIC_LIBRARY)

# ----------- #

include $(CLEAR_VARS)

LOCAL_STATIC_LIBRARIES := keymagic_core

LOCAL_C_INCLUDES := $(KEYMAGIC_CORE_SOURCE_DIR)

LOCAL_SRC_FILES := src/com_keymagic_KeyMagicEngine.cpp src/android_fopen.c

LOCAL_MODULE := jni_keymagic
LOCAL_MODULE_TAGS := optional

LOCAL_CLANG := true
LOCAL_SDK_VERSION := 14
LOCAL_NDK_STL_VARIANT := c++_static
LOCAL_LDFLAGS += -ldl
LOCAL_LDLIBS  += -landroid
LOCAL_LDLIBS  += -llog

include $(BUILD_SHARED_LIBRARY)