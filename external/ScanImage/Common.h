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
* @file Common.h
* Common defines and typedefs used by the DS, App, and scanner
* @author TWAIN Working Group
* @date April 2007
*/

#ifndef __COMMON_H__
#define __COMMON_H__

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
  #define TWNDS_OS_WIN
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
  #endif
  #include <windows.h>
#endif

#include "twain.h"

#ifdef TWH_CMP_GNU
  #ifndef TWNDS_OS_WIN
    #define TWNDS_OS_LINUX
  #endif
  #include <wchar.h>
  #include <stdarg.h>
#endif

/**
* These headers are available on all platforms...
*/
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <assert.h>

/**
* First off, figure out what compiler we're running and on which
* platform we think we're running it.  We assume that you're building
* on the same platform you intend to run, so if you are cross compiling
* you will likely have a bit of work to do here...
*/

/**
* Compilers we support...
*/
#define TWNDS_CMP_VISUALCPP     0x1001 // Preferably 2005+
#define TWNDS_CMP_GNUGPP        0x1002 // Preferably v4.x+

/**
* If the user defines TWNDS_CMP in their make file or project,
* then we'll assume they want to take responsibility for picking
* how we'll build the system.  At this point it seems like the
* compiler definition is used to select which native library calls
* we're dealing with, while the os definition is more about
* where we'll expect to find stuff on the running system, like
* directories...
*/
#ifndef TWNDS_CMP

  // GNU g++
  #if defined(TWH_CMP_GNU)
    #define TWNDS_CMP             TWNDS_CMP_GNUGPP
    #define TWNDS_CMP_VERSION     __GNUC__

    #define kTWAIN_DSM_DIR         "/usr/local/lib/"

  // Visual Studio C++
  #elif defined(TWH_CMP_MSC)
    #define TWNDS_CMP              TWNDS_CMP_VISUALCPP
    #define TWNDS_CMP_VERSION      _MSC_VER
    // Not neccessary it is in Windows path
    #define kTWAIN_DSM_DIR         ""
  // ruh-roh...
  #else
    #error Sorry, we don't recognize this system...
  #endif
#endif


/**
* @def LOADLIBRARY(lib)
* Call system loadibrary function.  OS abstraction macro that tries to load a library.
* @param lib path and name of library
* 
* @def LOADFUNCTION(lib, func)
* Call system GetProcAddress function.  OS abstraction macro that tries to locate the addess of a funtion name.
* @param lib path and name of library
* @param func name of the funtion
*
* @def UNLOADLIBRARY(lib)
* Call system FreeLibrary function.  OS abstraction macro that tries to release the library.
* @param lib library modual to unload
* 
* @def UNLINK
* OS abstraction macro that calls system _unlink function.
*
* @def READ
* OS abstraction macro that calls system _read function.
* 
* @def CLOSE
* OS abstraction macro that calls system _close function.
* 
* @def SNPRINTF
* OS abstraction macro that calls system _snprintf function.
* 
*/

#if (TWNDS_CMP == TWNDS_CMP_VISUALCPP)
  //#include "stdafx.h"
  #define DllExport   __declspec( dllexport )
  #define LOADLIBRARY(lib) LoadLibrary(lib) 
  #define LOADFUNCTION(lib, func) GetProcAddress(lib, func)
  #define UNLOADLIBRARY(lib) FreeLibrary(lib)
  #define UNLINK _unlink
  #define READ _read
  #define CLOSE _close
  #define FILE_EXISTS(FILE_NAME) ((0xFFFFFFFF==GetFileAttributes(FILE_NAME))?FALSE:TRUE)
  #define PATH_SEPERATOR '\\'
  #ifndef PATH_MAX
    #define PATH_MAX _MAX_PATH
  #endif

  #if (TWNDS_CMP_VERSION >= 1400)
    #define SNPRINTF _snprintf_s
    #define SSCANF sscanf_s
    #define FOPEN(pf,name,mode) (void)fopen_s(&pf,name,mode)
    #define _OPEN(pf,name,mode,share,perm) (void)_sopen_s(&pf,name,mode,share,perm)
  #else
    #define SSCANF sscanf
    #define SNPRINTF _snprintf
    #define FOPEN(pf,name,mode) pf=fopen(name,mode)
    #define _OPEN(pf,name,mode,share,perm) pf = _open(name,mode,share)
  #endif
  #define MAX(a, b) max(a,b)
  #define MIN(a, b) min(a,b)

