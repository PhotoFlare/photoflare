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
 * @file TwainAppCMD.cpp
 * TWAIN Application.
 * A TWAIN Application communicates with the DSM to acquire images. 
 * The goal of the application is to acquire data from a Source.  
 * However, applications cannot contact the Source directly.  All requests for
 * data, capability information, error information, etc. must be handled 
 * Through the Source Manager.
 * @author JFL Peripheral Solutions Inc.
 * @date April 2007
 */

#ifdef _WINDOWS
  #include "stdafx.h"
#endif
#include "main.h"

#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <stdio.h>

#include "TwainAppCMD.h"
//#include "CTiffWriter.h"
#include "TwainString.h"

using namespace std;


//////////////////////////////////////////////////////////////////////////////
TwainAppCMD::TwainAppCMD(HWND parent /*=NULL*/):TwainApp(parent)
{
#ifdef TWNDS_OS_LINUX
// Initialize event semaphore
  sem_init(&m_TwainEvent,   // handle to the event semaphore
           0,     // not shared
           0);    // initially set to non signaled state
#endif
}

//////////////////////////////////////////////////////////////////////////////
TwainAppCMD::~TwainAppCMD()
{
#ifdef TWNDS_OS_LINUX
// now destroy the event semaphore
  sem_destroy(&m_TwainEvent);   // Event semaphore handle
#endif
}

//////////////////////////////////////////////////////////////////////////////
void TwainAppCMD::fillIdentity(TW_IDENTITY& _identity)
{
  TwainApp::fillIdentity(_identity);

  SSTRCPY(_identity.Manufacturer, sizeof(_identity.Manufacturer), "TWAIN Working Group");
  SSTRCPY(_identity.ProductFamily, sizeof(_identity.ProductFamily), "Sample TWAIN App");
  SSTRCPY(_identity.ProductName, sizeof(_identity.ProductName), "Command line TWAIN app");

  return;
}

//////////////////////////////////////////////////////////////////////////////
void TwainAppCMD::printIdentStruct(const TW_IDENTITY& _ident)
{
  PrintCMDMessage("\n Id: %u\n", _ident.Id);
  PrintCMDMessage("Version: %u.%u\n", _ident.Version.MajorNum, _ident.Version.MinorNum);
  PrintCMDMessage("SupportedGroups: %u\n", _ident.SupportedGroups);
  PrintCMDMessage("Manufacturer: %s\n", _ident.Manufacturer);
  PrintCMDMessage("ProductFamily: %s\n", _ident.ProductFamily);
  PrintCMDMessage("ProductName: %s\n", _ident.ProductName);

  return;
}

//////////////////////////////////////////////////////////////////////////////
void TwainAppCMD::printIdentityStruct(const TW_UINT32 _identityID)
{
  for(unsigned int x = 0; x < m_DataSources.size(); ++x)
  {
    if(_identityID == m_DataSources[x].Id)
    {
      printIdentStruct(m_DataSources[x]);
      break;
    }
  }

  return;
}

//////////////////////////////////////////////////////////////////////////////
void TwainAppCMD::printAvailableDataSources()
{
  if(m_DSMState < 3)
  {
    PrintCMDMessage("The DSM has not been opened yet, please open it first\n");
    return;
  }

  // print the Id and name of each available source
  for(unsigned int x = 0; x < m_DataSources.size(); ++x)
  {
    PrintCMDMessage("%d: %.33s by %.33s\n", m_DataSources[x].Id, m_DataSources[x].ProductName, m_DataSources[x].Manufacturer);
  }

  return;
}

