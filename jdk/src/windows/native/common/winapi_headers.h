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

#endif // WINAPI_HEADERS_H
