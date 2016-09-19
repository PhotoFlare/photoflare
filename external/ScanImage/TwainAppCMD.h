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
 * @file TwainAppCMD.h
 * TWAIN CMD Application.
 * A TWAIN Application communicates with the DSM to acquire images. 
 * The goal of the application is to acquire data from a Source.  
 * However, applications cannot contact the Source directly.  All requests for
 * data, capability information, error information, etc. must be handled 
 * Through the Source Manager.
 * @author JFL Peripheral Solutions Inc.
 * @date April 2007
 */

#ifndef __TWAINAPPCMD_H__
#define __TWAINAPPCMD_H__

#pragma once

#ifdef _WINDOWS
  #ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
  #endif
#endif

#include "TwainApp.h"

#ifndef TWH_CMP_MSC
  typedef void* HWND;
#endif

using namespace std;

#ifdef TWNDS_OS_LINUX
  #include <semaphore.h> 
#endif

/**
* The Main Application Class.
*/
class TwainAppCMD : public TwainApp
{
public:
/**
* Constructor for the main application.
* Initialize the application.
* @param[in] parent handle to Window
*/
  TwainAppCMD(HWND parent = NULL);

/**
* Deconstructor for the main application TwainAppCMD.  
* Clean up any alocated memory. 
*/
  ~TwainAppCMD();

/**
* Fills an identity structure with this applications identity details.
* Initialize the Apps identity so the DSM can uniquely identify it.
* @param[out] _identity a structure that will get filled with Applications details
*/
  virtual void fillIdentity(TW_IDENTITY& _identity);

/**
* Override so we can Initilize
* @param[in] _dsID the ID of the datasource to load
*/
  virtual void loadDS(const TW_INT32 _dsID);

/**
* Unloads the currently loaded datasource.  If successful moves the 
* app to state 3.
*/
  virtual void unloadDS();

/**
* Print a list of Data Sources.  Prints to stdout a quick list of the 
* Data Sources the app knows about.
*/
  void printAvailableDataSources();

/**
* Prints an identity structure that uniquely identifies an Application 
* or Data Source out in detail to stdout.
* @param[in] _ident the TW_IDENTITY structure to print
*/
  void printIdentStruct(const TW_IDENTITY& _ident);

/**
* Goes through the list of known identity structures and prints the one
* with an ID that matches the passed in id.
* @param[in] _identityID The ID of the identity structure to print
*/
  void printIdentityStruct(const TW_UINT32 _identityID);

/**
* Initialize default capabilities.
* Negotiates the default capabilities with the currently opened data source.
*/
  void initCaps();
/**
* Free allocated capabilities.
*/
  void uninitCaps();
/**
* Tries to starts the scanning process.  Must be in state 6 to start.  
* If successfully finishes scan moves app to state 5.
*/
  void startScan();

/**
* Sets the CAP_XFERCOUNT capability
* @param[in] _count the value to set
*/
  void set_CAP_XFERCOUNT(const TW_INT16 _count);
  
/**
* Sets the ICAP_UNITS capability
* @param[in] _val the value to set
*/
  void set_ICAP_UNITS(const TW_UINT16 _val);

/**
* Sets the ICAP_PIXELTYPE capability
* @param[in] _pt the value to set
*/
  void set_ICAP_PIXELTYPE(const TW_UINT16 _pt);

/**
* Sets the ICAP_RESOLUTION specified in _Cap
* @param[in] _Cap the RESOLUTION cap to set, either ICAP_XRESOLUTION or ICAP_YRESOLUTION
* @param[in] _pVal the value to set the cap to.
*/
  void set_ICAP_RESOLUTION(const TW_UINT16 _Cap, const pTW_FIX32 _pVal);

/**
* Sets the ICAP_XFERMECH capability
* @param[in] _mech the mechanism to use. (TWSX_NATIVE, TWSX_FILE, TWSX_MEMORY)
*/
  void set_ICAP_XFERMECH(const TW_UINT16 _mech);

/**
* Sets the ICAP_IMAGEFILEFORMAT capability
* @param[in] _fileformat the compression to use. (TWFF_TIFF, TWFF_PICT, TWFF_BMP, TWFF_XBM, TWFF_JFIF, TWFF_FPX, TWFF_TIFFMULTI, TWFF_PNG, TWFF_SPIFF, TWFF_EXIF)
*/
  void set_ICAP_IMAGEFILEFORMAT(const TW_UINT16 _fileformat);

/**
* Sets the ICAP_COMPRESSION capability
* @param[in] _comp the compression to use. (TWCP_NONE, TWCP_PACKBITS, TWCP_GROUP4, TWCP_JPEG, ...)
*/
  void set_ICAP_COMPRESSION(const TW_UINT16 _comp);

/**
* Sets the ICAP_FRAMES capability
* @param[in] _pFrame the frame data to set
*/
  void set_ICAP_FRAMES(const pTW_FRAME _pFrame);

/**
* Sets the ICAP_BITDEPTH capability using a TW_ENUMERATION struct.
* @param[in] _nVal the bit depth to set
*/
  void set_ICAP_BITDEPTH(const TW_UINT16 _nVal);

/**
* Gets the current setting for the ICAP_UNITS cap
* @param[out] _val set to the caps value
* @return true on succes
*/
  bool getICAP_UNITS(TW_UINT16& _val);

/**
* Gets the current setting for the CAP_XFERCOUNT cap
* @param[out] _val set to the caps value
* @return true on succes
*/
  bool getCAP_XFERCOUNT(TW_INT16& _val);

/**
* Gets the current setting for the ICAP_XFERMECH cap
* @param[out] _val a TW_UINT16 holding the current transfer method (TWSX_NATIVE, etc...)
* @return true on succes
*/
  bool getICAP_XFERMECH(TW_UINT16& _val);

/**
* Gets the current setting for the ICAP_IMAGEFILEFORMAT cap
* @param[out] _val a TW_UINT16 holding the current compression method (TWFF_TIFF, TWFF_PICT, TWFF_BMP, TWFF_XBM, TWFF_JFIF, TWFF_FPX, TWFF_TIFFMULTI, TWFF_PNG, TWFF_SPIFF, TWFF_EXIF)
* @return true on succes
*/
  bool getICAP_IMAGEFILEFORMAT(TW_UINT16& _val);

/**
* Gets the current setting for the ICAP_COMPRESSION cap
* @param[out] _val a TW_UINT16 holding the current compression method (TWCP_NONE, TWCP_PACKBITS, TWCP_GROUP4, TWCP_JPEG, ...)
* @return true on succes
*/
  bool getICAP_COMPRESSION(TW_UINT16& _val);

/**
* Gets the current setting for the ICAP_PIXELTYPE cap
* @param[out] _val a TW_UINT16 holding the current pixel type (TWPT_BW, etc...)
* @return true on succes
*/
  bool getICAP_PIXELTYPE(TW_UINT16& _val);

/**
* Gets the current setting for the ICAP_BITDEPTH cap
* @param[out] _val a TW_UINT16 holding the current bit depth
* @return true on succes
*/
  bool getICAP_BITDEPTH(TW_UINT16& _val);

/**
* Gets the current setting for the ICAP_XRESOLUTION cap
* @param[out] _xres a TW_FIX32 holding the current x resolution
* @return true on succes
*/
  bool getICAP_XRESOLUTION(TW_FIX32& _xres);

/**
* Gets the current setting for the ICAP_YRESOLUTION cap
* @param[out] _yres a TW_FIX32 holding the current y resolution
* @return true on succes
*/
  bool getICAP_YRESOLUTION(TW_FIX32& _yres);


