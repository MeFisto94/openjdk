/*
* Copyright (c) 2017 MeFisto94. All rights reserved.
* DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
*
* This code is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 2 only, as
* published by the Free Software Foundation.
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
*/

#ifndef WINAPI_STUB_H
#define WINAPI_STUB_H

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0A00 // Win 10
#endif

#include <stdlib.h>
#include <WinSock2.h>
#include <windows.h>
#include "winapi_headers.h"
#include "../../../share/native/common/jni_util.h"

#pragma warning(disable: 4996)
#pragma warning(disable: 4267)
#pragma warning(disable: 4005)

// Defines the Bitmask to grab all file attributes out of the flags. ENCRYPTED is the highest available flag and
// since a flag is (1 << x), ((1 << x) - 1) == (1 << x - 1 ) | (1 << x - 2) | .... 1;
// so we select everything smaller or equal to ENCRYPTED
#define FILE_ATTRIBUTES FILE_ATTRIBUTE_ENCRYPTED  | (FILE_ATTRIBUTE_ENCRYPTED - 1)
#define FILE_FLAGS !FILE_ATTRIBUTES

//#define CreateFileA(A, B, C, D, E, F, G) 
HANDLE inline UWP_CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDispotion, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
	CREATEFILE2_EXTENDED_PARAMETERS *memPtr = (CREATEFILE2_EXTENDED_PARAMETERS*)malloc(sizeof(CREATEFILE2_EXTENDED_PARAMETERS));
	memPtr->lpSecurityAttributes = lpSecurityAttributes;
	memPtr->hTemplateFile = hTemplateFile;
	//Note that this extraction will only work as long as the flags dont change their values. If this happens we should rather just
	//call CreateFile2 then
	memPtr->dwFileAttributes = dwFlagsAndAttributes & FILE_ATTRIBUTES;
	memPtr->dwFileFlags = dwFlagsAndAttributes & FILE_FLAGS;

	size_t len = strlen(lpFileName) + 1;
	LPWSTR wFileName = (LPWSTR)malloc(len);
	mbstowcs(wFileName, lpFileName, len);

	HANDLE res = CreateFile2((LPCWSTR)wFileName, dwDesiredAccess, dwShareMode, dwCreationDispotion, memPtr);

	free((void*)wFileName);
	free((void*)memPtr);

	return res;
}

HANDLE inline UWP_CreateFileW(LPWSTR wFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDispotion, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
	CREATEFILE2_EXTENDED_PARAMETERS *memPtr = (CREATEFILE2_EXTENDED_PARAMETERS*)malloc(sizeof(CREATEFILE2_EXTENDED_PARAMETERS));
	memPtr->lpSecurityAttributes = lpSecurityAttributes;
	memPtr->hTemplateFile = hTemplateFile;
	//Note that this extraction will only work as long as the flags dont change their values. If this happens we should rather just
	//call CreateFile2 then
	memPtr->dwFileAttributes = dwFlagsAndAttributes & FILE_ATTRIBUTES;
	memPtr->dwFileFlags = dwFlagsAndAttributes & FILE_FLAGS;

	HANDLE res = CreateFile2(wFileName, dwDesiredAccess, dwShareMode, dwCreationDispotion, memPtr);

	free((void*)memPtr);

	return res;
}

/*
DWORD inline UWP_GetEnvironmentVariableA(const char* name, char* buffer, int len) {
	return 0;
}

BOOL inline UWP_SetEnvironmentVariableA(LPCSTR lpName, LPCSTR lpValue) {
	return false;

}

DWORD inline UWP_GetTempPathA(DWORD nBufferLength, LPSTR lpBuffer) {
	LPWSTR wBuffer = (LPWSTR)malloc(nBufferLength + 1); // Uncertain about \0's
	DWORD res = GetTempPathW(nBufferLength, wBuffer);
	wcstombs(lpBuffer, wBuffer, nBufferLength);
	free((void*)wBuffer);
	return res;
}

DWORD inline UWP_GetSystemDirectoryA(LPCSTR lpBuffer, DWORD nBufferLength) {
	lpBuffer = "";
	return NULL;
}

DWORD inline UWP_GetWindowsDirectoryA(LPCSTR lpBuffer, DWORD nBufferLength) {
	lpBuffer = "";
	return NULL;
}

HANDLE inline UWP_CreateFileMappingA(HANDLE hFile, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCSTR lpName) {
	size_t len = strlen(lpName);
	PWSTR buf = (PWSTR)malloc(len + 1);
	mbstowcs(buf, lpName, len);
	HANDLE res = CreateFileMappingFromApp(hFile, lpFileMappingAttributes, flProtect, ((ULONG64)dwMaximumSizeHigh) << 32L | dwMaximumSizeLow, (PCWSTR)buf);
	free((void*)buf);
	return res;
}

HANDLE inline UWP_OpenFileMappingA(DWORD dwDesiredAccess, BOOL bInheritHandle, LPCSTR lpName) {
	size_t len = strlen(lpName);
	PWSTR buf = (PWSTR)malloc(len + 1);
	mbstowcs(buf, lpName, len);
	HANDLE res = OpenFileMappingFromApp(dwDesiredAccess, bInheritHandle, (PCWSTR)buf);
	free((void*)buf);
	return res;
}*/
	
// Note: Semantics are a bit different here: We LOAD the Library, where GetModuleHandle usually acts on already load libs.
// probably LoadPackagedLibrary() caches the results though.
HMODULE inline UWP_GetModuleHandleA(LPCSTR lpLibFileName) {
	size_t len = strlen(lpLibFileName);
	PWSTR buf = (PWSTR)malloc(len + 1);
	mbstowcs(buf, lpLibFileName, len);

	HMODULE h = LoadPackagedLibrary(buf, 0);
	free((void*)buf);
	return h;
}

