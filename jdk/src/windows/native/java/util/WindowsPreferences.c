/*
 * Copyright (c) 2000, 2014, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 */

#include <stdlib.h>
#include <windows.h>
#include "jni.h"
#include "jni_util.h"
#include "jvm.h"
#ifdef __cplusplus
extern "C" {
#endif

	/* Since UWP does not support the registry, we are using the FilePreferences from SOLARIS (LINUX)*/
	/* So these are all empty stubs */
    JNIEXPORT jintArray JNICALL Java_java_util_prefs_WindowsPreferences_WindowsRegOpenKey
               (JNIEnv* env, jclass this_class, jint hKey, jbyteArray lpSubKey, jint securityMask) {
        return NULL;
    }

    JNIEXPORT jint JNICALL Java_java_util_prefs_WindowsPreferences_WindowsRegCloseKey
               (JNIEnv* env, jclass this_class, jint hKey) {
		return -1;
    };

    JNIEXPORT jintArray JNICALL Java_java_util_prefs_WindowsPreferences_WindowsRegCreateKeyEx
               (JNIEnv* env, jclass this_class, jint hKey, jbyteArray lpSubKey) {
		return NULL;
    }

    JNIEXPORT jint JNICALL Java_java_util_prefs_WindowsPreferences_WindowsRegDeleteKey
              (JNIEnv* env, jclass this_class, jint hKey, jbyteArray lpSubKey) {
		return -1;

    };

    JNIEXPORT jint JNICALL Java_java_util_prefs_WindowsPreferences_WindowsRegFlushKey
        (JNIEnv* env, jclass this_class, jint hKey) {
		return -1;
        }

    JNIEXPORT jbyteArray JNICALL Java_java_util_prefs_WindowsPreferences_WindowsRegQueryValueEx
         (JNIEnv* env, jclass this_class, jint hKey, jbyteArray valueName) {
		return NULL;
    }

    JNIEXPORT jint JNICALL Java_java_util_prefs_WindowsPreferences_WindowsRegSetValueEx
    (JNIEnv* env, jclass this_class, jint hKey, jbyteArray valueName, jbyteArray data) {
		return -1;
    }

     JNIEXPORT jint JNICALL Java_java_util_prefs_WindowsPreferences_WindowsRegDeleteValue
            (JNIEnv* env, jclass this_class, jint hKey, jbyteArray valueName) {
		 return -1;
     }

    JNIEXPORT jintArray JNICALL Java_java_util_prefs_WindowsPreferences_WindowsRegQueryInfoKey
                                  (JNIEnv* env, jclass this_class, jint hKey) {
		return NULL;
    }

     JNIEXPORT jbyteArray JNICALL Java_java_util_prefs_WindowsPreferences_WindowsRegEnumKeyEx
     (JNIEnv* env, jclass this_class, jint hKey , jint subKeyIndex, jint maxKeyLength) {
		 return NULL;
     }

     JNIEXPORT jbyteArray JNICALL Java_java_util_prefs_WindowsPreferences_WindowsRegEnumValue
          (JNIEnv* env, jclass this_class, jint hKey , jint valueIndex, jint maxValueNameLength){
		 return NULL;
     }


#ifdef __cplusplus
}
#endif
