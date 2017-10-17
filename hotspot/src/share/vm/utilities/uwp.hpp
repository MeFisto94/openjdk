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

#ifndef SHARE_VM_UTILITIES_UWP_HPP
#define SHARE_VM_UTILITIES_UWP_HPP

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0A00 // Win 10
#endif

#include <windows.h>
#include "../../../../../jdk/src/windows/native/common/winapi_headers.h"

#pragma warning(disable: 4996)
#pragma warning(disable: 4267)
#pragma warning(disable: 4005)

extern "C" {

	// Defines the Bitmask to grab all file attributes out of the flags. ENCRYPTED is the highest available flag and
	// since a flag is (1 << x), ((1 << x) - 1) == (1 << x - 1 ) | (1 << x - 2) | .... 1;
	// so we select everything smaller or equal to ENCRYPTED
#define FILE_ATTRIBUTES FILE_ATTRIBUTE_ENCRYPTED  | (FILE_ATTRIBUTE_ENCRYPTED - 1)
#define FILE_FLAGS !FILE_ATTRIBUTES

//#define CreateFileA(A, B, C, D, E, F, G) 
	HANDLE inline UWP_CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDispotion, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile) {
		_CREATEFILE2_EXTENDED_PARAMETERS *memPtr = (_CREATEFILE2_EXTENDED_PARAMETERS*)malloc(sizeof(_CREATEFILE2_EXTENDED_PARAMETERS));
		memPtr->lpSecurityAttributes = lpSecurityAttributes;
		memPtr->hTemplateFile = hTemplateFile;
		/* Note that this extraction will only work as long as the flags dont change their values. If this happens we should rather just
		call CreateFile2 then */
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
	}

	LPVOID inline UWP_MapViewOfFile(HANDLE hFileMappingObject, DWORD dwDesiredAccess, DWORD dwFileOffsetHigh, DWORD dwFileOffsetLow, SIZE_T dwNumberOfBytesToMap) {
		return (LPVOID)MapViewOfFileFromApp(hFileMappingObject, dwDesiredAccess, ((ULONG64)dwFileOffsetHigh) << 32L | dwFileOffsetLow, dwNumberOfBytesToMap);
	}

	LPVOID inline UWP_MapViewOfFileEx(HANDLE hFileMappingObject, DWORD dwDesiredAccess, DWORD dwFileOffsetHigh, DWORD dwFileOffsetLow, SIZE_T dwNumberOfBytesToMap, LPVOID lpBaseAddress) {
		// Unfortunately there is no overloaded version allow lpBaseAddress...
		return UWP_MapViewOfFile(hFileMappingObject, dwDesiredAccess, dwFileOffsetHigh, dwFileOffsetLow, dwNumberOfBytesToMap);
	}

	BOOL inline UWP_CreateProcessA(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles,
		DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation) {
		// There are ways of launching things on UWP but you cannot launch Win32 Apps, you can only launch by URL/File, so launching a png launches the image viewer.
		return false;
	}

	HMODULE inline UWP_GetModuleHandleA(LPCSTR lpLibFileName) {
		size_t len = strlen(lpLibFileName);
		PWSTR buf = (PWSTR)malloc(len + 1);
		mbstowcs(buf, lpLibFileName, len);

		HMODULE h = LoadPackagedLibrary(buf, 0);
		free((void*)buf);
		return h;
	}

	BOOLEAN inline UWP_RtlAddFunctionTable(PRUNTIME_FUNCTION FunctionTable, DWORD EntryCount, DWORD64 BaseAddress) {
		return false; // Criticial, since the vm might stop booting up actually
	}

	void inline UWP_DebugBreak() {
		//__asm { int 3 };
		// Compiler says C4235, not supported on this architecture...
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

		return true;
	}
}

#define CreateFile UWP_CreateFileA
#define GetEnvironmentVariable UWP_GetEnvironmentVariableA
#define SetEnvironmentVariable UWP_SetEnvironmentVariableA
#define GetTempPath UWP_GetTempPathA
#define GetSystemDirectory UWP_GetSystemDirectoryA
#define GetWindowsDirectory UWP_GetWindowsDirectoryA
#define OpenFileMapping UWP_OpenFileMappingA
#define CreateFileMapping UWP_CreateFileMappingA
#define MapViewOfFileEx UWP_MapViewOfFileEx
#define MapViewOfFile UWP_MapViewOfFile
#define GetModuleHandle UWP_GetModuleHandleA
#define CreateProcess UWP_CreateProcessA
#define RtlAddFunctionTable UWP_RtlAddFunctionTable
#define DebugBreak UWP_DebugBreak
#define GetVersionExW UWP_GetVersionExW

#endif // SHARE_VM_UTILITIES_UWP_HPP