//////////////////////////////////////////////////////////////////////////////
void TwainAppCMD::loadDS(const TW_INT32 _dsID)
{
  TwainApp::loadDS(_dsID);

  if(m_DSMState == 4)
  {
     initCaps();
  }
}
//////////////////////////////////////////////////////////////////////////////
void TwainAppCMD::unloadDS()
{
  uninitCaps();

  TwainApp::unloadDS();
}
//////////////////////////////////////////////////////////////////////////////
void TwainAppCMD::initCaps()
{
  if(m_DSMState < 3)
  {
    PrintCMDMessage("The DSM needs to be opened first.\n");
    return;
  }
  else if(m_DSMState < 4)
  {
    PrintCMDMessage("A Data Source needs to be opened first.\n");
    return;
  }

  // get the default pixel type
  memset(&m_CAP_XFERCOUNT,0,sizeof(TW_CAPABILITY));
  m_CAP_XFERCOUNT.Cap = CAP_XFERCOUNT;
  get_CAP(m_CAP_XFERCOUNT);

  memset(&m_ICAP_PIXELTYPE,0,sizeof(TW_CAPABILITY));
  m_ICAP_PIXELTYPE.Cap = ICAP_PIXELTYPE;
  get_CAP(m_ICAP_PIXELTYPE);

  memset(&m_ICAP_XFERMECH,0,sizeof(TW_CAPABILITY));
  m_ICAP_XFERMECH.Cap = ICAP_XFERMECH;
  get_CAP(m_ICAP_XFERMECH);
  
  memset(&m_ICAP_IMAGEFILEFORMAT,0,sizeof(TW_CAPABILITY));
  m_ICAP_IMAGEFILEFORMAT.Cap = ICAP_IMAGEFILEFORMAT;
  get_CAP(m_ICAP_IMAGEFILEFORMAT);
    
  memset(&m_ICAP_COMPRESSION,0,sizeof(TW_CAPABILITY));
  m_ICAP_COMPRESSION.Cap = ICAP_COMPRESSION;
  get_CAP(m_ICAP_COMPRESSION);
    
  memset(&m_ICAP_UNITS,0,sizeof(TW_CAPABILITY));
  m_ICAP_UNITS.Cap = ICAP_UNITS;
  get_CAP(m_ICAP_UNITS);

  memset(&m_ICAP_BITDEPTH,0,sizeof(TW_CAPABILITY));
  m_ICAP_BITDEPTH.Cap = ICAP_BITDEPTH;
  get_CAP(m_ICAP_BITDEPTH);

  memset(&m_ICAP_XRESOLUTION,0,sizeof(TW_CAPABILITY));
  m_ICAP_XRESOLUTION.Cap = ICAP_XRESOLUTION;
  get_CAP(m_ICAP_XRESOLUTION);

  memset(&m_ICAP_YRESOLUTION,0,sizeof(TW_CAPABILITY));
  m_ICAP_YRESOLUTION.Cap = ICAP_YRESOLUTION;
  get_CAP(m_ICAP_YRESOLUTION);

  memset(&m_ICAP_FRAMES,0,sizeof(TW_CAPABILITY));
  m_ICAP_FRAMES.Cap = ICAP_FRAMES;
  get_CAP(m_ICAP_FRAMES);

  return;
}

