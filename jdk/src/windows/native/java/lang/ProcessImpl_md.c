/*
 * Copyright (c) 1997, 2013, Oracle and/or its affiliates. All rights reserved.
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

#include <stdbool.h>
#include <assert.h>
#include "java_lang_ProcessImpl.h"

#include "jni.h"
#include "jvm.h"
#include "jni_util.h"
#include "io_util.h"
#include <windows.h>
#include <io.h>

JNIEXPORT jlong JNICALL
Java_java_lang_ProcessImpl_create(JNIEnv *env, jclass ignored,
                                  jstring cmd,
                                  jstring envBlock,
                                  jstring dir,
                                  jlongArray stdHandles,
                                  jboolean redirectErrorStream)
{
	JNU_ThrowIllegalAccessException(env, "Cannot use this feature on the UWP Platform");
	return 0;
}

JNIEXPORT jint JNICALL
Java_java_lang_ProcessImpl_getExitCodeProcess(JNIEnv *env, jclass ignored, jlong handle)
{
	return 1;
}

JNIEXPORT jint JNICALL
Java_java_lang_ProcessImpl_getStillActive(JNIEnv *env, jclass ignored)
{
    return 0;
}

JNIEXPORT void JNICALL
Java_java_lang_ProcessImpl_waitForInterruptibly(JNIEnv *env, jclass ignored, jlong handle)
{
}

JNIEXPORT void JNICALL
Java_java_lang_ProcessImpl_waitForTimeoutInterruptibly(JNIEnv *env,
                                                       jclass ignored,
                                                       jlong handle,
                                                       jlong timeout)
{
}

JNIEXPORT void JNICALL
Java_java_lang_ProcessImpl_terminateProcess(JNIEnv *env, jclass ignored, jlong handle)
{
	// Would be supported, but we dont want half-working classes.
    // TerminateProcess((HANDLE) handle, 1);
}

JNIEXPORT jboolean JNICALL
Java_java_lang_ProcessImpl_isProcessAlive(JNIEnv *env, jclass ignored, jlong handle)
{
	return false;
}

JNIEXPORT jboolean JNICALL
Java_java_lang_ProcessImpl_closeHandle(JNIEnv *env, jclass ignored, jlong handle)
{
	return false;
}

/**
 * Returns a copy of the Unicode characters of a string. Fow now this
 * function doesn't handle long path names and other issues.
 */
static WCHAR* getPath(JNIEnv *env, jstring ps) {
    WCHAR *pathbuf = NULL;
    const jchar *chars = (*(env))->GetStringChars(env, ps, NULL);
    if (chars != NULL) {
        size_t pathlen = wcslen(chars);
        pathbuf = (WCHAR*)malloc((pathlen + 6) * sizeof(WCHAR));
        if (pathbuf == NULL) {
            JNU_ThrowOutOfMemoryError(env, NULL);
        } else {
            wcscpy(pathbuf, chars);
        }
        (*env)->ReleaseStringChars(env, ps, chars);
    }
    return pathbuf;
}

JNIEXPORT jlong JNICALL
Java_java_lang_ProcessImpl_openForAtomicAppend(JNIEnv *env, jclass ignored, jstring path)
{
	JNU_ThrowIllegalAccessException(env, "Cannot use this feature on the UWP Platform");
}
