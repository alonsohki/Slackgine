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

ANDROID_LEVEL=android-9
MY_LOCAL_CFLAGS := -Wall -ansi -pipe -fno-exceptions -fno-rtti -DANDROID -I. -I.. -I../Android/platform_external_libpng -I/soft/dev/android-ndk/platforms/android-9/arch-arm/usr/include/
MY_LOCAL_CXXFLAGS := $(LOCAL_CFLAGS)

include platform_external_libpng/Android.mk

LOCAL_PATH := ../core
include $(CLEAR_VARS)
LOCAL_CFLAGS            := $(MY_LOCAL_CFLAGS)
LOCAL_CXXFLAGS          := $(MY_LOCAL_CXXFLAGS)
LOCAL_MODULE            := core
LOCAL_MODULE_FILENAME   := core
include ../core/Android.mk
include $(BUILD_STATIC_LIBRARY)

LOCAL_PATH := ../shared
include $(CLEAR_VARS)
LOCAL_CFLAGS            := $(MY_LOCAL_CFLAGS)
LOCAL_CXXFLAGS          := $(MY_LOCAL_CXXFLAGS)
LOCAL_MODULE            := shared
LOCAL_MODULE_FILENAME   := shared
include ../shared/Android.mk
include $(BUILD_STATIC_LIBRARY)

LOCAL_PATH := ../math
include $(CLEAR_VARS)
LOCAL_CFLAGS            := $(MY_LOCAL_CFLAGS)
LOCAL_CXXFLAGS          := $(MY_LOCAL_CXXFLAGS)
LOCAL_MODULE            := math
LOCAL_MODULE_FILENAME   := math
include ../math/Android.mk
include $(BUILD_STATIC_LIBRARY)

LOCAL_PATH := ../l3m
include $(CLEAR_VARS)
LOCAL_CFLAGS            := $(MY_LOCAL_CFLAGS)
LOCAL_CXXFLAGS          := $(MY_LOCAL_CXXFLAGS)
LOCAL_MODULE            := l3m
LOCAL_MODULE_FILENAME   := l3m
include ../l3m/Android.mk
include $(BUILD_STATIC_LIBRARY)

