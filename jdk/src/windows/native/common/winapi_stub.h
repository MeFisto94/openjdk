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

#ifndef JDK_WINAPI_STUB_H
#define JDK_WINAPI_STUB_H

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0A00 // Win 10
#endif

#include <stdlib.h>
#include <WinSock2.h>
#include <windows.h>
#include <memoryapi.h>
#include <AccCtrl.h>
#include <AclAPI.h>
#include "winapi_headers.h"
#include "jni.h"
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

HANDLE inline UWP_CreateFileW(LPCWSTR wFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDispotion, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
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
}*/

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
	HANDLE res = OpenFileMappingFromApp((ULONG)dwDesiredAccess, bInheritHandle, (PCWSTR)buf);
	free((void*)buf);
	return res;
}

BOOL inline UWP_GetVersionExW(LPOSVERSIONINFOW lpVersionInformation) {
	// https://www.codeproject.com/Articles/678606/Part-Overcoming-Windows-s-deprecation-of-GetVe would be the solution, but 
	// so much useless work. Plus we want to state that this code is _not_ running in Windows 10 but rather UWP

	// this is correct, 10.0 instead of 6.4, see https://msdn.microsoft.com/en-us/library/windows/desktop/ms724832(v=vs.85).aspx
	lpVersionInformation->dwMajorVersion = 10;
	lpVersionInformation->dwMinorVersion = 0;
	lpVersionInformation->dwBuildNumber = 1337;
	lpVersionInformation->dwPlatformId = VER_PLATFORM_WIN32_NT;
	memcpy(lpVersionInformation->szCSDVersion, L"Service Pack UWP", 17);

	if (lpVersionInformation->dwOSVersionInfoSize == sizeof(OSVERSIONINFOEX)) {
		// Want extended information
		((LPOSVERSIONINFOEXW)lpVersionInformation)->wProductType = VER_NT_WORKSTATION;
		((LPOSVERSIONINFOEXW)lpVersionInformation)->wServicePackMajor = 0;
		((LPOSVERSIONINFOEXW)lpVersionInformation)->wServicePackMinor = 0;
		((LPOSVERSIONINFOEXW)lpVersionInformation)->wSuiteMask = VER_SUITE_PERSONAL | VER_SUITE_TERMINAL | VER_SUITE_SINGLEUSERTS;
	}

	return TRUE;
}
	
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
 *  Now the behavior is actually a bit different on both in that there even is WSARecvDisconnect, where the other party can handle and get the 
 *  disconnect message (but not on TCP/IP anyway, so this is probably irrelevant).
 *
 * "Note  The native implementation of TCP/IP on Windows does not support disconnect data. Disconnect data is only supported with Windows Sockets providers
 *   that have the XP1_DISCONNECT_DATA flag in their WSAPROTOCOL_INFO structure."
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


inline int UWP_ReleaseDC(HWND hWnd, HDC hDC) {
	return 0;
}

inline BOOL UWP_ImpersonateSelf(SECURITY_IMPERSONATION_LEVEL ImpersonationLevel) {
	return FALSE;
}

inline BOOL UWP_GetFileInformationByHandle(HANDLE hFile, LPBY_HANDLE_FILE_INFORMATION lpFileInformation) {
	FILE_BASIC_INFO basic;
	FILE_ID_INFO id;
	FILE_STANDARD_INFO standard;

	BOOL b = GetFileInformationByHandleEx(hFile, FileBasicInfo, &basic, sizeof(FILE_BASIC_INFO));
	b += GetFileInformationByHandleEx(hFile, FileIdInfo, &id, sizeof(FILE_ID_INFO));
	b += GetFileInformationByHandleEx(hFile, FileStandardInfo, &standard, sizeof(FILE_STANDARD_INFO));

	lpFileInformation->dwFileAttributes = basic.FileAttributes;
	lpFileInformation->ftCreationTime.dwLowDateTime = basic.CreationTime.LowPart;
	lpFileInformation->ftCreationTime.dwHighDateTime = basic.CreationTime.HighPart;
	lpFileInformation->ftLastAccessTime.dwLowDateTime = basic.LastAccessTime.LowPart;
	lpFileInformation->ftLastAccessTime.dwHighDateTime = basic.LastAccessTime.HighPart;
	lpFileInformation->ftLastWriteTime.dwLowDateTime = basic.LastWriteTime.LowPart;
	lpFileInformation->ftLastWriteTime.dwHighDateTime = basic.LastWriteTime.HighPart;
	lpFileInformation->dwVolumeSerialNumber = (DWORD)(id.VolumeSerialNumber & 0xFFFFFFFF); // Only take the lower part here as well
	lpFileInformation->nNumberOfLinks = standard.NumberOfLinks;
	lpFileInformation->nFileSizeHigh = standard.EndOfFile.HighPart;
	lpFileInformation->nFileSizeLow = standard.EndOfFile.LowPart;

	// Problem, the real ID is 128 bits but we can only store 64bits in the old API, so we just take the LOWER HALF.
	// Let's just hope this is LittleEndian.
	lpFileInformation->nFileIndexHigh = id.FileId.Identifier[7] << 24 | id.FileId.Identifier[6] << 16 | id.FileId.Identifier[5] << 8 | id.FileId.Identifier[4];
	lpFileInformation->nFileIndexLow  = id.FileId.Identifier[3] << 24 | id.FileId.Identifier[2] << 16 | id.FileId.Identifier[1] << 8 | id.FileId.Identifier[0];

	return b;
}

inline BOOL UWP_SetFileSecurityW(LPCWSTR lpFileName, SECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR pSecurityDescriptor) {
	WCHAR *lpFileNameS = malloc(wcslen(lpFileName) + 1);
	memcpy_s(lpFileNameS, wcslen(lpFileName) + 1, lpFileName, wcslen(lpFileName) + 1);

	PISECURITY_DESCRIPTOR pi = ((PISECURITY_DESCRIPTOR)pSecurityDescriptor);
	BOOL b = SetNamedSecurityInfoW(lpFileNameS, SE_FILE_OBJECT, SecurityInformation, pi->Owner, pi->Group, pi->Dacl, pi->Sacl);
	free(lpFileNameS);
	return b;
}

inline BOOL UWP_GetFileSecurityW(LPCWSTR lpFileName, SECURITY_INFORMATION RequestedInformation, PSECURITY_DESCRIPTOR pSecurityDescriptor, DWORD nLength, LPDWORD lpnLengthNeeded) {
	*lpnLengthNeeded = sizeof(SECURITY_DESCRIPTOR);
	if (nLength <= *lpnLengthNeeded) {
		SetLastError(ERROR_INSUFFICIENT_BUFFER);
		return FALSE;
	}

	return GetNamedSecurityInfoW(lpFileName, SE_FILE_OBJECT, RequestedInformation, NULL, NULL, NULL, NULL, &pSecurityDescriptor);
}


#define CreateFile UWP_CreateFileA
#define CreateFileW UWP_CreateFileW
#define CreateFileMapping UWP_CreateFileMappingA
#define OpenFileMapping UWP_OpenFileMappingA
#define GetModuleHandle UWP_GetModuleHandleA
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
#define GetFileInformationByHandle UWP_GetFileInformationByHandle
#define SetFileSecurityW UWP_SetFileSecurityW
#define GetFileSecurityW UWP_GetFileSecurityW

#endif // WINAPI_STUB_H
