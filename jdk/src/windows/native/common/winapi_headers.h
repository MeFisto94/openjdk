/*
* Copyright(c) 2017 MeFisto94.All rights reserved.
* DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
*
* This code is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 2 only, as
* published by the Free Software Foundation.
*
* This code is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
*FITNESS FOR A PARTICULAR PURPOSE.See the GNU General Public License
* version 2 for more details(a copy is included in the LICENSE file that
* accompanied this code).
*
* You should have received a copy of the GNU General Public License version
* 2 along with this work; if not, write to the Free Software Foundation,
*Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110 - 1301 USA.
*
*/

#ifndef WINAPI_HEADERS_H
#define WINAPI_HEADERS_H

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00 // Win 10
#endif

#include <stdlib.h>
#include <windows.h>

	/*#define WSADESCRIPTION_LEN      256
	#define WSASYS_STATUS_LEN       128

	typedef struct WSAData {
		WORD                    wVersion;
		WORD                    wHighVersion;
	#ifdef _WIN64
		unsigned short          iMaxSockets;
		unsigned short          iMaxUdpDg;
		char FAR *              lpVendorInfo;
		char                    szDescription[WSADESCRIPTION_LEN + 1];
		char                    szSystemStatus[WSASYS_STATUS_LEN + 1];
	#else
		char                    szDescription[WSADESCRIPTION_LEN + 1];
		char                    szSystemStatus[WSASYS_STATUS_LEN + 1];
		unsigned short          iMaxSockets;
		unsigned short          iMaxUdpDg;
		char FAR *              lpVendorInfo;
	#endif
	} WSADATA, FAR * LPWSADATA;*/

	// TlHelp32.h
	#define MAX_MODULE_NAME32 255
	typedef struct tagMODULEENTRY32
	{
		DWORD   dwSize;
		DWORD   th32ModuleID;       // This module
		DWORD   th32ProcessID;      // owning process
		DWORD   GlblcntUsage;       // Global usage count on the module
		DWORD   ProccntUsage;       // Module usage count in th32ProcessID's context
		BYTE  * modBaseAddr;        // Base address of module in th32ProcessID's context
		DWORD   modBaseSize;        // Size in bytes of module starting at modBaseAddr
		HMODULE hModule;            // The hModule of this module in th32ProcessID's context
		char    szModule[MAX_MODULE_NAME32 + 1];
		char    szExePath[MAX_PATH];
	} MODULEENTRY32;
	typedef MODULEENTRY32 *  PMODULEENTRY32;
	typedef MODULEENTRY32 *  LPMODULEENTRY32;

	// processthreadsapi.h
	typedef struct _STARTUPINFOA {
		DWORD   cb;
		LPSTR   lpReserved;
		LPSTR   lpDesktop;
		LPSTR   lpTitle;
		DWORD   dwX;
		DWORD   dwY;
		DWORD   dwXSize;
		DWORD   dwYSize;
		DWORD   dwXCountChars;
		DWORD   dwYCountChars;
		DWORD   dwFillAttribute;
		DWORD   dwFlags;
		WORD    wShowWindow;
		WORD    cbReserved2;
		LPBYTE  lpReserved2;
		HANDLE  hStdInput;
		HANDLE  hStdOutput;
		HANDLE  hStdError;
	} STARTUPINFOA, *LPSTARTUPINFOA;

	typedef struct _PROCESS_INFORMATION {
		HANDLE hProcess;
		HANDLE hThread;
		DWORD dwProcessId;
		DWORD dwThreadId;
	} PROCESS_INFORMATION, *PPROCESS_INFORMATION, *LPPROCESS_INFORMATION;

	// versrc.h
	typedef struct tagVS_FIXEDFILEINFO
	{
		DWORD   dwSignature;            /* e.g. 0xfeef04bd */
		DWORD   dwStrucVersion;         /* e.g. 0x00000042 = "0.42" */
		DWORD   dwFileVersionMS;        /* e.g. 0x00030075 = "3.75" */
		DWORD   dwFileVersionLS;        /* e.g. 0x00000031 = "0.31" */
		DWORD   dwProductVersionMS;     /* e.g. 0x00030010 = "3.10" */
		DWORD   dwProductVersionLS;     /* e.g. 0x00000031 = "0.31" */
		DWORD   dwFileFlagsMask;        /* = 0x3F for version "0.42" */
		DWORD   dwFileFlags;            /* e.g. VFF_DEBUG | VFF_PRERELEASE */
		DWORD   dwFileOS;               /* e.g. VOS_DOS_WINDOWS16 */
		DWORD   dwFileType;             /* e.g. VFT_DRIVER */
		DWORD   dwFileSubtype;          /* e.g. VFT2_DRV_KEYBOARD */
		DWORD   dwFileDateMS;           /* e.g. 0 */
		DWORD   dwFileDateLS;           /* e.g. 0 */
	} VS_FIXEDFILEINFO;

	// ImageHlp.h
	typedef enum _MINIDUMP_TYPE {
		MiniDumpNormal = 0x00000000,
		MiniDumpWithDataSegs = 0x00000001,
		MiniDumpWithFullMemory = 0x00000002,
		MiniDumpWithHandleData = 0x00000004,
		MiniDumpFilterMemory = 0x00000008,
		MiniDumpScanMemory = 0x00000010,
		MiniDumpWithUnloadedModules = 0x00000020,
		MiniDumpWithIndirectlyReferencedMemory = 0x00000040,
		MiniDumpFilterModulePaths = 0x00000080,
		MiniDumpWithProcessThreadData = 0x00000100,
		MiniDumpWithPrivateReadWriteMemory = 0x00000200,
		MiniDumpWithoutOptionalData = 0x00000400,
		MiniDumpWithFullMemoryInfo = 0x00000800,
		MiniDumpWithThreadInfo = 0x00001000,
		MiniDumpWithCodeSegs = 0x00002000,
		MiniDumpWithoutAuxiliaryState = 0x00004000,
		MiniDumpWithFullAuxiliaryState = 0x00008000,
		MiniDumpWithPrivateWriteCopyMemory = 0x00010000,
		MiniDumpIgnoreInaccessibleMemory = 0x00020000,
		MiniDumpWithTokenInformation = 0x00040000,
		MiniDumpWithModuleHeaders = 0x00080000,
		MiniDumpFilterTriage = 0x00100000,
		MiniDumpValidTypeFlags = 0x001fffff,
	} MINIDUMP_TYPE;

	typedef struct _MINIDUMP_EXCEPTION_INFORMATION {
		DWORD ThreadId;
		PEXCEPTION_POINTERS ExceptionPointers;
		BOOL ClientPointers;
	} MINIDUMP_EXCEPTION_INFORMATION, *PMINIDUMP_EXCEPTION_INFORMATION;

	typedef struct _MINIDUMP_USER_STREAM {
		ULONG32 Type;
		ULONG BufferSize;
		PVOID Buffer;

	} MINIDUMP_USER_STREAM, *PMINIDUMP_USER_STREAM;

	typedef struct _MINIDUMP_USER_STREAM_INFORMATION {
		ULONG UserStreamCount;
		PMINIDUMP_USER_STREAM UserStreamArray;
	} MINIDUMP_USER_STREAM_INFORMATION, *PMINIDUMP_USER_STREAM_INFORMATION;

	typedef struct _MINIDUMP_THREAD_CALLBACK {
		ULONG ThreadId;
		HANDLE ThreadHandle;
		CONTEXT Context;
		ULONG SizeOfContext;
		ULONG64 StackBase;
		ULONG64 StackEnd;
	} MINIDUMP_THREAD_CALLBACK, *PMINIDUMP_THREAD_CALLBACK;


	typedef struct _MINIDUMP_THREAD_EX_CALLBACK {
		ULONG ThreadId;
		HANDLE ThreadHandle;
		CONTEXT Context;
		ULONG SizeOfContext;
		ULONG64 StackBase;
		ULONG64 StackEnd;
		ULONG64 BackingStoreBase;
		ULONG64 BackingStoreEnd;
	} MINIDUMP_THREAD_EX_CALLBACK, *PMINIDUMP_THREAD_EX_CALLBACK;

	typedef struct _MINIDUMP_MODULE_CALLBACK {
		PWCHAR FullPath;
		ULONG64 BaseOfImage;
		ULONG SizeOfImage;
		ULONG CheckSum;
		ULONG TimeDateStamp;
		VS_FIXEDFILEINFO VersionInfo;
		PVOID CvRecord;
		ULONG SizeOfCvRecord;
		PVOID MiscRecord;
		ULONG SizeOfMiscRecord;
	} MINIDUMP_MODULE_CALLBACK, *PMINIDUMP_MODULE_CALLBACK;

	typedef struct _MINIDUMP_INCLUDE_MODULE_CALLBACK {
		ULONG64 BaseOfImage;
	} MINIDUMP_INCLUDE_MODULE_CALLBACK, *PMINIDUMP_INCLUDE_MODULE_CALLBACK;

	typedef struct _MINIDUMP_INCLUDE_THREAD_CALLBACK {
		ULONG ThreadId;
	} MINIDUMP_INCLUDE_THREAD_CALLBACK, *PMINIDUMP_INCLUDE_THREAD_CALLBACK;

	typedef struct _MINIDUMP_IO_CALLBACK {
		HANDLE Handle;
		ULONG64 Offset;
		PVOID Buffer;
		ULONG BufferBytes;
	} MINIDUMP_IO_CALLBACK, *PMINIDUMP_IO_CALLBACK;

	typedef struct _MINIDUMP_READ_MEMORY_FAILURE_CALLBACK
	{
		ULONG64 Offset;
		ULONG Bytes;
		HRESULT FailureStatus;
	} MINIDUMP_READ_MEMORY_FAILURE_CALLBACK,
		*PMINIDUMP_READ_MEMORY_FAILURE_CALLBACK;

	typedef struct _MINIDUMP_VM_QUERY_CALLBACK
	{
		ULONG64 Offset;
	} MINIDUMP_VM_QUERY_CALLBACK, *PMINIDUMP_VM_QUERY_CALLBACK;

	typedef struct _MINIDUMP_VM_PRE_READ_CALLBACK
	{
		ULONG64 Offset;
		PVOID Buffer;
		ULONG Size;
	} MINIDUMP_VM_PRE_READ_CALLBACK, *PMINIDUMP_VM_PRE_READ_CALLBACK;

	typedef struct _MINIDUMP_VM_POST_READ_CALLBACK
	{
		ULONG64 Offset;
		PVOID Buffer;
		ULONG Size;
		ULONG Completed;
		HRESULT Status;
	} MINIDUMP_VM_POST_READ_CALLBACK, *PMINIDUMP_VM_POST_READ_CALLBACK;

	typedef struct _MINIDUMP_CALLBACK_INPUT {
		ULONG ProcessId;
		HANDLE ProcessHandle;
		ULONG CallbackType;
		union {
			HRESULT Status;
			MINIDUMP_THREAD_CALLBACK Thread;
			MINIDUMP_THREAD_EX_CALLBACK ThreadEx;
			MINIDUMP_MODULE_CALLBACK Module;
			MINIDUMP_INCLUDE_THREAD_CALLBACK IncludeThread;
			MINIDUMP_INCLUDE_MODULE_CALLBACK IncludeModule;
			MINIDUMP_IO_CALLBACK Io;
			MINIDUMP_READ_MEMORY_FAILURE_CALLBACK ReadMemoryFailure;
			ULONG SecondaryFlags;
			MINIDUMP_VM_QUERY_CALLBACK VmQuery;
			MINIDUMP_VM_PRE_READ_CALLBACK VmPreRead;
			MINIDUMP_VM_POST_READ_CALLBACK VmPostRead;
		};
	} MINIDUMP_CALLBACK_INPUT, *PMINIDUMP_CALLBACK_INPUT;

	typedef struct _MINIDUMP_MEMORY_INFO {
		ULONG64 BaseAddress;
		ULONG64 AllocationBase;
		ULONG32 AllocationProtect;
		ULONG32 __alignment1;
		ULONG64 RegionSize;
		ULONG32 State;
		ULONG32 Protect;
		ULONG32 Type;
		ULONG32 __alignment2;
	} MINIDUMP_MEMORY_INFO, *PMINIDUMP_MEMORY_INFO;

	typedef struct _MINIDUMP_CALLBACK_OUTPUT {
		union {
			ULONG ModuleWriteFlags;
			ULONG ThreadWriteFlags;
			ULONG SecondaryFlags;
			struct {
				ULONG64 MemoryBase;
				ULONG MemorySize;
			};
			struct {
				BOOL CheckCancel;
				BOOL Cancel;
			};
			HANDLE Handle;
			struct {
				MINIDUMP_MEMORY_INFO VmRegion;
				BOOL Continue;
			};
			struct {
				HRESULT VmQueryStatus;
				MINIDUMP_MEMORY_INFO VmQueryResult;
			};
			struct {
				HRESULT VmReadStatus;
				ULONG VmReadBytesCompleted;
			};
			HRESULT Status;
		};
	} MINIDUMP_CALLBACK_OUTPUT, *PMINIDUMP_CALLBACK_OUTPUT;


	typedef
		BOOL
		(WINAPI * MINIDUMP_CALLBACK_ROUTINE) (
			_Inout_ PVOID CallbackParam,
			_In_    PMINIDUMP_CALLBACK_INPUT CallbackInput,
			_Inout_ PMINIDUMP_CALLBACK_OUTPUT CallbackOutput
			);

	typedef struct _MINIDUMP_CALLBACK_INFORMATION {
		MINIDUMP_CALLBACK_ROUTINE CallbackRoutine;
		PVOID CallbackParam;
	} MINIDUMP_CALLBACK_INFORMATION, *PMINIDUMP_CALLBACK_INFORMATION;
#endif // WINAPI_HEADERS_H