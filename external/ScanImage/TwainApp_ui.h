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
 * @file TwainApp_ui.h
 * User interface for TWAIN Application
 * @author JFL Peripheral Solutions Inc.
 * @date April 2007
 */


#ifndef __TWAINAPP_UI_H__
#define __TWAINAPP_UI_H__

#pragma once

#ifdef _WINDOWS
  #ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
  #endif
#endif

#include "main.h"
#include "twain.h"
#include "CommonTWAIN.h"

/**
* Returns an error message that can be used when a type is of an unexpected type.
* @param[in] _unExpected the expected type
* @param[in] _unReceived the received type
* @return the error string
*/
string getErrorString_UnexpectedType(const TW_UINT16 _unExpected, const TW_UINT16 _unReceived);

/**
* prints the main menu
*/
void printOptions();

/**
* prints the main capabilities menu
*/
void printMainCaps();

/**
* prints the ICAP_xxxxx menu for type TW_ONEVALUE
* @param[in] _unCap The cap to print
* @param[in] _pCap pointer to the TW_ONEVALUE returned by the source for this cap
*/
void print_ICAP(const TW_UINT16 _unCap, pTW_ONEVALUE _pCap);

/**
* @overload void print_ICAP(const TW_UINT16 _unCap, pTW_ENUMERATION _pCap)
* prints the ICAP_xxxxx menu for type TW_ENUMERATION
* @param[in] _unCap The cap to print
* @param[in] _pCap pointer to the TW_ENUMERATION returned by the source for this cap
*/
void print_ICAP(const TW_UINT16 _unCap, pTW_ENUMERATION _pCap);

#endif // __TWAINAPP_UI_H__