#elif  (TWNDS_CMP == TWNDS_CMP_GNUGPP)
  #define DllExport
  #define LOADLIBRARY(lib) dlopen(lib, RTLD_NOW)
  #define LOADFUNCTION(lib, func) dlsym(lib, func)
  #define UNLOADLIBRARY(lib) dlclose(lib)
  #define UNLINK unlink
  #define kTWAIN_DSM_DLL_NAME "libtwaindsm.so"
  #define READ read
  #define CLOSE close
  #define PATH_SEPERATOR '/'
  #define SNPRINTF snprintf
  #define SSCANF sscanf
  typedef void * HMODULE;
  #define MAX(a, b)  (((a) > (b)) ? (a) : (b))
  #define MIN(a, b)  (((a) < (b)) ? (a) : (b))
  #define FILE_EXISTS(FILE_NAME) ((0 == access(FILE_NAME, R_OK))?TRUE:FALSE)
  #define FOPEN(pf,name,mode) (pf=fopen(name,mode))

  #if !defined(TRUE)
  	#define FALSE		0
  	#define TRUE		1
  #endif

  #include <inttypes.h>
  typedef uint16_t WORD;
  typedef uint32_t DWORD;

#else
  #error Sorry, we don't recognize this system...
#endif


/**
* We want to use secure string functions whenever possible, if g++
* every includes a set I think it would be excellent to switch over
* to it, but at least with Windows using them we stand a better
* chance of finding boo-boos...
*/
#if (TWNDS_CMP == TWNDS_CMP_VISUALCPP) && (TWNDS_CMP_VERSION >= 1400)
  #define SSTRCPY(d,z,s) strncpy_s(d,z,s,_TRUNCATE)
  #define SSTRCAT(d,z,s) strncat_s(d,z,s,_TRUNCATE)
  #define SSTRNCPY(d,z,s,m) strncpy_s(d,z,s,m)
  #define SGETENV(d,z,n) ::GetEnvironmentVariable(n,d,z)
  inline int SSNPRINTF(char *d, size_t z, size_t c, const char *f,...)
  {
      int result;
      va_list valist;
      va_start(valist,f);
      result = _vsnprintf_s(d,z,c,f,valist);
      va_end(valist);
      return result;
  }

/**
* These functions are insecure, but everybody has them, so we
* don't need an else/error section like we use everywhere else...
*/
#else
  #define SSTRCPY(d,z,s) strcpy(d,s)
  #define SSTRCAT(d,z,s) strcat(d,s)
  #define SSTRNCPY(d,z,s,m) strncpy(d,s,m)
  #define SGETENV(d,z,n) strcpy(d,getenv(n)?getenv(n):"")
  inline int SSNPRINTF(char *d, size_t, size_t c, const char *f,...)
  {
      int result;
      va_list valist;
      va_start(valist,f);
      #if (TWNDS_CMP == TWNDS_CMP_VISUALCPP)
        result = _vsnprintf(d,c,f,valist);
      #elif (TWNDS_CMP == TWNDS_CMP_GNUGPP)
        result = vsnprintf(d,c,f,valist);
      #else
        #error Sorry, we don't recognize this system...
      #endif
      va_end(valist);
      return result;
  }
#endif

/**
* Determine the number of bytes needed for one line.
*/
#define BYTES_PERLINE(width, bpp) ((((int)(width)*(bpp))+7)/8)

/**
* Determine the number of bytes needed rouned up to 4 byte alignment.
*/
#define BYTES_PERLINE_ALIGN4(width, bpp) (((((int)(width)*(bpp))+31)/32)*4)


#endif // __COMMON_H__