//////////////////////////////////////////////////////////////////////////////
void TwainAppCMD::uninitCaps()
{
  if(m_DSMState < 3)
  {
    PrintCMDMessage("The DSM needs to be opened first.\n");
    return;
  }
  else if(m_DSMState < 4)
  {
    PrintCMDMessage("A Data Source needs to be opened first.\n");
    return;
  }

  if(m_CAP_XFERCOUNT.hContainer)
  {
    _DSM_Free(m_CAP_XFERCOUNT.hContainer);
    m_CAP_XFERCOUNT.hContainer=0;
  }
  if(m_ICAP_PIXELTYPE.hContainer)
  {
    _DSM_Free(m_ICAP_PIXELTYPE.hContainer);
    m_ICAP_PIXELTYPE.hContainer=0;
  }
  if(m_ICAP_XFERMECH.hContainer)
  {
    _DSM_Free(m_ICAP_XFERMECH.hContainer);
    m_ICAP_XFERMECH.hContainer=0;
  }
  if(m_ICAP_IMAGEFILEFORMAT.hContainer)
  {
    _DSM_Free(m_ICAP_IMAGEFILEFORMAT.hContainer);
    m_ICAP_IMAGEFILEFORMAT.hContainer=0;
  }
  if(m_ICAP_COMPRESSION.hContainer)
  {
    _DSM_Free(m_ICAP_COMPRESSION.hContainer);
    m_ICAP_COMPRESSION.hContainer=0;
  }
  if(m_ICAP_UNITS.hContainer)
  {
    _DSM_Free(m_ICAP_UNITS.hContainer);
    m_ICAP_UNITS.hContainer=0;
  }
  if(m_ICAP_BITDEPTH.hContainer)
  {
    _DSM_Free(m_ICAP_BITDEPTH.hContainer);
    m_ICAP_BITDEPTH.hContainer=0;
  }
  if(m_ICAP_XRESOLUTION.hContainer)
  {
    _DSM_Free(m_ICAP_XRESOLUTION.hContainer);
    m_ICAP_XRESOLUTION.hContainer=0;
  }
  if(m_ICAP_YRESOLUTION.hContainer)
  {
    _DSM_Free(m_ICAP_YRESOLUTION.hContainer);
    m_ICAP_YRESOLUTION.hContainer=0;
  }
  if(m_ICAP_FRAMES.hContainer)
  {
    _DSM_Free(m_ICAP_FRAMES.hContainer);
    m_ICAP_FRAMES.hContainer=0;
  }

  return;
}
//////////////////////////////////////////////////////////////////////////////
void TwainAppCMD::startScan()
{
  if(m_DSMState != 6)
  {
    printError(m_pDataSource, "A scan cannot be initiated unless we are in state 6");
    return;
  }

  TW_UINT16 mech;
  if(!getICAP_XFERMECH(mech))
  {
    printError(m_pDataSource, "Error: could not get the transfer mechanism");
    return;
  }

  switch (mech)
  {
  case TWSX_NATIVE:
    initiateTransfer_Native();
    break;

  case TWSX_FILE:
    {
      TW_UINT16 fileformat = TWFF_TIFF;
      if(!getICAP_IMAGEFILEFORMAT(fileformat))
      {
        // Default back to TIFF
        fileformat = TWFF_TIFF;
      }
      initiateTransfer_File(fileformat);
    }
    break;

  case TWSX_MEMORY:
    initiateTransfer_Memory();
    break;
  }

  return;
}

//////////////////////////////////////////////////////////////////////////////
void TwainAppCMD::set_CAP_XFERCOUNT(const TW_INT16 _count)
{
  set_CapabilityOneValue(CAP_XFERCOUNT, _count, TWTY_INT16);

  // now that we have set it, re-get it to ensure it was set
  if(TWCC_SUCCESS==get_CAP(m_CAP_XFERCOUNT))
  {
    TW_INT16 count;
    if(getCAP_XFERCOUNT(count) &&
      count == _count)
    {
      PrintCMDMessage("Capability successfully set!\n");
    }
  }

  return;
}

//////////////////////////////////////////////////////////////////////////////
void TwainAppCMD::set_ICAP_UNITS(const TW_UINT16 _val)
{
  set_CapabilityOneValue(ICAP_UNITS, _val, TWTY_UINT16);

  // now that we have set it, re-get it to ensure it was set
  if(TWCC_SUCCESS==get_CAP(m_ICAP_UNITS))
  {
    if(TWON_ENUMERATION == m_ICAP_UNITS.ConType &&
      0 != m_ICAP_UNITS.hContainer)
    {
      pTW_ENUMERATION pCapPT = (pTW_ENUMERATION)_DSM_LockMemory(m_ICAP_UNITS.hContainer);

      if(_val == pCapPT->ItemList[pCapPT->CurrentIndex])
      {
        PrintCMDMessage("Capability successfully set!\n");

        // successfully setting this cap means that we have to re-obtain the X/Y resolutions as well
        get_CAP(m_ICAP_XRESOLUTION);
        get_CAP(m_ICAP_YRESOLUTION);
      }
      _DSM_UnlockMemory(m_ICAP_UNITS.hContainer);
    }
  }

  return;
}

//////////////////////////////////////////////////////////////////////////////
void TwainAppCMD::set_ICAP_PIXELTYPE(const TW_UINT16 _pt)
{
  set_CapabilityOneValue(ICAP_PIXELTYPE, _pt, TWTY_UINT16);

  // now that we have set it, re-get it to ensure it was set
  if(TWCC_SUCCESS==get_CAP(m_ICAP_PIXELTYPE))
  {
    if(TWON_ENUMERATION == m_ICAP_PIXELTYPE.ConType &&
      0 != m_ICAP_PIXELTYPE.hContainer)
    {
      pTW_ENUMERATION pCapPT = (pTW_ENUMERATION)_DSM_LockMemory(m_ICAP_PIXELTYPE.hContainer);

      if(_pt == ((TW_UINT16*)(&pCapPT->ItemList))[pCapPT->CurrentIndex])
      {
        PrintCMDMessage("Capability successfully set!\n");
      }
      _DSM_UnlockMemory(m_ICAP_PIXELTYPE.hContainer);
    }
  }

  get_CAP(m_ICAP_BITDEPTH);

  return;
}

