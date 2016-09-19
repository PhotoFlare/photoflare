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
 * @file TwainApp.h
 * TWAIN Application.
 * A TWAIN Application communicates with the DSM to acquire images. 
 * The goal of the application is to acquire data from a Source.  
 * However, applications cannot contact the Source directly.  All requests for
 * data, capability information, error information, etc. must be handled 
 * Through the Source Manager.
 * @author JFL Peripheral Solutions Inc.
 * @date April 2007
 */

/*! \mainpage TWAIN Application
 *
 * The goal of the application is to acquire data from a Source.  
 * However, applications cannot contact the Source directly.  All requests for
 * data, capability information, error information, etc. must be handled 
 * Through the Source Manager.
 *
 *
 *
 *
 *
 *
 *
 *
 * Copyright © 2007 TWAIN Working Group:  Adobe Systems Incorporated,
 * AnyDoc Software Inc., Eastman Kodak Company, 
 * Fujitsu Computer Products of America, JFL Peripheral Solutions Inc., 
 * Ricoh Corporation, and Xerox Corporation.
 * All rights reserved.
*/
#ifndef __TWAINAPP_H__
#define __TWAINAPP_H__

#pragma once

#ifdef _WINDOWS
  #ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
  #endif
#endif

#include <string>
#include <vector>

#include "CommonTWAIN.h"
#include "FreeImage.h"
#include "DSMInterface.h"

#ifndef TWH_CMP_MSC
  typedef void* HWND;
#endif

using namespace std;

/**
* Determine if two TW_FIX32 variables are equal.
* @param[in] _fix1 First TW_FIX32
* @param[in] _fix2 Second TW_FIX32
* @return bool TRUE if equal
*/
bool operator== (const TW_FIX32& _fix1, const TW_FIX32& _fix2);

/**
* Output CMD messages.  In CMD app used for prompt.  In GUI app used by debugger
* @param[in] pStr the message to display
* @param[in] ... additional varibles
*/
void PrintCMDMessage(const char* const pStr, ...);

/**
* The Main Application Class.
*/
class TwainApp
{
public:
/**
* Constructor for the main application.
* Initialize the application.
* @param[in] parent handle to Window
*/
  TwainApp(HWND parent = NULL);

/**
* Deconstructor for the main application TwainApp.  
* Clean up any alocated memory. 
*/
  ~TwainApp();


