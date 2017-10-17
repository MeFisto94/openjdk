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
#include <windows.h>
#include "winapi_headers.h"

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

#endif // WINAPI_STUB_H