//////////////////////////////////////////////////////////////////////////////
void TwainAppCMD::set_ICAP_RESOLUTION(const TW_UINT16 _ICAP, const pTW_FIX32 _pVal)
{
  if((ICAP_XRESOLUTION != _ICAP) &&
     (ICAP_YRESOLUTION != _ICAP))
  {
    printError(m_pDataSource, "Invalid resolution passed in! Resolution set failed.");
    return;
  }

  set_CapabilityOneValue(_ICAP, _pVal);

  // Get the new RESOLUTION caps values to see if the set was successfull.
  get_CAP(m_ICAP_XRESOLUTION);
  get_CAP(m_ICAP_YRESOLUTION);

  pTW_CAPABILITY pCapRes = 0;

  if(ICAP_XRESOLUTION == _ICAP)
  {
    pCapRes = &m_ICAP_XRESOLUTION;
  }
  else
  {
    pCapRes = &m_ICAP_YRESOLUTION;
  }
  
  // check ICAP_XRESOLUTION
  if(TWON_ENUMERATION == pCapRes->ConType &&
    0 != pCapRes->hContainer)
  {
    pTW_ENUMERATION_FIX32 pdat = (pTW_ENUMERATION_FIX32)pCapRes->hContainer;

    if(TWTY_FIX32 == pdat->ItemType &&
      _pVal->Whole == pdat->ItemList[pdat->CurrentIndex].Whole &&
      _pVal->Frac == pdat->ItemList[pdat->CurrentIndex].Frac)
    {
      PrintCMDMessage("Resolution successfully set!\n");
    }
  }
  return;
}

//////////////////////////////////////////////////////////////////////////////
void TwainAppCMD::set_ICAP_FRAMES(const pTW_FRAME _pFrame)
{

  set_CapabilityOneValue(ICAP_FRAMES, _pFrame);

  // now that we have set it, re-get it to ensure it was set
  if(TWCC_SUCCESS==get_CAP(m_ICAP_FRAMES))
  {
    if(TWON_ENUMERATION == m_ICAP_FRAMES.ConType &&
      0 != m_ICAP_FRAMES.hContainer)
    {
      pTW_ENUMERATION_FRAME pCapPT = (pTW_ENUMERATION_FRAME)_DSM_LockMemory(m_ICAP_FRAMES.hContainer);
      
      pTW_FRAME ptframe = &pCapPT->ItemList[pCapPT->CurrentIndex];

      if( (_pFrame->Bottom == ptframe->Bottom) &&
          (_pFrame->Top == ptframe->Top) &&
          (_pFrame->Left == ptframe->Left) &&
          (_pFrame->Right == ptframe->Right))
      {
        PrintCMDMessage("Frames successfully set!\n");
      }
      _DSM_UnlockMemory(m_ICAP_FRAMES.hContainer);
    }
  }

  return;
}

//////////////////////////////////////////////////////////////////////////////
void TwainAppCMD::set_ICAP_XFERMECH(const TW_UINT16 _mech)
{
  set_CapabilityOneValue(ICAP_XFERMECH, _mech, TWTY_UINT16);

  // now that we have set it, re-get it to ensure it was set
  if(TWCC_SUCCESS==get_CAP(m_ICAP_XFERMECH))
  {
    TW_UINT16 mech;
    if(getICAP_XFERMECH(mech) &&
      mech == _mech)
    {
      PrintCMDMessage("XferMech successfully set!\n");
    }
  }

  // Update compression and FileFormat after xfer is set
  get_CAP(m_ICAP_COMPRESSION);
  get_CAP(m_ICAP_IMAGEFILEFORMAT);

  return;
}