  TW_UINT16 DSM_Entry(TW_UINT32    _DG,
                      TW_UINT16    _DAT,
                      TW_UINT16    _MSG,
                      TW_MEMREF    _pData);
/**
* Print out error message and condition code from DS.  Looks up the 
* condition code using the _pdestID and prints it after the _errorMsg.
* If _errorMsg is empty, a default message is displayed.
* @param[in] _pdestID the destination to get the condition code. 0 is DSM
*            anything else is a source.
* @param[in] _errorMsg the error message to display. If empty a standard message
*            is displayed
* @return a valid TWCC_xxxx condition code
*/
  TW_INT16 printError(pTW_IDENTITY _pdestID = 0, const string& _errorMsg = "");

/**
* Retrieves the condition code from the DSM.
* @param[in] _pdestID a pointer to the source to query, or 0 for the DSM
* @param[out] _cc this will be set to the condition code
* @return a valid TWRC_xxxx return code
*/
  TW_UINT16 getTWCC(pTW_IDENTITY _pdestID, TW_INT16& _cc);

/**
* Fills an identity structure with this applications identity details.
* Initialize the Apps identity so the DSM can uniquely identify it.
* @param[out] _identity a structure that will get filled with Applications details
*/
  virtual void fillIdentity(TW_IDENTITY& _identity);

/**
* Exit application clean up everything.
*/
  void exit();

/**
* Try to connect to the DSM.  Set the App to correct state.
* m_DSMState is set to state 3 if successful.
*/
  void connectDSM();

/**
* Try to disconnect from the DSM.  Set the App to correct state.
* m_DSMState is set to state 2 if successful.
*/
  void disconnectDSM();

/**
* Queries the DSM for all available Data Sources.  Discover all Data 
* Sources available and build a list of DS and store them in m_DataSources.
*/
  void getSources();

/**
* Try to load a Data Source.  Loads the data source with the ID passed in. 
* To open a Data Source the DSM must be in state 3 (Open).  If successful 
* moves the app to state 4
* @param[in] _dsID the ID of the datasource to load, -1 to open the default
*/
  virtual void loadDS(const TW_INT32 _dsID);

/**
* Unloads the currently loaded datasource.  If successful moves the 
* app to state 3.
*/
  virtual void unloadDS();

/**
* Request the acquisition of data from the currently opened source.  If 
* successful moves app to state 5.
* @param[in] hWnd the handle to the window
* @param[in] bShowUI true to show UI false to enable without UI
* @return true if successfully enabled DS.
*/
  virtual bool enableDS(TW_HANDLE hWnd, BOOL bShowUI);

/**
* Show the DS GUI so the user can modify current settings.
* successful moves app to state 5.
* @param[in] hWnd the handle to the window
* @return true if successfully enabled DS.
*/
  virtual bool enableDSUIOnly(TW_HANDLE hWnd);

/**
* Tries to disable the sources UI.  If successful moves app to state 4
*/
  virtual void disableDS();

/**
* Starts the scanning process using native mode.  Data will be transfered 
* in native mode.
*/
  void initiateTransfer_Native();

/**
* Starts the scanning process using file mode.  Data will be transfered 
* via a file.
*/
  void initiateTransfer_File(TW_UINT16 fileformat = TWFF_TIFF);

/**
* Starts the scanning process using the memory strip mode.  Data will be transfered 
* in memory using a buffer.
*/
  void initiateTransfer_Memory();

/**
* Try to sets a OneValue Capability of any of the TW_INT or TW_UINT types to the value passed
* @param[in] Cap the capability to update to set
* @param[in] _value the value to set
* @param[in] _type the TWAIN Type to set
* @return a valid TWRC_xxxx return code.
*/
  TW_UINT16 set_CapabilityOneValue(TW_UINT16 Cap, const int _value, TW_UINT16 _type);

/**
* Try to sets a OneValue Capability of type TW_FIX32 to the value passed
* @param[in] Cap the capability to update to set
* @param[in] _pValue the value to set
* @return a valid TWRC_xxxx return code.
*/
  TW_UINT16 set_CapabilityOneValue(TW_UINT16 Cap, const pTW_FIX32 _pValue);

/**
* Try to sets a OneValue Capability of type TW_FRAME to the value passed
* @param[in] Cap the capability to update to set
* @param[in] _pValue the value to set
* @return a valid TWRC_xxxx return code.
*/
  TW_UINT16 set_CapabilityOneValue(TW_UINT16 Cap, const pTW_FRAME _pValue);

/**
* Try to sets an Array Capability of any of the TW_INT or TW_UINT types to the values passed
* @param[in] Cap the capability to update to set
* @param[in] _pValues pointer to the array of values to set
* @param[in] Count the number of items in the array
* @param[in] -type the type of items to set
* @return a valid TWRC_xxxx return code.
*/
  TW_UINT16 set_CapabilityArray(TW_UINT16 Cap, const int * _pValues, int Count, TW_UINT16 _type);

/**
* Gets the capability.
* @param[in,out] _cap the capability struct to fill. _cap.Cap needs to be
* @param[in] _msg one of MSG_GET, MSG_GETCURRENT, MSG_GETDEFAULT, or MSG_RESET
* filled with the capability to retrieve.
* @return a valid TWCC_xxxx condition code
*/
  TW_INT16 get_CAP(TW_CAPABILITY& _cap, TW_UINT16 _msg = MSG_GET);

/**
* Query Support messages of the capability.
* @param[in] _cap the capability id to lookup.
* @param[out] _QS the Query Support of the cap
* @return a valid TWRC_xxxx return code
*/
  TW_INT16 QuerySupport_CAP(TW_UINT16 _cap, TW_UINT32 &_QS);

/**
* Get the lable for a capability.
* @param[in] _cap the capability id to lookup.
* @param[out] sLable the Lable if returned
* @return a valid TWRC_xxxx return code
*/
  TW_INT16 GetLabel(TW_UINT16 _cap, string &sLable);

/**
* Get the help or description for a capability.
* @param[in] _cap the capability id to lookup.
* @param[out] sDescription the Description or help if returned
* @return a valid TWRC_xxxx return code
*/
  TW_INT16 GetHelp(TW_UINT16 _cap, string &sHelp);

/**
* Returns a pointer to the applications identity structure.
* @return a TW_IDENTITY pointer to the applications identity struct.
*/
  pTW_IDENTITY getAppIdentity();

/**
* Returns a pointer to the currently opened data source.
* @param[in] _index the DS index to return
* @return a TW_IDENTITY pointer to the opened data source 
* if nothing open then return NULL.
*/
  pTW_IDENTITY getDataSource(TW_INT16 _index = -1) const;

/**
* Returns a pointer to the default data source.
* @return a TW_IDENTITY pointer to the default.
*/
  pTW_IDENTITY getDefaultDataSource(void);

/**
* Set the default data source.
* @param[in] _index the DS index to set as default
* @return a TW_IDENTITY pointer of the default.
*/
  pTW_IDENTITY setDefaultDataSource(unsigned int _index);

/**
* Display the Select Source Dialog and returns a pointer to the selected data source.
* @return a TW_IDENTITY pointer.  NULL if no source selected.
*/
  pTW_IDENTITY selectDefaultDataSource(void);

/**
* Gets the imageinfo of the currently transfered image
* @return pointer to the image info
*/
  pTW_IMAGEINFO getIMAGEINFO() {return &m_ImageInfo;}

/**
* Gets the extended image info of the currently transfered image
* @return pointer to the extended image info
*/
  string getEXIMAGEINFO(){return m_strExImageInfo;}

/**
* retrieve the imageinfo for the current image
* @return true on succes
*/
  bool updateIMAGEINFO();

/**
* retrieve the extended image info for the current image
* @return true on succes
*/
  virtual void updateEXTIMAGEINFO();