HANDLE inline UWP_GetStdHandle(DWORD nStdHandle) {
	return INVALID_HANDLE_VALUE;
}

/* Implementations of POSIX/libc/msvcrt methods*/
inline int __cdecl _getdrive() {
	return 3; // This is the drive index, 3 returns C and should work for most paths, however code should not rely on paths at all anymore
}

inline char* __cdecl getenv(char const* _VarName) {
	char *p = (char *)malloc(1); // Empty env var, 
	p[0] = '\0';
	return p;
}

inline int __cdecl _getpid() {
	return GetCurrentProcessId(); // For some reason _getpid isn't supported, GetCurrentProcessId is though
}

inline LPSTR UWP_CharNextExA(WORD CodePage, LPCSTR lpCurrentChar, DWORD dwFlags) {
	if (dwFlags != 0) {
		return NULL;
	}

	//@TODO: CodePage??

	if (lpCurrentChar == '\0') {
		return (LPSTR)lpCurrentChar;
	}
	else {
		return (LPSTR)lpCurrentChar + 1;
	}
}

inline BOOL UWP_EqualSid(PSID pSid1, PSID pSid2) {
	PISID piSid1 = (PISID)pSid1;
	PISID piSid2 = (PISID)pSid2;

	if (piSid1->Revision != piSid2->Revision) {
		return FALSE;
	}

	if (piSid1->SubAuthorityCount != piSid2->SubAuthorityCount) {
		return FALSE;
	}

	for (BYTE i = 0; i < 6; i++) {
		if (piSid1->IdentifierAuthority.Value[i] != piSid2->IdentifierAuthority.Value[i]) {
			return FALSE;
		}
	}

	for (BYTE i = 0; i < piSid1->SubAuthorityCount; i++) {
		if (piSid1->SubAuthority[i] != piSid2->SubAuthority[i]) {
			return FALSE;
		}
	}

	return TRUE;
}

inline void ThrowUnsupportedOpEx(JNIEnv *env, const char* reason) {
	/*jclass ex = env->FindClass("java/lang/UnsupportedOperationException");
	env->ThrowNew(ex, reason);*/
	JNU_ThrowByName(env, "java/lang/UnsupportedOperationException", reason);
}

/* This is actually a "bigger" Problem here, as WSASendDisconnect has been deprecated (and thus not UWPed) becuase one should use WSASend.
   Now the behavior is actually a bit different on both in that there even is WSARecvDisconnect, where the other party can handle and get the 
   disconnect message (but not on TCP/IP anyway, so this is probably irrelevant).

  "Note  The native implementation of TCP/IP on Windows does not support disconnect data. Disconnect data is only supported with Windows Sockets providers
    that have the XP1_DISCONNECT_DATA flag in their WSAPROTOCOL_INFO structure."
*/
inline int WSAAPI UWP_WSASendDisconnect(SOCKET s, LPWSABUF lpOutboundDisconnectData) {
	return send(s, lpOutboundDisconnectData->buf, lpOutboundDisconnectData->len, 0) + shutdown(s, SD_SEND);
}

inline int WINAPI UWP_MessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) {
	OutputDebugStringA("[MessageBox]: ");
	if (lpCaption != NULL) {
		OutputDebugStringA(lpCaption);
		OutputDebugStringA(" -- ");
	}
	if (lpText != NULL) {
		OutputDebugStringA(lpText);
	}
	OutputDebugStringA("\n");

	return 0;
}

inline HMODULE UWP_LoadLibraryA(LPCSTR lpLibFileName) {
	return UWP_GetModuleHandleA(lpLibFileName);
}

inline HDC UWP_GetDC(HWND hWnd) {
	return NULL;
}


inline int UWP_ReleaseDC(HWND hWnd, HDC  hDC) {
	return 0;
}

inline BOOL UWP_ImpersonateSelf(SECURITY_IMPERSONATION_LEVEL ImpersonationLevel) {
	return FALSE;
}


#define CreateFile UWP_CreateFileA
#define CreateFileW UWP_CreateFileW
/*
#define GetEnvironmentVariable UWP_GetEnvironmentVariableA
#define SetEnvironmentVariable UWP_SetEnvironmentVariableA
#define GetTempPath UWP_GetTempPathA
#define GetSystemDirectory UWP_GetSystemDirectoryA
#define GetWindowsDirectory UWP_GetWindowsDirectoryA
#define OpenFileMapping UWP_OpenFileMappingA
#define CreateFileMapping UWP_CreateFileMappingA
#define MapViewOfFileEx UWP_MapViewOfFileEx
#define MapViewOfFile UWP_MapViewOfFile*/
#define GetModuleHandle UWP_GetModuleHandleA
/*#define CreateProcess UWP_CreateProcessA
#define RtlAddFunctionTable UWP_RtlAddFunctionTable*/
#define GetStdHandle UWP_GetStdHandle
#define CharNextExA UWP_CharNextExA
#define EqualSid UWP_EqualSid
#define WSASendDisconnect UWP_WSASendDisconnect
#define MessageBox UWP_MessageBoxA
#define LoadLibrary UWP_LoadLibraryA
#define GetDC UWP_GetDC
#define GetWindowDC UWP_GetDC
#define ReleaseDC UWP_ReleaseDC
#define ImpersonateSelf UWP_ImpersonateSelf

#endif // WINAPI_STUB_H