//////////////////////////////////////////////////////////////////////////////
void TwainAppCMD::set_ICAP_IMAGEFILEFORMAT(const TW_UINT16 _fileformat)
{
  set_CapabilityOneValue(ICAP_IMAGEFILEFORMAT, _fileformat, TWTY_UINT16);

  // now that we have set it, re-get it to ensure it was set
  if(TWCC_SUCCESS==get_CAP(m_ICAP_IMAGEFILEFORMAT))
  {
    TW_UINT16 fileformat;
    if(getICAP_IMAGEFILEFORMAT(fileformat) &&
      fileformat == _fileformat)
    {
      PrintCMDMessage("ImageFileFormat successfully set!\n");
    }
  }

  // Update compression after xfer is set
  get_CAP(m_ICAP_COMPRESSION);

  return;
}

//////////////////////////////////////////////////////////////////////////////
void TwainAppCMD::set_ICAP_COMPRESSION(const TW_UINT16 _comp)
{
  set_CapabilityOneValue(ICAP_COMPRESSION, _comp, TWTY_UINT16);

  // now that we have set it, re-get it to ensure it was set
  if(TWCC_SUCCESS==get_CAP(m_ICAP_COMPRESSION))
  {
    TW_UINT16 comp;
    if(getICAP_COMPRESSION(comp) &&
      comp == _comp)
    {
      PrintCMDMessage("Compression successfully set!\n");
    }
  }

  return;
}

//////////////////////////////////////////////////////////////////////////////
void TwainAppCMD::set_ICAP_BITDEPTH(const TW_UINT16 _nVal)
{
  set_CapabilityOneValue(ICAP_BITDEPTH, _nVal, TWTY_UINT16);

  // now that we have set it, re-get it to ensure it was set
  if(TWCC_SUCCESS==get_CAP(m_ICAP_BITDEPTH))
  {
    TW_UINT16 val;
    if(getICAP_BITDEPTH(val) &&
      val == _nVal)
    {
      PrintCMDMessage("BitDepth successfully set!\n");
    }
  }

  return;
}

//////////////////////////////////////////////////////////////////////////////
bool TwainAppCMD::getICAP_UNITS(TW_UINT16& _val)
{
  TW_UINT32 val;
  bool rtn = getCurrent(&m_ICAP_UNITS, val);
  _val = (TW_UINT16)val;
  return rtn;
}

bool TwainAppCMD::getCAP_XFERCOUNT(TW_INT16& _val)
{
  TW_UINT32 val;
  bool rtn = getCurrent(&m_CAP_XFERCOUNT, val);
  _val = (TW_UINT16)val;
  return rtn;
}

bool TwainAppCMD::getICAP_XFERMECH(TW_UINT16& _val)
{
  TW_UINT32 val;
  bool rtn = getCurrent(&m_ICAP_XFERMECH, val);
  _val = (TW_UINT16)val;
  return rtn;
}

bool TwainAppCMD::getICAP_PIXELTYPE(TW_UINT16& _val)
{
  TW_UINT32 val;
  bool rtn = getCurrent(&m_ICAP_PIXELTYPE, val);
  _val = (TW_UINT16)val;
  return rtn;
}

bool TwainAppCMD::getICAP_BITDEPTH(TW_UINT16& _val)
{
  TW_UINT32 val;
  bool rtn = getCurrent(&m_ICAP_BITDEPTH, val);
  _val = (TW_UINT16)val;
  return rtn;
}

bool TwainAppCMD::getICAP_IMAGEFILEFORMAT(TW_UINT16& _val)
{
  TW_UINT32 val;
  bool rtn = getCurrent(&m_ICAP_IMAGEFILEFORMAT, val);
  _val = (TW_UINT16)val;
  return rtn;
}

bool TwainAppCMD::getICAP_COMPRESSION(TW_UINT16& _val)
{
  TW_UINT32 val;
  bool rtn = getCurrent(&m_ICAP_COMPRESSION, val);
  _val = (TW_UINT16)val;
  return rtn;
}

bool TwainAppCMD::getICAP_XRESOLUTION(TW_FIX32& _xres)
{
  return getCurrent(&m_ICAP_XRESOLUTION, _xres);
}

bool TwainAppCMD::getICAP_YRESOLUTION(TW_FIX32& _yres)
{
  return getCurrent(&m_ICAP_YRESOLUTION, _yres);
}