  int             m_DSMState;             /**< The current TWAIN state of the dsm (2-7) */
  TW_UINT16       m_DSMessage;            /**< Statis to indicate if we are waiting for DS */

protected:
/**
* Create a DIB of the dimention set by m_ImageInfo and bitdepth of 
* data source.
* @return the DIB created on succes or 0 if failed.
*/
//  FIBITMAP* createDIB();

  HWND                m_Parent;               /**< Handle to Window to recieve window messages. */
  TW_IDENTITY         m_MyInfo;               /**< Identity structure with this applications identity details */
  pTW_IDENTITY        m_pDataSource;          /**< Pointer of Identity structure to current loaded data souce */
  vector<TW_IDENTITY> m_DataSources;          /**< Store a list of available data sources located by DSM */

  TW_IMAGEINFO        m_ImageInfo;            /**< Detailed image info retrieved from the DS */
  pTW_EXTIMAGEINFO    m_pExtImageInfo;        /**< Detailed extended image info retrieved from the DS */
  TW_UINT16           m_nXferNum;             /**< Keep track of the number of images attempted to transfer - used when saving file to disk*/
  string              m_strExImageInfo;       /**< Extended image info retrieved from the DS */
  TW_UINT16           m_nGetLableSupported;   /**< Remember if MSG_GETLABLE is supported */
  TW_UINT16           m_nGetHelpSupported;    /**< Remember if MSG_GETHELP is supported */
  string              m_strSavePath;          /**< The Path to save the files to */

  TW_USERINTERFACE    m_ui;                   /**< Coordinates UI between application and data source. */
};

#endif //  __TWAINAPP_H__
