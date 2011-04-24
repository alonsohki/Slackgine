# Copyright (C) 2009 The Android Open Source Project
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
#
MY_LOCAL_PATH := $(call my-dir)
MY_LOCAL_MODULES := 

LOCAL_CFLAGS := -Wall -ansi -pipe -O3 -fno-exceptions -fno-rtti -DANDROID -I. -I.. -DDEBUG
LOCAL_CXXFLAGS := $(LOCAL_CFLAGS)

# shared
LOCAL_PATH := $(MY_LOCAL_PATH)/../../shared
include ../shared/Android.mk
MY_LOCAL_MODULES += $(LOCAL_MODULE)

# math
LOCAL_PATH := $(MY_LOCAL_PATH)/../../math
include ../math/Android.mk
MY_LOCAL_MODULES += $(LOCAL_MODULE)

# core
LOCAL_PATH := $(MY_LOCAL_PATH)/../../core
include ../core/Android.mk
MY_LOCAL_MODULES += $(LOCAL_MODULE)

# l3m
LOCAL_PATH := $(MY_LOCAL_PATH)/../../l3m
include ../l3m/Android.mk
MY_LOCAL_MODULES += $(LOCAL_MODULE)

# renderer
LOCAL_PATH := $(MY_LOCAL_PATH)/../../renderer
include ../renderer/Android.mk
MY_LOCAL_MODULES += $(LOCAL_MODULE)

# Binding
LOCAL_PATH := $(MY_LOCAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := Slackgine-jni-bind
LOCAL_SRC_FILES := Slackgine.cpp
LOCAL_STATIC_LIBRARIES := $(MY_LOCAL_MODULES)
LOCAL_CFLAGS += -I../.. -I.. -I../../..
LOCAL_CXXFLAGS += -I../.. -I.. -I../../..
LOCAL_LDLIBS := -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)
