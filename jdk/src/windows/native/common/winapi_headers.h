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

#ifndef JDK_WINAPI_HEADERS_H
#define JDK_WINAPI_HEADERS_H

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

	// ImageHlp.h

	// As a general principal always call the 64 bit version
	// of every API, if a choice exists.  The 64 bit version
	// works great on 32 bit platforms, and is forward
	// compatible to 64 bit platforms.

	#ifdef _WIN64
	#ifndef _IMAGEHLP64
	#define _IMAGEHLP64
	#endif
	#endif

	// StackWalking API
	//

	typedef enum {
		AddrMode1616,
		AddrMode1632,
		AddrModeReal,
		AddrModeFlat
	} ADDRESS_MODE;

	typedef struct _tagADDRESS64 {
		DWORD64       Offset;
		WORD          Segment;
		ADDRESS_MODE  Mode;
	} ADDRESS64, *LPADDRESS64;

	typedef
		BOOL
		(__stdcall *PREAD_PROCESS_MEMORY_ROUTINE64)(
			_In_ HANDLE hProcess,
			_In_ DWORD64 qwBaseAddress,
			_Out_writes_bytes_(nSize) PVOID lpBuffer,
			_In_ DWORD nSize,
			_Out_ LPDWORD lpNumberOfBytesRead
			);

	typedef
		PVOID
		(__stdcall *PFUNCTION_TABLE_ACCESS_ROUTINE64)(
			_In_ HANDLE ahProcess,
			_In_ DWORD64 AddrBase
			);

	typedef
		DWORD64
		(__stdcall *PGET_MODULE_BASE_ROUTINE64)(
			_In_ HANDLE hProcess,
			_In_ DWORD64 Address
			);


	typedef
		DWORD64
		(__stdcall *PTRANSLATE_ADDRESS_ROUTINE64)(
			_In_ HANDLE hProcess,
			_In_ HANDLE hThread,
			_In_ LPADDRESS64 lpaddr
			);

	//
	// This structure is included in the STACKFRAME structure,
	// and is used to trace through usermode callbacks in a thread's
	// kernel stack.  The values must be copied by the kernel debugger
	// from the DBGKD_GET_VERSION and WAIT_STATE_CHANGE packets.
	//

	//
	// New KDHELP structure for 64 bit system support.
	// This structure is preferred in new code.
	//
	typedef struct _KDHELP64 {

		//
		// address of kernel thread object, as provided in the
		// WAIT_STATE_CHANGE packet.
		//
		DWORD64   Thread;

		//
		// offset in thread object to pointer to the current callback frame
		// in kernel stack.
		//
		DWORD   ThCallbackStack;

		//
		// offset in thread object to pointer to the current callback backing
		// store frame in kernel stack.
		//
		DWORD   ThCallbackBStore;

		//
		// offsets to values in frame:
		//
		// address of next callback frame
		DWORD   NextCallback;

		// address of saved frame pointer (if applicable)
		DWORD   FramePointer;


		//
		// Address of the kernel function that calls out to user mode
		//
		DWORD64   KiCallUserMode;

		//
		// Address of the user mode dispatcher function
		//
		DWORD64   KeUserCallbackDispatcher;

		//
		// Lowest kernel mode address
		//
		DWORD64   SystemRangeStart;

		//
		// Address of the user mode exception dispatcher function.
		// Added in API version 10.
		//
		DWORD64   KiUserExceptionDispatcher;

		//
		// Stack bounds, added in API version 11.
		//
		DWORD64   StackBase;
		DWORD64   StackLimit;

		//
		// Target OS build number. Added in API version 12.
		//
		DWORD     BuildVersion;
		DWORD     Reserved0;
		DWORD64   Reserved1[4];

	} KDHELP64, *PKDHELP64;

	#if !defined(_IMAGEHLP_SOURCE_) && defined(_IMAGEHLP64)
		#define KDHELP KDHELP64
		#define PKDHELP PKDHELP64
	#else
	typedef struct _KDHELP {

		//
		// address of kernel thread object, as provided in the
		// WAIT_STATE_CHANGE packet.
		//
		DWORD   Thread;

		//
		// offset in thread object to pointer to the current callback frame
		// in kernel stack.
		//
		DWORD   ThCallbackStack;

		//
		// offsets to values in frame:
		//
		// address of next callback frame
		DWORD   NextCallback;

		// address of saved frame pointer (if applicable)
		DWORD   FramePointer;

		//
		// Address of the kernel function that calls out to user mode
		//
		DWORD   KiCallUserMode;

		//
		// Address of the user mode dispatcher function
		//
		DWORD   KeUserCallbackDispatcher;

		//
		// Lowest kernel mode address
		//
		DWORD   SystemRangeStart;

		//
		// offset in thread object to pointer to the current callback backing
		// store frame in kernel stack.
		//
		DWORD   ThCallbackBStore;

		//
		// Address of the user mode exception dispatcher function.
		// Added in API version 10.
		//
		DWORD   KiUserExceptionDispatcher;

		//
		// Stack bounds, added in API version 11.
		//
		DWORD   StackBase;
		DWORD   StackLimit;

		DWORD   Reserved[5];

	} KDHELP, *PKDHELP;

	__inline
		void
		KdHelp32To64(
			_In_ PKDHELP p32,
			_Out_ PKDHELP64 p64
		)
	{
		p64->Thread = p32->Thread;
		p64->ThCallbackStack = p32->ThCallbackStack;
		p64->NextCallback = p32->NextCallback;
		p64->FramePointer = p32->FramePointer;
		p64->KiCallUserMode = p32->KiCallUserMode;
		p64->KeUserCallbackDispatcher = p32->KeUserCallbackDispatcher;
		p64->SystemRangeStart = p32->SystemRangeStart;
		p64->KiUserExceptionDispatcher = p32->KiUserExceptionDispatcher;
		p64->StackBase = p32->StackBase;
		p64->StackLimit = p32->StackLimit;
	}
	#endif

	#if !defined(_IMAGEHLP_SOURCE_) && defined(_IMAGEHLP64)
		#define STACKFRAME STACKFRAME64
		#define LPSTACKFRAME LPSTACKFRAME64
	#endif

	typedef struct _tagSTACKFRAME64 {
		ADDRESS64   AddrPC;               // program counter
		ADDRESS64   AddrReturn;           // return address
		ADDRESS64   AddrFrame;            // frame pointer
		ADDRESS64   AddrStack;            // stack pointer
		ADDRESS64   AddrBStore;           // backing store pointer
		PVOID       FuncTableEntry;       // pointer to pdata/fpo or NULL
		DWORD64     Params[4];            // possible arguments to the function
		BOOL        Far;                  // WOW far call
		BOOL        Virtual;              // is this a virtual frame?
		DWORD64     Reserved[3];
		KDHELP64    KdHelp;
	} STACKFRAME64, *LPSTACKFRAME64;

	typedef struct _IMAGEHLP_SYMBOL64 {
		DWORD   SizeOfStruct;           // set to sizeof(IMAGEHLP_SYMBOL64)
		DWORD64 Address;                // virtual address including dll base address
		DWORD   Size;                   // estimated size of symbol, can be zero
		DWORD   Flags;                  // info about the symbols, see the SYMF defines
		DWORD   MaxNameLength;          // maximum size of symbol name in 'Name'
		CHAR    Name[1];                // symbol name (null terminated string)
	} IMAGEHLP_SYMBOL64, *PIMAGEHLP_SYMBOL64;

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
