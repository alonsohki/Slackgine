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

MY_LOCAL_CFLAGS := -Wall -pipe -fno-exceptions -fno-rtti -DANDROID -I. -I.. -I../Android/platform_external_libpng
MY_LOCAL_CXXFLAGS := $(MY_LOCAL_CFLAGS)

BUILD_HOST_STATIC_LIBRARY :=
include platform_external_libpng/Android.mk

LOCAL_PATH := ../core
include $(CLEAR_VARS)
LOCAL_CFLAGS            := $(MY_LOCAL_CFLAGS)
LOCAL_CXXFLAGS          := $(MY_LOCAL_CXXFLAGS)
LOCAL_MODULE            := core
LOCAL_MODULE_FILENAME   := libcore
include ../core/Android.mk
include $(BUILD_STATIC_LIBRARY)

LOCAL_PATH := ../shared
include $(CLEAR_VARS)
LOCAL_CFLAGS            := $(MY_LOCAL_CFLAGS)
LOCAL_CXXFLAGS          := $(MY_LOCAL_CXXFLAGS)
LOCAL_MODULE            := shared
LOCAL_MODULE_FILENAME   := libshared
include ../shared/Android.mk
include $(BUILD_STATIC_LIBRARY)

LOCAL_PATH := ../math
include $(CLEAR_VARS)
LOCAL_CFLAGS            := $(MY_LOCAL_CFLAGS)
LOCAL_CXXFLAGS          := $(MY_LOCAL_CXXFLAGS)
LOCAL_MODULE            := math
LOCAL_MODULE_FILENAME   := libmath
include ../math/Android.mk
include $(BUILD_STATIC_LIBRARY)

LOCAL_PATH := ../l3m
include $(CLEAR_VARS)
LOCAL_CFLAGS            := $(MY_LOCAL_CFLAGS)
LOCAL_CXXFLAGS          := $(MY_LOCAL_CXXFLAGS)
LOCAL_MODULE            := l3m
LOCAL_MODULE_FILENAME   := libl3m
include ../l3m/Android.mk
include $(BUILD_STATIC_LIBRARY)

LOCAL_PATH := ../renderer
include $(CLEAR_VARS)
LOCAL_CFLAGS            := $(MY_LOCAL_CFLAGS)
LOCAL_CXXFLAGS          := $(MY_LOCAL_CXXFLAGS)
LOCAL_MODULE            := renderer
LOCAL_MODULE_FILENAME   := librenderer
include ../renderer/Android.mk
include $(BUILD_STATIC_LIBRARY)

