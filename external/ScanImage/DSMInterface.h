/***************************************************************************
* Copyright © 2007 TWAIN Working Group:  
*   Adobe Systems Incorporated, AnyDoc Software Inc., Eastman Kodak Company, 
*   Fujitsu Computer Products of America, JFL Peripheral Solutions Inc., 
*   Ricoh Corporation, and Xerox Corporation.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the TWAIN Working Group nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY TWAIN Working Group ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL TWAIN Working Group BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
***************************************************************************/

/**
* @file DSMInterface.h
* Common defines and typedefs used for accessing DSM for TWAIN Applications.
* @author TWAIN Working Group
* @date April 2007
*/

#ifndef __DSMINTERFACE_H__
#define __DSMINTERFACE_H__

#pragma once

#ifdef _WINDOWS
  #ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
  #endif
#endif

//#include "main.h"
#include "twain.h"

/**
* Load the DSM library.
* @param[in] the name of the DSM library to open
* @return true if success.
*/
bool LoadDSMLib(char* _pszLibName);

/**
* Unload the DSM library.
*/
void unLoadDSMLib();

/**
* Initialize and register the entry point into the DSM.
* @param[in] _pOrigin Identifies the source module of the message. This could
*           identify an Application, a Source, or the Source Manager.
*
* @param[in] _pDest Identifies the destination module for the message.
*           This could identify an application or a data source.
*           If this is NULL, the message goes to the Source Manager.
*
* @param[in] _DG The Data Group. 
*           Example: DG_IMAGE.
*
* @param[in] _DAT The Data Attribute Type.
*           Example: DAT_IMAGEMEMXFER.
*    
* @param[in] _MSG The message.  Messages are interpreted by the destination module
*           with respect to the Data Group and the Data Attribute Type.  
*           Example: MSG_GET.
*
* @param[in,out] _pData A pointer to the data structure or variable identified 
*           by the Data Attribute Type.
*           Example: (TW_MEMREF)&ImageMemXfer
*                   where ImageMemXfer is a TW_IMAGEMEMXFER structure.
*                    
* @return a valid TWRC_xxxx return code.
*          Example: TWRC_SUCCESS.
*/
TW_UINT16 _DSM_Entry( pTW_IDENTITY _pOrigin,
                      pTW_IDENTITY _pDest,
                      TW_UINT32    _DG,
                      TW_UINT16    _DAT,
                      TW_UINT16    _MSG,
                      TW_MEMREF    _pData);

/**
* Allocate global memory
* @param[in] _size of memory to allocate.
* @return TW_HANDLE to the memory allocated.
*/
TW_HANDLE _DSM_Alloc(TW_UINT32 _size);

/**
* Free previously allocated global memory
* @param[in] _hMemory TW_HANDLE to the memory needing free.
*/
void _DSM_Free(TW_HANDLE _hMemory);

/**
* Lock global memory from being updated by others. return a pointer to the 
* memory so we can update it.
* @param[in] _hMemory TW_HANDLE to the memory to update.
* @return TW_MEMREF pointer to the memory.
*/
TW_MEMREF _DSM_LockMemory(TW_HANDLE _hMemory);

/**
* Unlock global memory after locking. to allow updating by others.
* @param[in] _hMemory TW_HANDLE to memory returned by _DSM_Alloc
*/
void _DSM_UnlockMemory(TW_HANDLE _hMemory);

/**
* Not Used.
* @todo Not Implimented
* @param[in] twEvent reference to the TW_EVENT
* @return TW_UINT16
*/
TW_UINT16 HandleEvent(TW_EVENT &twEvent);

#endif // __DSMINTERFACE_H__
