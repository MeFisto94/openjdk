/*
 * Copyright (c) 2002, 2014, Oracle and/or its affiliates. All rights reserved.
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

/* Need to define this to get CAPI functions included */
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

#include <windows.h>
#include <bcrypt.h>
#include <jni.h>
#include "sun_security_provider_NativeSeedGenerator.h"

#ifndef STATUS_SUCCESS
	#define STATUS_SUCCESS 0x0
#endif

/*
 * Get a random seed from the MS CryptoAPI. Return true if successful, false
 * otherwise.
 *
 * Some early versions of Windows 95 do not support the required functions.
 * Use runtime linking to avoid problems.
 *
 * UWP doesn't support CryptGenRandom anymore but we use BCrypt (since Vista) in Favor of that.
 */
JNIEXPORT jboolean JNICALL Java_sun_security_provider_NativeSeedGenerator_nativeGenerateSeed
  (JNIEnv *env, jclass clazz, jbyteArray randArray)
{
	BCRYPT_ALG_HANDLE hAlg;
    jboolean result = JNI_FALSE;
    jsize numBytes;
    jbyte* randBytes;

	if (BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_RSA_ALGORITHM, NULL, 0) != STATUS_SUCCESS) {
		return JNI_FALSE;
	}

    numBytes = (*env)->GetArrayLength(env, randArray);
    randBytes = (*env)->GetByteArrayElements(env, randArray, NULL);
    if (randBytes == NULL) {
        goto cleanup;
    }

	// Unfortunately the Entropy in randBytes is not used in W8+ (so UWP)
	if (BCryptGenRandom(hAlg, randBytes, numBytes, BCRYPT_RNG_USE_ENTROPY_IN_BUFFER)) {
		result = JNI_TRUE;
	}

    (*env)->ReleaseByteArrayElements(env, randArray, randBytes, 0);

cleanup:
	BCryptCloseAlgorithmProvider(hAlg, 0);

    return result;
}
