// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

/*++



Module Name:

    include/pal/shmemory.h

Abstract:
    Header file for interface to shared memory

How to use :

The SHMalloc function can be used to allocate memory in the shared memory area.
It returns a value of type SHMPTR, which will be useable in all participating
processes. The SHMPtrToPtr can be used to convert a SHMPTR value into
an address. In addition, Lock/Release functions must be
used when manipulating data in shared memory, to ensure inter-process synchronization.



--*/

#ifndef _PAL_SHMEMORY_H_
#define _PAL_SHMEMORY_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
Type for shared memory blocks. use SHMPtrToPtr to get a useable address.
 */
typedef DWORD_PTR SHMPTR;

typedef enum {
    SIID_NAMED_OBJECTS,
    SIID_FILE_LOCKS,

    SIID_LAST
} SHM_INFO_ID;

/*++
SHMPtrToPtr

Convert a SHMPTR value into a useable pointer.
--*/
LPVOID SHMPtrToPtr(SHMPTR shmptr);

/*++
SHMInitialize

Hook this process into the PAL shared memory system; initialize the shared
memory if no other process has done it.
--*/
BOOL SHMInitialize(void);

/*++
SHMCleanup

Release all shared memory resources held; remove ourselves from the list of
registered processes, and remove all shared memory files if no process remains
--*/
void SHMCleanup(void);

/*++
SHMalloc

Allocate a block of memory of the specified size

Parameters :
    size_t size : size of block required

Return value :
    A SHMPTR identifying the new block, or 0 on failure. Use SHMPtrToPtr to
    convert a SHMPTR into a useable pointer
--*/
SHMPTR SHMalloc(size_t size);

/*++
SHMfree

Release a block of shared memory

Parameters :
    SHMPTR shmptr : pointer to block of memory to release

(no return value)
--*/
void SHMfree(SHMPTR shmptr);

/*++
SHMLock

Restrict shared memory access to the current thread of the current process

(no parameters)

Return value :
    New lock count
--*/
int SHMLock(void);

/*++
SHMRelease

Release a lock on shared memory taken with SHMLock.

(no parameters)

Return value :
    New lock count
--*/
int SHMRelease(void);


/*++
Function :
    SHMGetInfo

    Retrieve some information from shared memory

Parameters :
    SHM_INFO_ID element : identifier of element to retrieve

Return value :
    Value of specified element

Notes :
    The SHM lock should be held while manipulating shared memory
--*/
SHMPTR SHMGetInfo(SHM_INFO_ID element);

/*++
Function :
    SHMSetInfo

    Place some information into shared memory

Parameters :
    SHM_INFO_ID element : identifier of element to save
    SHMPTR value : new value of element

Return value :
    TRUE if successfull, FALSE otherwise.

Notes :
    The SHM lock should be held while manipulating shared memory
--*/
BOOL SHMSetInfo(SHM_INFO_ID element, SHMPTR value);


/********************** Shared memory help functions ********************/

/*++
SHMStrDup

Duplicates the string in shared memory.

Returns the new address as SHMPTR on success.
Returns (SHMPTR)NULL on failure.
--*/
SHMPTR SHMStrDup( LPCSTR string );

/*++
SHMWStrDup

Duplicates the wide string in shared memory.

Returns the new address as SHMPTR on success.
Returns (SHMPTR)NULL on failure.
--*/
SHMPTR SHMWStrDup( LPCWSTR string );

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* _PAL_SHMEMORY_H_ */