  ////////////////
  // CAPABILITIES

  TW_CAPABILITY   m_CAP_XFERCOUNT;        /**< Number of images the application is willing to accept this session. */
  TW_CAPABILITY   m_ICAP_XFERMECH;        /**< Transfer mechanism - used to learn options and set-up for upcoming transfer. */
  TW_CAPABILITY   m_ICAP_IMAGEFILEFORMAT; /**< File format saved when using File Xfer Mechanism. */
  TW_CAPABILITY   m_ICAP_COMPRESSION;     /**< Compression method used for upcoming transfer. */
  TW_CAPABILITY   m_ICAP_UNITS;           /**< Unit of measure (inches, centimeters, etc). */
  TW_CAPABILITY   m_ICAP_PIXELTYPE;       /**< The type of pixel data (B/W, gray, color, etc). */
  TW_CAPABILITY   m_ICAP_BITDEPTH;        /**< Pixel bit depth for Current value of ICAP_PIXELTYPE. */
  TW_CAPABILITY   m_ICAP_XRESOLUTION;     /**< Current/Available optical resolutions for x-axis. */
  TW_CAPABILITY   m_ICAP_YRESOLUTION;     /**< Current/Available optical resolutions for y-axis */
  TW_CAPABILITY   m_ICAP_FRAMES;          /**< Size and location of frames on page. */
  
#ifdef TWNDS_OS_LINUX
  sem_t m_TwainEvent;
#endif

protected:
};

#endif //  __TWAINAPPCMD_H__
