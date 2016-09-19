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
 * @file TwainString.cpp
 * Convert TWAIN Constants to Strings
 * @author JFL Peripheral Solutions Inc.
 * @date Dec 2008
 */

#include "TwainString.h"

extern TW_HANDLE _DSM_Alloc(TW_UINT32 _size);
extern void _DSM_Free(TW_HANDLE _hMemory);
extern TW_MEMREF _DSM_LockMemory(TW_HANDLE _hMemory);
extern void _DSM_UnlockMemory(TW_HANDLE _hMemory);

/** the size of the temp buffer */
#define TEMPBUFSIZE 1024

/**
* return a temp buffer from an array.  Rotating through the queue.
* @return pointer to the buffer to use.
*/
char * nextTempBuffer()
{
  static char szTempBuffer[3][TEMPBUFSIZE];
  static int  nTempBuffer = 0;

  if(++nTempBuffer >= 3)
  {
    nTempBuffer = 0;
  }
  szTempBuffer[nTempBuffer][0] = '\0';
  return szTempBuffer[nTempBuffer];
}


//////////////////////////////////////////////////////////////////////////////
const char* convertCAP_Item_toString(const TW_UINT16 _unCap, const TW_UINT32 _unItem, const TW_UINT16 _unType)
{
  const char* pszString = "Unknown capability";

  switch(_unCap)
  {
    case CAP_ALARMS:
      pszString = convertCAP_ALARMS_toString((TW_UINT16)_unItem);
      break;

    case ICAP_AUTOSIZE:
      pszString = convertICAP_AUTOSIZE_toString((TW_UINT16)_unItem);
      break;

    case ICAP_COMPRESSION:
      pszString = convertICAP_COMPRESSION_toString((TW_UINT16)_unItem);
      break;

    case ICAP_BARCODESEARCHMODE:
      pszString = convertICAP_BARCODESEARCHMODE_toString((TW_UINT16)_unItem);
      break;

    case ICAP_BITORDER:
      pszString = convertICAP_BITORDER_toString((TW_UINT16)_unItem);
      break;

    case ICAP_AUTODISCARDBLANKPAGES:
      pszString = convertICAP_AUTODISCARDBLANKPAGES_toString((TW_UINT16)_unItem);
      break;

    case ICAP_BITDEPTH:
      {
      char *buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "%u", _unItem);
      pszString = buff;
      }
      break;

    case ICAP_BITDEPTHREDUCTION:
      pszString = convertICAP_BITDEPTHREDUCTION_toString((TW_UINT16)_unItem);
      break;

    case ICAP_SUPPORTEDBARCODETYPES:
      pszString = convertICAP_SUPPORTEDBARCODETYPES_toString((TW_UINT16)_unItem);
      break;

    case CAP_CAMERASIDE:
      pszString = convertCAP_CAMERASIDE_toString((TW_UINT16)_unItem);
      break;

    case CAP_CLEARBUFFERS:
      pszString = convertCAP_CLEARBUFFERS_toString((TW_UINT16)_unItem);
      break;

    case CAP_DEVICEEVENT:
      pszString = convertCAP_DEVICEEVENT_toString((TW_UINT16)_unItem);
      break;

    case CAP_DUPLEX:
      pszString = convertCAP_DUPLEX_toString((TW_UINT16)_unItem);
      break;

    case CAP_EXTENDEDCAPS:
      pszString = convertCAP_toString((TW_UINT16)_unItem);
      break;

    case CAP_FEEDERALIGNMENT:
      pszString = convertCAP_FEEDERALIGNMENT_toString((TW_UINT16)_unItem);
      break;

    case ICAP_FEEDERTYPE:
      pszString = convertICAP_FEEDERTYPE_toString((TW_UINT16)_unItem);
      break;

    case ICAP_IMAGEFILEFORMAT:
      pszString = convertICAP_IMAGEFILEFORMAT_toString((TW_UINT16)_unItem);
      break;

    case ICAP_FLASHUSED2:
      pszString = convertICAP_FLASHUSED2_toString((TW_UINT16)_unItem);
      break;

    case CAP_FEEDERORDER:
      pszString = convertCAP_FEEDERORDER_toString((TW_UINT16)_unItem);
      break;

    case CAP_FEEDERPOCKET:
      pszString = convertCAP_FEEDERPOCKET_toString((TW_UINT16)_unItem);
      break;

    case ICAP_FLIPROTATION:
      pszString = convertICAP_FLIPROTATION_toString((TW_UINT16)_unItem);
      break;

    case ICAP_FILTER:
      pszString = convertICAP_FILTER_toString((TW_UINT16)_unItem);
      break;

    case ICAP_ICCPROFILE:
      pszString = convertICAP_ICCPROFILE_toString((TW_UINT16)_unItem);
      break;

    case ICAP_IMAGEFILTER:
      pszString = convertICAP_IMAGEFILTER_toString((TW_UINT16)_unItem);
      break;

    case ICAP_IMAGEMERGE:
      pszString = convertICAP_IMAGEMERGE_toString((TW_UINT16)_unItem);
      break;

    case CAP_JOBCONTROL:
      pszString = convertCAP_JOBCONTROL_toString((TW_UINT16)_unItem);
      break;

    case ICAP_JPEGQUALITY:
      pszString = convertICAP_JPEGQUALITY_toString((TW_UINT16)_unItem);
      break;

    case ICAP_LIGHTPATH:
      pszString = convertICAP_LIGHTPATH_toString((TW_UINT16)_unItem);
      break;

    case ICAP_LIGHTSOURCE:
      pszString = convertICAP_LIGHTSOURCE_toString((TW_UINT16)_unItem);
      break;

    case ICAP_NOISEFILTER:
      pszString = convertICAP_NOISEFILTER_toString((TW_UINT16)_unItem);
      break;

    case ICAP_ORIENTATION:
      pszString = convertICAP_ORIENTATION_toString((TW_UINT16)_unItem);
      break;

    case ICAP_OVERSCAN:
      pszString = convertICAP_OVERSCAN_toString((TW_UINT16)_unItem);
      break;

    case ICAP_PLANARCHUNKY:
      pszString = convertICAP_PLANARCHUNKY_toString((TW_UINT16)_unItem);
      break;

    case ICAP_PIXELFLAVOR:
      pszString = convertICAP_PIXELFLAVOR_toString((TW_UINT16)_unItem);
      break;

    case CAP_PRINTERMODE:
      pszString = convertCAP_PRINTERMODE_toString((TW_UINT16)_unItem);
      break;

    case CAP_PRINTER:
      pszString = convertCAP_PRINTER_toString((TW_UINT16)_unItem);
      break;

    case CAP_POWERSUPPLY:
      pszString = convertCAP_POWERSUPPLY_toString((TW_UINT16)_unItem);
      break;

    case ICAP_PIXELTYPE:
      pszString = convertICAP_PIXELTYPE_toString((TW_UINT16)_unItem);
      break;

    case CAP_SEGMENTED:
      pszString = convertCAP_SEGMENTED_toString((TW_UINT16)_unItem);
      break;

    case ICAP_SUPPORTEDEXTIMAGEINFO:
      pszString = convertExtImageInfoName_toString((TW_UINT16)_unItem);
      break;

    case ICAP_SUPPORTEDSIZES:
      pszString = convertICAP_SUPPORTEDSIZES_toString((TW_UINT16)_unItem);
      break;

    case ICAP_XFERMECH:
      pszString = convertICAP_XFERMECH_toString((TW_UINT16)_unItem);
      break;

    case ICAP_UNITS:
      pszString = convertICAP_UNITS_toString((TW_UINT16)_unItem);
      break;

    default:
      {
      char *buff = nextTempBuffer();
      switch(_unType)
      {
        case TWTY_UINT8:
        case TWTY_UINT16:
        case TWTY_UINT32:
          SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "%u [0x%x]", _unItem, _unItem);
          break;

        case TWTY_INT8:
        case TWTY_INT16:
        case TWTY_INT32:
          SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "%d", _unItem);
          break;

        case TWTY_BOOL:
          SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "%s", (TW_BOOL)_unItem? "True" : "False");
          break;
      }
      pszString = buff;
      }
      break;
  }

  return pszString;
}

//////////////////////////////////////////////////////////////////////////////
const char* convertEI_Item_toString(const TW_UINT16 _unEI, const TW_UINT32 _unItem)
{
  const char* pszString = NULL;

  switch(_unEI)
  {
    case TWEI_BARCODEROTATION:
      pszString = convertTWEI_BARCODEROTATION_toString((TW_UINT16)_unItem);
      break;

    case TWEI_BARCODETYPE:
      pszString = convertICAP_SUPPORTEDBARCODETYPES_toString((TW_UINT16)_unItem);
      break;

    case TWEI_PAGESIDE:
      pszString = convertTWEI_PAGESIDE_toString((TW_UINT16)_unItem);
      break;

    case TWEI_DESKEWSTATUS:
      pszString = convertTWEI_DESKEWSTATUS_toString((TW_UINT16)_unItem);
      break;

    case TWEI_FILESYSTEMSOURCE:
      pszString = convertTWEI_FILESYSTEM_toString((TW_UINT16)_unItem);
      break;

    case TWEI_PIXELFLAVOR:
      pszString = convertICAP_PIXELFLAVOR_toString((TW_UINT16)_unItem);
      break;

    case TWEI_MAGTYPE:
      pszString = convertTWEI_MAGTYPE_toString((TW_UINT16)_unItem);
      break;

    case TWEI_PATCHCODE:
      pszString = convertTWEI_PATCHCODE_toString((TW_UINT16)_unItem);
      break;
  }

  return pszString;
}


//////////////////////////////////////////////////////////////////////////////
const char* convertCAP_toString(const TW_UINT16 _unCap)
{
  const char* text;

  switch(_unCap)
  {
  case CAP_CUSTOMBASE:
    text = "CAP_CUSTOMBASE";
    break;

  case CAP_XFERCOUNT:
    text = "CAP_XFERCOUNT";
    break;

  case ICAP_COMPRESSION:
    text = "ICAP_COMPRESSION";
    break;

  case ICAP_PIXELTYPE:
    text = "ICAP_PIXELTYPE";
    break;

  case ICAP_UNITS:
    text = "ICAP_UNITS";
    break;

  case ICAP_XFERMECH:
    text = "ICAP_XFERMECH";
    break;

  case CAP_AUTHOR:
    text = "CAP_AUTHOR";
    break;

  case CAP_CAPTION:
    text = "CAP_CAPTION";
    break;

  case CAP_FEEDERENABLED:
    text = "CAP_FEEDERENABLED";
    break;

  case CAP_FEEDERLOADED:
    text = "CAP_FEEDERLOADED";
    break;

  case CAP_TIMEDATE:
    text = "CAP_TIMEDATE";
    break;

  case CAP_SUPPORTEDCAPS:
    text = "CAP_SUPPORTEDCAPS";
    break;

  case CAP_EXTENDEDCAPS:
    text = "CAP_EXTENDEDCAPS";
    break;

  case CAP_AUTOFEED:
    text = "CAP_AUTOFEED";
    break;

  case CAP_CLEARPAGE:
    text = "CAP_CLEARPAGE";
    break;

  case CAP_FEEDPAGE:
    text = "CAP_FEEDPAGE";
    break;

  case CAP_REWINDPAGE:
    text = "CAP_REWINDPAGE";
    break;

  case CAP_INDICATORS:
    text = "CAP_INDICATORS";
    break;

  case CAP_SUPPORTEDCAPSEXT:
    text = "CAP_SUPPORTEDCAPSEXT";
    break;

  case CAP_PAPERDETECTABLE:
    text = "CAP_PAPERDETECTABLE";
    break;

  case CAP_UICONTROLLABLE:
    text = "CAP_UICONTROLLABLE";
    break;

  case CAP_DEVICEONLINE:
    text = "CAP_DEVICEONLINE";
    break;

  case CAP_AUTOSCAN:
    text = "CAP_AUTOSCAN";
    break;

  case CAP_THUMBNAILSENABLED:
    text = "CAP_THUMBNAILSENABLED";
    break;

  case CAP_DUPLEX:
    text = "CAP_DUPLEX";
    break;

  case CAP_DUPLEXENABLED:
    text = "CAP_DUPLEXENABLED";
    break;

  case CAP_ENABLEDSUIONLY:
    text = "CAP_ENABLEDSUIONLY";
    break;

  case CAP_CUSTOMDSDATA:
    text = "CAP_CUSTOMDSDATA";
    break;

  case CAP_ENDORSER:
    text = "CAP_ENDORSER";
    break;

  case CAP_JOBCONTROL:
    text = "CAP_JOBCONTROL";
    break;

  case CAP_ALARMS:
    text = "CAP_ALARMS";
    break;

  case CAP_ALARMVOLUME:
    text = "CAP_ALARMVOLUME";
    break;

  case CAP_AUTOMATICCAPTURE:
    text = "CAP_AUTOMATICCAPTURE";
    break;

  case CAP_TIMEBEFOREFIRSTCAPTURE:
    text = "CAP_TIMEBEFOREFIRSTCAPTURE";
    break;

  case CAP_TIMEBETWEENCAPTURES:
    text = "CAP_TIMEBETWEENCAPTURES";
    break;

  case CAP_CLEARBUFFERS:
    text = "CAP_CLEARBUFFERS";
    break;

  case CAP_MAXBATCHBUFFERS:
    text = "CAP_MAXBATCHBUFFERS";
    break;

  case CAP_DEVICETIMEDATE:
    text = "CAP_DEVICETIMEDATE";
    break;

  case CAP_POWERSUPPLY:
    text = "CAP_POWERSUPPLY";
    break;

  case CAP_CAMERAPREVIEWUI:
    text = "CAP_CAMERAPREVIEWUI";
    break;

  case CAP_DEVICEEVENT:
    text = "CAP_DEVICEEVENT";
    break;

  case CAP_SERIALNUMBER:
    text = "CAP_SERIALNUMBER";
    break;

  case CAP_PRINTER:
    text = "CAP_PRINTER";
    break;

  case CAP_PRINTERENABLED:
    text = "CAP_PRINTERENABLED";
    break;

  case CAP_PRINTERINDEX:
    text = "CAP_PRINTERINDEX";
    break;

  case CAP_PRINTERMODE:
    text = "CAP_PRINTERMODE";
    break;

  case CAP_PRINTERSTRING:
    text = "CAP_PRINTERSTRING";
    break;

  case CAP_PRINTERSUFFIX:
    text = "CAP_PRINTERSUFFIX";
    break;

  case CAP_LANGUAGE:
    text = "CAP_LANGUAGE";
    break;

  case CAP_FEEDERALIGNMENT:
    text = "CAP_FEEDERALIGNMENT";
    break;

  case CAP_FEEDERORDER:
    text = "CAP_FEEDERORDER";
    break;

  case CAP_REACQUIREALLOWED:
    text = "CAP_REACQUIREALLOWED";
    break;

  case CAP_BATTERYMINUTES:
    text = "CAP_BATTERYMINUTES";
    break;

  case CAP_BATTERYPERCENTAGE:
    text = "CAP_BATTERYPERCENTAGE";
    break;

  case CAP_CAMERASIDE:
    text = "CAP_CAMERASIDE";
    break;

  case CAP_SEGMENTED:
    text = "CAP_SEGMENTED";
    break;

  case CAP_CAMERAENABLED:
    text = "CAP_CAMERAENABLED";
    break;

  case CAP_CAMERAORDER:
    text = "CAP_CAMERAORDER";
    break;

  case CAP_MICRENABLED:
    text = "CAP_MICRENABLED";
    break;

  case CAP_FEEDERPREP:
    text = "CAP_FEEDERPREP";
    break;

  case CAP_FEEDERPOCKET:
    text = "CAP_FEEDERPOCKET";
    break;

  case CAP_AUTOMATICSENSEMEDIUM:
    text = "CAP_AUTOMATICSENSEMEDIUM";
    break;

  case CAP_CUSTOMINTERFACEGUID:
    text = "CAP_CUSTOMINTERFACEGUID";
    break;

  case ICAP_AUTOBRIGHT:
    text = "ICAP_AUTOBRIGHT";
    break;

  case ICAP_BRIGHTNESS:
    text = "ICAP_BRIGHTNESS";
    break;

  case ICAP_CONTRAST:
    text = "ICAP_CONTRAST";
    break;

  case ICAP_CUSTHALFTONE:
    text = "ICAP_CUSTHALFTONE";
    break;

  case ICAP_EXPOSURETIME:
    text = "ICAP_EXPOSURETIME";
    break;

  case ICAP_FILTER:
    text = "ICAP_FILTER";
    break;

  case ICAP_FLASHUSED:
    text = "ICAP_FLASHUSED";
    break;

  case ICAP_GAMMA:
    text = "ICAP_GAMMA";
    break;

  case ICAP_HALFTONES:
    text = "ICAP_HALFTONES";
    break;

  case ICAP_HIGHLIGHT:
    text = "ICAP_HIGHLIGHT";
    break;

  case ICAP_IMAGEFILEFORMAT:
    text = "ICAP_IMAGEFILEFORMAT";
    break;

  case ICAP_LAMPSTATE:
    text = "ICAP_LAMPSTATE";
    break;

  case ICAP_LIGHTSOURCE:
    text = "ICAP_LIGHTSOURCE";
    break;

  case ICAP_ORIENTATION:
    text = "ICAP_ORIENTATION";
    break;

  case ICAP_PHYSICALWIDTH:
    text = "ICAP_PHYSICALWIDTH";
    break;

  case ICAP_PHYSICALHEIGHT:
    text = "ICAP_PHYSICALHEIGHT";
    break;

  case ICAP_SHADOW:
    text = "ICAP_SHADOW";
    break;

  case ICAP_FRAMES:
    text = "ICAP_FRAMES";
    break;

  case ICAP_XNATIVERESOLUTION:
    text = "ICAP_XNATIVERESOLUTION";
    break;

  case ICAP_YNATIVERESOLUTION:
    text = "ICAP_YNATIVERESOLUTION";
    break;

  case ICAP_XRESOLUTION:
    text = "ICAP_XRESOLUTION";
    break;

  case ICAP_YRESOLUTION:
    text = "ICAP_YRESOLUTION";
    break;

  case ICAP_MAXFRAMES:
    text = "ICAP_MAXFRAMES";
    break;

  case ICAP_TILES:
    text = "ICAP_TILES";
    break;

  case ICAP_BITORDER:
    text = "ICAP_BITORDER";
    break;

  case ICAP_CCITTKFACTOR:
    text = "ICAP_CCITTKFACTOR";
    break;

  case ICAP_LIGHTPATH:
    text = "ICAP_LIGHTPATH";
    break;

  case ICAP_PIXELFLAVOR:
    text = "ICAP_PIXELFLAVOR";
    break;

  case ICAP_PLANARCHUNKY:
    text = "ICAP_PLANARCHUNKY";
    break;

  case ICAP_ROTATION:
    text = "ICAP_ROTATION";
    break;

  case ICAP_SUPPORTEDSIZES:
    text = "ICAP_SUPPORTEDSIZES";
    break;

  case ICAP_THRESHOLD:
    text = "ICAP_THRESHOLD";
    break;

  case ICAP_XSCALING:
    text = "ICAP_XSCALING";
    break;

  case ICAP_YSCALING:
    text = "ICAP_YSCALING";
    break;

  case ICAP_BITORDERCODES:
    text = "ICAP_BITORDERCODES";
    break;

  case ICAP_PIXELFLAVORCODES:
    text = "ICAP_PIXELFLAVORCODES";
    break;

  case ICAP_JPEGPIXELTYPE:
    text = "ICAP_JPEGPIXELTYPE";
    break;

  case ICAP_TIMEFILL:
    text = "ICAP_TIMEFILL";
    break;

  case ICAP_BITDEPTH:
    text = "ICAP_BITDEPTH";
    break;

  case ICAP_BITDEPTHREDUCTION:
    text = "ICAP_BITDEPTHREDUCTION";
    break;

  case ICAP_UNDEFINEDIMAGESIZE:
    text = "ICAP_UNDEFINEDIMAGESIZE";
    break;

  case ICAP_IMAGEDATASET:
    text = "ICAP_IMAGEDATASET";
    break;

  case ICAP_EXTIMAGEINFO:
    text = "ICAP_EXTIMAGEINFO";
    break;

  case ICAP_MINIMUMHEIGHT:
    text = "ICAP_MINIMUMHEIGHT";
    break;

  case ICAP_MINIMUMWIDTH:
    text = "ICAP_MINIMUMWIDTH";
    break;

  case ICAP_FLIPROTATION:
    text = "ICAP_FLIPROTATION";
    break;

  case ICAP_BARCODEDETECTIONENABLED:
    text = "ICAP_BARCODEDETECTIONENABLED";
    break;

  case ICAP_SUPPORTEDBARCODETYPES:
    text = "ICAP_SUPPORTEDBARCODETYPES";
    break;

  case ICAP_BARCODEMAXSEARCHPRIORITIES:
    text = "ICAP_BARCODEMAXSEARCHPRIORITIES";
    break;

  case ICAP_BARCODESEARCHPRIORITIES:
    text = "ICAP_BARCODESEARCHPRIORITIES";
    break;

  case ICAP_BARCODESEARCHMODE:
    text = "ICAP_BARCODESEARCHMODE";
    break;

  case ICAP_BARCODEMAXRETRIES:
    text = "ICAP_BARCODEMAXRETRIES";
    break;

  case ICAP_BARCODETIMEOUT:
    text = "ICAP_BARCODETIMEOUT";
    break;

  case ICAP_ZOOMFACTOR:
    text = "ICAP_ZOOMFACTOR";
    break;

  case ICAP_PATCHCODEDETECTIONENABLED:
    text = "ICAP_PATCHCODEDETECTIONENABLED";
    break;

  case ICAP_SUPPORTEDPATCHCODETYPES:
    text = "ICAP_SUPPORTEDPATCHCODETYPES";
    break;

  case ICAP_PATCHCODEMAXSEARCHPRIORITIES:
    text = "ICAP_PATCHCODEMAXSEARCHPRIORITIES";
    break;

  case ICAP_PATCHCODESEARCHPRIORITIES:
    text = "ICAP_PATCHCODESEARCHPRIORITIES";
    break;

  case ICAP_PATCHCODESEARCHMODE:
    text = "ICAP_PATCHCODESEARCHMODE";
    break;

  case ICAP_PATCHCODEMAXRETRIES:
    text = "ICAP_PATCHCODEMAXRETRIES";
    break;

  case ICAP_PATCHCODETIMEOUT:
    text = "ICAP_PATCHCODETIMEOUT";
    break;

  case ICAP_FLASHUSED2:
    text = "ICAP_FLASHUSED2";
    break;

  case ICAP_IMAGEFILTER:
    text = "ICAP_IMAGEFILTER";
    break;

  case ICAP_NOISEFILTER:
    text = "ICAP_NOISEFILTER";
    break;

  case ICAP_OVERSCAN:
    text = "ICAP_OVERSCAN";
    break;

  case ICAP_AUTOMATICBORDERDETECTION:
    text = "ICAP_AUTOMATICBORDERDETECTION";
    break;

  case ICAP_AUTOMATICDESKEW:
    text = "ICAP_AUTOMATICDESKEW";
    break;

  case ICAP_AUTOMATICROTATE:
    text = "ICAP_AUTOMATICROTATE";
    break;

  case ICAP_JPEGQUALITY:
    text = "ICAP_JPEGQUALITY";
    break;

  case ICAP_FEEDERTYPE:
    text = "ICAP_FEEDERTYPE";
    break;

  case ICAP_ICCPROFILE:
    text = "ICAP_ICCPROFILE";
    break;

  case ICAP_AUTOSIZE:
    text = "ICAP_AUTOSIZE";
    break;

    case ICAP_AUTOMATICCROPUSESFRAME:
    text = "ICAP_AUTOMATICCROPUSESFRAME";
    break;

  case ICAP_AUTOMATICLENGTHDETECTION:
    text = "ICAP_AUTOMATICLENGTHDETECTION";
    break;

  case ICAP_AUTOMATICCOLORENABLED:
    text = "ICAP_AUTOMATICCOLORENABLED";
    break;

  case ICAP_AUTOMATICCOLORNONCOLORPIXELTYPE:
    text = "ICAP_AUTOMATICCOLORNONCOLORPIXELTYPE";
    break;

  case ICAP_COLORMANAGEMENTENABLED:
    text = "ICAP_COLORMANAGEMENTENABLED";
    break;

  case ICAP_IMAGEMERGE:
    text = "ICAP_IMAGEMERGE";
    break;

  case ICAP_IMAGEMERGEHEIGHTTHRESHOLD:
    text = "ICAP_IMAGEMERGEHEIGHTTHRESHOLD";
    break;

  case ICAP_SUPPORTEDEXTIMAGEINFO:
    text = "ICAP_SUPPORTEDEXTIMAGEINFO";
    break;

  case ACAP_AUDIOFILEFORMAT:
    text = "ACAP_AUDIOFILEFORMAT";
    break;

  case ACAP_XFERMECH:
    text = "ACAP_XFERMECH";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      if(_unCap < CAP_CUSTOMBASE)
      {
        SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown CAP 0x:%4X", _unCap);
      }
      else if (_unCap > CAP_CUSTOMBASE)
      {
        SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Custom CAP 0x:%4X", _unCap);
      }
      else // == CAP_CUSTOMBASE
      {
        SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Invalid CAP 0x:%4X", _unCap);
      }
      text = buff;
    }
    break;
  }

  return text;
}



//////////////////////////////////////////////////////////////////////////////
const char* convertTWTY_toString(const TW_UINT16 _unItem)
{
  const char* text;

  switch(_unItem)
  {
  case TWTY_INT8:
    text = "INT8";
    break;

  case TWTY_INT16:
    text = "INT16";
    break;

  case TWTY_INT32:
    text = "INT32";
    break;

  case TWTY_UINT8:
    text = "UINT8";
    break;

  case TWTY_UINT16:
    text = "UINT16";
    break;

  case TWTY_UINT32:
    text = "UINT32";
    break;

  case TWTY_BOOL:
    text = "BOOL";
    break;

  case TWTY_FIX32:
    text = "FIX32";
    break;

  case TWTY_FRAME:
    text = "FRAME";
    break;

  case TWTY_STR32:
    text = "STR32";
    break;

  case TWTY_STR64:
    text = "STR64";
    break;

  case TWTY_STR128:
    text = "STR128";
    break;

  case TWTY_STR255:
    text = "STR255";
    break;

  case TWTY_STR1024:
    text = "STR1024";
    break;

  case TWTY_UNI512:
    text = "UNI512";
    break;

  case TWTY_HANDLE:
    text = "HANDLE";
    break;    

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWTY 0x:%4X", _unItem);
      text = buff;
    }
    break;
  }

  return text;
}



//////////////////////////////////////////////////////////////////////////////
// Convert Extended Image Info Attributes to String

const char* convertExtImageInfoName_toString(int InfoID)
{
  const char* text;

  switch(InfoID)
  {
  case TWEI_BARCODEX:
    text = "TWEI_BARCODEX";
    break;

  case TWEI_BARCODEY:
    text = "TWEI_BARCODEY";
    break;

	case TWEI_BARCODETEXT:
    text = "TWEI_BARCODETEXT";
    break;

	case TWEI_BARCODETYPE:
    text = "TWEI_BARCODETYPE";
    break;

	case TWEI_DESHADETOP:
    text = "TWEI_DESHADETOP";
    break;

	case TWEI_DESHADELEFT:
    text = "TWEI_DESHADELEFT";
    break;

	case TWEI_DESHADEHEIGHT:
    text = "TWEI_DESHADEHEIGHT";
    break;

	case TWEI_DESHADEWIDTH:
    text = "TWEI_DESHADEWIDTH";
    break;

	case TWEI_DESHADESIZE:
    text = "TWEI_DESHADESIZE";
    break;

	case TWEI_SPECKLESREMOVED:
    text = "TWEI_SPECKLESREMOVED";
    break;

	case TWEI_HORZLINEXCOORD:
    text = "TWEI_HORZLINEXCOORD";
    break;

	case TWEI_HORZLINEYCOORD:
    text = "TWEI_HORZLINEYCOORD";
    break;

	case TWEI_HORZLINELENGTH:
    text = "TWEI_HORZLINELENGTH";
    break;

	case TWEI_HORZLINETHICKNESS:
    text = "TWEI_HORZLINETHICKNESS";
    break;

	case TWEI_VERTLINEXCOORD:
    text = "TWEI_VERTLINEXCOORD";
    break;

	case TWEI_VERTLINEYCOORD:
    text = "TWEI_VERTLINEYCOORD";
    break;

	case TWEI_VERTLINELENGTH:
    text = "TWEI_VERTLINELENGTH";
    break;

	case TWEI_VERTLINETHICKNESS:
    text = "TWEI_VERTLINETHICKNESS";
    break;

	case TWEI_PATCHCODE:
    text = "TWEI_PATCHCODE";
    break;

	case TWEI_ENDORSEDTEXT:
    text = "TWEI_ENDORSEDTEXT";
    break;

	case TWEI_FORMCONFIDENCE:
    text = "TWEI_FORMCONFIDENCE";
    break;

	case TWEI_FORMTEMPLATEMATCH:
    text = "TWEI_FORMTEMPLATEMATCH";
    break;

	case TWEI_FORMTEMPLATEPAGEMATCH:
    text = "TWEI_FORMTEMPLATEPAGEMATCH";
    break;

	case TWEI_FORMHORZDOCOFFSET:
    text = "TWEI_FORMHORZDOCOFFSET";
    break;

	case TWEI_FORMVERTDOCOFFSET:
    text = "TWEI_FORMVERTDOCOFFSET";
    break;

	case TWEI_BARCODECOUNT:
    text = "TWEI_BARCODECOUNT";
    break;

	case TWEI_BARCODECONFIDENCE:
    text = "TWEI_BARCODECONFIDENCE";
    break;

	case TWEI_BARCODEROTATION:
    text = "TWEI_BARCODEROTATION";
    break;

	case TWEI_BARCODETEXTLENGTH:
    text = "TWEI_BARCODETEXTLENGTH";
    break;

	case TWEI_DESHADECOUNT:
    text = "TWEI_DESHADECOUNT";
    break;

	case TWEI_DESHADEBLACKCOUNTOLD:
    text = "TWEI_DESHADEBLACKCOUNTOLD";
    break;

	case TWEI_DESHADEBLACKCOUNTNEW:
    text = "TWEI_DESHADEBLACKCOUNTNEW";
    break;

	case TWEI_DESHADEBLACKRLMIN:
    text = "TWEI_DESHADEBLACKRLMIN";
    break;

	case TWEI_DESHADEBLACKRLMAX:
    text = "TWEI_DESHADEBLACKRLMAX";
    break;

	case TWEI_DESHADEWHITECOUNTOLD:
    text = "TWEI_DESHADEWHITECOUNTOLD";
    break;

	case TWEI_DESHADEWHITECOUNTNEW:
    text = "TWEI_DESHADEWHITECOUNTNEW";
    break;

	case TWEI_DESHADEWHITERLMIN:
    text = "TWEI_DESHADEWHITERLMIN";
    break;

	case TWEI_DESHADEWHITERLAVE:
    text = "TWEI_DESHADEWHITERLAVE";
    break;

	case TWEI_DESHADEWHITERLMAX:
    text = "TWEI_DESHADEWHITERLMAX";
    break;

	case TWEI_BLACKSPECKLESREMOVED:
    text = "TWEI_BLACKSPECKLESREMOVED";
    break;

	case TWEI_WHITESPECKLESREMOVED:
    text = "TWEI_WHITESPECKLESREMOVED";
    break;

	case TWEI_HORZLINECOUNT:
    text = "TWEI_HORZLINECOUNT";
    break;

	case TWEI_VERTLINECOUNT:
    text = "TWEI_VERTLINECOUNT";
    break;

	case TWEI_DESKEWSTATUS:
    text = "TWEI_DESKEWSTATUS";
    break;

	case TWEI_SKEWORIGINALANGLE:
    text = "TWEI_SKEWORIGINALANGLE";
    break;

	case TWEI_SKEWFINALANGLE:
    text = "TWEI_SKEWFINALANGLE";
    break;

	case TWEI_SKEWCONFIDENCE:
    text = "TWEI_SKEWCONFIDENCE";
    break;

	case TWEI_SKEWWINDOWX1:
    text = "TWEI_SKEWWINDOWX1";
    break;

	case TWEI_SKEWWINDOWY1:
    text = "TWEI_SKEWWINDOWY1";
    break;

	case TWEI_SKEWWINDOWX2:
    text = "TWEI_SKEWWINDOWX2";
    break;

	case TWEI_SKEWWINDOWY2:
    text = "TWEI_SKEWWINDOWY2";
    break;

	case TWEI_SKEWWINDOWX3:
    text = "TWEI_SKEWWINDOWX3";
    break;

	case TWEI_SKEWWINDOWY3:
    text = "TWEI_SKEWWINDOWY3";
    break;

	case TWEI_SKEWWINDOWX4:
    text = "TWEI_SKEWWINDOWX4";
    break;

	case TWEI_SKEWWINDOWY4:
    text = "TWEI_SKEWWINDOWY4";
    break;

	case TWEI_BOOKNAME:
    text = "TWEI_BOOKNAME";
    break;

	case TWEI_CHAPTERNUMBER:
    text = "TWEI_CHAPTERNUMBER";
    break;

	case TWEI_DOCUMENTNUMBER:
    text = "TWEI_DOCUMENTNUMBER";
    break;

	case TWEI_PAGENUMBER:
    text = "TWEI_PAGENUMBER";
    break;

	case TWEI_CAMERA:
    text = "TWEI_CAMERA";
    break;

	case TWEI_FRAMENUMBER:
    text = "TWEI_FRAMENUMBER";
    break;

	case TWEI_FRAME:
    text = "TWEI_FRAME";
    break;

	case TWEI_PIXELFLAVOR:
    text = "TWEI_PIXELFLAVOR";
    break;

	case TWEI_PAGESIDE:
    text = "TWEI_PAGESIDE";
    break;

	case TWEI_MAGDATA:
    text = "TWEI_MAGDATA";
    break;

	case TWEI_MAGTYPE:
    text = "TWEI_MAGTYPE";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "ExtImageInfo ID 0x:%4X",InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert CAP_ALARMS values to String
const char* convertCAP_ALARMS_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWAL_ALARM:
    text = "Alarm";
    break;

	case TWAL_FEEDERERROR:
    text = "Feeder Error";
    break;

	case TWAL_FEEDERWARNING:
    text = "Feeder Warning";
    break;

	case TWAL_BARCODE:
    text = "Barcode";
    break;

	case TWAL_DOUBLEFEED:
    text = "Double Feed";
    break;

	case TWAL_JAM:
    text = "Paper Jam";
    break;

	case TWAL_PATCHCODE:
    text = "Patch Code";
    break;

	case TWAL_POWER:
    text = "Power";
    break;

	case TWAL_SKEW:
    text = "Skew";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWAL 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert ICAP_AUTOSIZE values to String
const char* convertICAP_AUTOSIZE_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWAS_NONE:
    text = "None";
    break;

	case TWAS_AUTO:
    text = "Auto";
    break;

	case TWAS_CURRENT:
    text = "Current";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWAS 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert TWEI_BARCODEROTATION values to String
const char* convertTWEI_BARCODEROTATION_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWBCOR_ROT0:
    text = "Rotation 0 deg";
    break;

	case TWBCOR_ROT90:
    text = "Rotation 90 deg";
    break;

	case TWBCOR_ROT180:
    text = "Rotation 180 deg";
    break;

	case TWBCOR_ROT270:
    text = "Rotation 270 deg";
    break;

	case TWBCOR_ROTX:
    text = "Rotation x deg";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWBCOR 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert ICAP_BARCODESEARCHMODE values to String
const char* convertICAP_BARCODESEARCHMODE_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWBD_HORZ:
    text = "Horizontal";
    break;

	case TWBD_VERT:
    text = "Vertical";
    break;

	case TWBD_HORZVERT:
    text = "Horz Vert";
    break;

	case TWBD_VERTHORZ:
    text = "Vert Horz";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWBD 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert ICAP_BITORDER values to String
const char* convertICAP_BITORDER_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWBO_LSBFIRST:
    text = "LSB first";
    break;

	case TWBO_MSBFIRST:
    text = "MSB first";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWBO 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert ICAP_AUTODISCARDBLANKPAGES values to String
const char* convertICAP_AUTODISCARDBLANKPAGES_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWBP_DISABLE:
    text = "Disable";
    break;

	case TWBP_AUTO:
    text = "Auto";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWBP 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert ICAP_BITDEPTHREDUCTION values to String
const char* convertICAP_BITDEPTHREDUCTION_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWBR_THRESHOLD:
    text = "Threshold";
    break;

	case TWBR_HALFTONE:
    text = "Halftone";
    break;

	case TWBR_CUSTHALFTONE:
    text = "Custom Halftone";
    break;

	case TWBR_DIFFUSION:
    text = "Diffusion";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWBR 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert ICAP_SUPPORTEDBARCODETYPES and TWEI_BARCODETYPE values to String
const char* convertICAP_SUPPORTEDBARCODETYPES_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWBT_3OF9:
    text = "3of9";
    break;

	case TWBT_2OF5INTERLEAVED:
    text = "2of5 interleaved";
    break;

	case TWBT_2OF5NONINTERLEAVED:
    text = "2of5 noninterleaved";
    break;

	case TWBT_CODE93:
    text = "code93";
    break;

	case TWBT_CODE128:
    text = "code128";
    break;

	case TWBT_UCC128:
    text = "UCC128";
    break;

	case TWBT_CODABAR:
    text = "Codebar";
    break;

	case TWBT_UPCA:
    text = "UPCA";
    break;

	case TWBT_UPCE:
    text = "UPCE";
    break;

	case TWBT_EAN8:
    text = "EAN8";
    break;

	case TWBT_EAN13:
    text = "EAN13";
    break;

	case TWBT_POSTNET:
    text = "POSTNET";
    break;

	case TWBT_PDF417:
    text = "PDF417";
    break;

	case TWBT_2OF5INDUSTRIAL:
    text = "2of5 industrial";
    break;

	case TWBT_2OF5MATRIX:
    text = "2of5 matrix";
    break;

	case TWBT_2OF5DATALOGIC:
    text = "2of5 datalogic";
    break;

	case TWBT_2OF5IATA:
    text = "2of5 IATA";
    break;

	case TWBT_3OF9FULLASCII:
    text = "3of9 fullASCII";
    break;

	case TWBT_CODABARWITHSTARTSTOP:
    text = "Codabar with start stop";
    break;

	case TWBT_MAXICODE:
    text = "MAXICODE";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWBT 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert ICAP_COMPRESSION values to String
const char* convertICAP_COMPRESSION_toString(const TW_UINT16 _unItem)
{
  const char* text;

  switch(_unItem)
  {
  case TWCP_NONE:
    text = "None";
    break;

  case TWCP_PACKBITS:
    text = "PACKBITS";
    break;

  case TWCP_GROUP31D:
    text = "GROUP31D";
    break;

  case TWCP_GROUP31DEOL:
    text = "GROUP31DEOL";
    break;

  case TWCP_GROUP32D:
    text = "GROUP32D";
    break;

  case TWCP_GROUP4:
    text = "GROUP4";
    break;

  case TWCP_JPEG:
    text = "JPEG";
    break;

  case TWCP_LZW:
    text = "LZW";
    break;

  case TWCP_JBIG:
    text = "JBIG";
    break;

  case TWCP_PNG:
    text = "PNG";
    break;

  case TWCP_RLE4:
    text = "RLE4";
    break;

  case TWCP_RLE8:
    text = "RLE8";
    break;

  case TWCP_BITFIELDS:
    text = "BITFIELDS";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWCP 0x:%4X", _unItem);
      text = buff;
    }
    break;
  }

  return text;
}


//////////////////////////////////////////////////////////////////////////////
// Convert CAP_CAMERASIDE values to String
const char* convertCAP_CAMERASIDE_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWCS_BOTH:
    text = "Both";
    break;

	case TWCS_TOP:
    text = "Top";
    break;

	case TWCS_BOTTOM:
    text = "Bottom";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWCS 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert TWEI_PAGESIDE values to String
const char* convertTWEI_PAGESIDE_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWCS_TOP:
    text = "Front";
    break;

	case TWCS_BOTTOM:
    text = "Back";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWCS 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert CAP_CLEARBUFFERS values to String
const char* convertCAP_CLEARBUFFERS_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWCB_AUTO:
    text = "Auto";
    break;

	case TWCB_CLEAR:
    text = "Clear";
    break;

	case TWCB_NOCLEAR:
    text = "No Clear";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWCB 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert CAP_DEVICEEVENT values to String
const char* convertCAP_DEVICEEVENT_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWDE_CUSTOMEVENTS:
    text = "Custom Events";
    break;

	case TWDE_CHECKAUTOMATICCAPTURE:
    text = "Check Automatic Capture";
    break;

	case TWDE_CHECKBATTERY:
    text = "Check Battery";
    break;

	case TWDE_CHECKDEVICEONLINE:
    text = "Check Device Online";
    break;

	case TWDE_CHECKFLASH:
    text = "Check Flash";
    break;

	case TWDE_CHECKPOWERSUPPLY:
    text = "Check Power Supply";
    break;

	case TWDE_CHECKRESOLUTION:
    text = "Check Resolution";
    break;

	case TWDE_DEVICEADDED:
    text = "Device Added";
    break;

	case TWDE_DEVICEOFFLINE:
    text = "Device Offline";
    break;

	case TWDE_DEVICEREADY:
    text = "Device Ready";
    break;

	case TWDE_DEVICEREMOVED:
    text = "Device Removed";
    break;

	case TWDE_IMAGECAPTURED:
    text = "Image Captured";
    break;

	case TWDE_IMAGEDELETED:
    text = "Image Deleted";
    break;

	case TWDE_PAPERDOUBLEFEED:
    text = "Paper Doublefeed";
    break;

	case TWDE_PAPERJAM:
    text = "Paperjam";
    break;

	case TWDE_LAMPFAILURE:
    text = "Lamp Failure";
    break;

	case TWDE_POWERSAVE:
    text = "Power Save";
    break;

	case TWDE_POWERSAVENOTIFY:
    text = "Power Save Notify";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWDE 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert TWEI_DESKEWSTATUS values to String
const char* convertTWEI_DESKEWSTATUS_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWDSK_SUCCESS:
    text = "Success";
    break;

	case TWDSK_REPORTONLY:
    text = "Report Only";
    break;

	case TWDSK_FAIL:
    text = "Fail";
    break;

	case TWDSK_DISABLED:
    text = "Disabled";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWDSK 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert CAP_DUPLEX values to String
const char* convertCAP_DUPLEX_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWDX_NONE:
    text = "None";
    break;

	case TWDX_1PASSDUPLEX:
    text = "1 Pass Duplex";
    break;

	case TWDX_2PASSDUPLEX:
    text = "2 Pass Duplex";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWDX 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert CAP_FEEDERALIGNMENT values to String
const char* convertCAP_FEEDERALIGNMENT_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWFA_NONE:
    text = "None";
    break;

	case TWFA_LEFT:
    text = "Left";
    break;

	case TWFA_CENTER:
    text = "Center";
    break;

	case TWFA_RIGHT:
    text = "Right";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWFA 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert ICAP_FEEDERTYPE values to String
const char* convertICAP_FEEDERTYPE_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWFE_GENERAL:
    text = "General";
    break;

	case TWFE_PHOTO:
    text = "Photo";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWFE 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Cover ICAP_IMAGEFILEFORMAT values to String
const char* convertICAP_IMAGEFILEFORMAT_toString(const TW_UINT16 _unItem)
{
  const char* text;

  switch(_unItem)
  {
  case TWFF_TIFF:
    text = "tiff";
    break;

  case TWFF_PICT:
    text = "pict";
    break;

  case TWFF_BMP:
    text = "bmp";
    break;

  case TWFF_XBM:
    text = "xbm";
    break;

  case TWFF_JFIF:
    text = "jpeg";
    break;

  case TWFF_FPX:
    text = "fpx";
    break;

  case TWFF_TIFFMULTI:
    text = "multi image tiff";
    break;

  case TWFF_PNG:
    text = "png";
    break;

  case TWFF_SPIFF:
    text = "spiff";
    break;

  case TWFF_EXIF:
    text = "exif";
    break;

  case TWFF_PDF:
    text = "PDF";
    break;

  case TWFF_JP2:
    text = "JP2";
    break;

  case TWFF_JPN:
    text = "JPN";
    break;

  case TWFF_JPX:
    text = "JPX";
    break;

  case TWFF_DEJAVU:
    text = "DEJAVU";
    break;

  case TWFF_PDFA:
    text = "PDF-A1";/* 2.0 Adobe PDF/A, Version 1*/
    break;

  case TWFF_PDFA2:
    text = "PDF-A2";/* 2.1 Adobe PDF/A, Version 2*/
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWFF 0x:%4X", _unItem);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Cover ICAP_IMAGEFILEFORMAT values to String
const char* convertICAP_IMAGEFILEFORMAT_toExt(const TW_UINT16 _unItem)
{
  const char* text;

  switch(_unItem)
  {
  case TWFF_PICT:
    text = ".pict";
    break;

  case TWFF_BMP:
    text = ".bmp";
    break;

  case TWFF_XBM:
    text = ".xbm";
    break;

  case TWFF_JFIF:
    text = ".jpeg";
    break;

  case TWFF_FPX:
    text = ".fpx";
    break;

  case TWFF_TIFF:
  case TWFF_TIFFMULTI:
    text = ".tiff";
    break;

  case TWFF_PNG:
    text = ".png";
    break;

  case TWFF_SPIFF:
    text = ".spiff";
    break;

  case TWFF_EXIF:
    text = ".exif";
    break;

  case TWFF_JP2:
    text = ".jp2";
    break;

  case TWFF_JPN:
    text = ".jpn";
    break;

  case TWFF_JPX:
    text = ".jpx";
    break;

  case TWFF_DEJAVU:
    text = ".dejavu";
    break;

  case TWFF_PDF:
  case TWFF_PDFA:
  case TWFF_PDFA2:
    text = ".pdf";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, ".0x:%4X", _unItem);//Unknown TWFF 
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert ICAP_FLASHUSED2 values to String
const char* convertICAP_FLASHUSED2_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWFL_NONE:
    text = "None";
    break;

	case TWFL_OFF:
    text = "Off";
    break;

	case TWFL_ON:
    text = "On";
    break;

	case TWFL_AUTO:
    text = "Auto";
    break;

	case TWFL_REDEYE:
    text = "Redeye";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWFL 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert CAP_FEEDERORDER values to String
const char* convertCAP_FEEDERORDER_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWFO_FIRSTPAGEFIRST:
    text = "First Page First";
    break;

	case TWFO_LASTPAGEFIRST:
    text = "Last Page First";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWFO 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert CAP_FEEDERPOCKET values to String
const char* convertCAP_FEEDERPOCKET_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWFP_POCKETERROR:
    text = "Pocket Error";
    break;

	case TWFP_POCKET1:
    text = "Pocket 1";
    break;

	case TWFP_POCKET2:
    text = "Pocket 2";
    break;

	case TWFP_POCKET3:
    text = "Pocket 3";
    break;

	case TWFP_POCKET4:
    text = "Pocket 4";
    break;

	case TWFP_POCKET5:
    text = "Pocket 5";
    break;

	case TWFP_POCKET6:
    text = "Pocket 6";
    break;

	case TWFP_POCKET7:
    text = "Pocket 7";
    break;

	case TWFP_POCKET8:
    text = "Pocket 8";
    break;

	case TWFP_POCKET9:
    text = "Pocket 9";
    break;

	case TWFP_POCKET10:
    text = "Pocket 10";
    break;

	case TWFP_POCKET11:
    text = "Pocket 11";
    break;

	case TWFP_POCKET12:
    text = "Pocket 12";
    break;

	case TWFP_POCKET13:
    text = "Pocket 13";
    break;

	case TWFP_POCKET14:
    text = "Pocket 14";
    break;

	case TWFP_POCKET15:
    text = "Pocket 15";
    break;

	case TWFP_POCKET16:
    text = "Pocket 16";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWFP 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert ICAP_FLIPROTATION values to String
const char* convertICAP_FLIPROTATION_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWFR_BOOK:
    text = "Book";
    break;

	case TWFR_FANFOLD:
    text = "Fanfold";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWFR 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert ICAP_FILTER values to String
const char* convertICAP_FILTER_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWFT_RED:
    text = "Red";
    break;

	case TWFT_GREEN:
    text = "Green";
    break;

	case TWFT_BLUE:
    text = "Blue";
    break;

	case TWFT_NONE:
    text = "None";
    break;

	case TWFT_WHITE:
    text = "White";
    break;

	case TWFT_CYAN:
    text = "Cyan";
    break;

	case TWFT_MAGENTA:
    text = "Magenta";
    break;

	case TWFT_YELLOW:
    text = "Yellow";
    break;

	case TWFT_BLACK:
    text = "Black";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWFT 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert TW_FILESYSTEM values to String
const char* convertTWEI_FILESYSTEM_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWFY_CAMERA:
    text = "Camera";
    break;

	case TWFY_CAMERATOP:
    text = "Camera Top";
    break;

	case TWFY_CAMERABOTTOM:
    text = "Camera Bottom";
    break;

	case TWFY_CAMERAPREVIEW:
    text = "Camera Preview";
    break;

	case TWFY_DOMAIN:
    text = "Domain";
    break;

	case TWFY_HOST:
    text = "Host";
    break;

	case TWFY_DIRECTORY:
    text = "Directory";
    break;

	case TWFY_IMAGE:
    text = "Image";
    break;

	case TWFY_UNKNOWN:
    text = "Unknown";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWFY 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert ICAP_ICCPROFILE values to String
const char* convertICAP_ICCPROFILE_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWIC_NONE:
    text = "None";
    break;

	case TWIC_LINK:
    text = "Link";
    break;

	case TWIC_EMBED:
    text = "Embed";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWIC 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert ICAP_IMAGEFILTER values to String
const char* convertICAP_IMAGEFILTER_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWIF_NONE:
    text = "None";
    break;

	case TWIF_AUTO:
    text = "Auto";
    break;

	case TWIF_LOWPASS:
    text = "Low Pass";
    break;

	case TWIF_BANDPASS:
    text = "Band Pass";
    break;

	case TWIF_HIGHPASS:
    text = "High Pass";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWIF 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert ICAP_IMAGEMERGE values to String
const char* convertICAP_IMAGEMERGE_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWIM_NONE:
    text = "None";
    break;

	case TWIM_FRONTONTOP:
    text = "Front on Top";
    break;

	case TWIM_FRONTONBOTTOM:
    text = "Front on Bottom";
    break;

	case TWIM_FRONTONLEFT:
    text = "Front on Left";
    break;

	case TWIM_FRONTONRIGHT:
    text = "Front on Right";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWIM 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert CAP_JOBCONTROL values to String
const char* convertCAP_JOBCONTROL_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWJC_NONE:
    text = "None";
    break;

	case TWJC_JSIC:
    text = "JSIC";
    break;

	case TWJC_JSIS:
    text = "JSIS";
    break;

	case TWJC_JSXC:
    text = "JSXC";
    break;

	case TWJC_JSXS:
    text = "JSXS";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWJC 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert ICAP_JPEGQUALITY values to String
const char* convertICAP_JPEGQUALITY_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWJQ_UNKNOWN:
    text = "Unknown";
    break;

	case TWJQ_LOW:
    text = "Low";
    break;

	case TWJQ_MEDIUM:
    text = "Medium";
    break;

	case TWJQ_HIGH:
    text = "High";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "%d", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert ICAP_LIGHTPATH values to String
const char* convertICAP_LIGHTPATH_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWLP_REFLECTIVE:
    text = "Reflective";
    break;

	case TWLP_TRANSMISSIVE:
    text = "Transmissive";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWLP 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert ICAP_LIGHTSOURCE values to String
const char* convertICAP_LIGHTSOURCE_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWLS_RED:
    text = "Red";
    break;

	case TWLS_GREEN:
    text = "Green";
    break;

	case TWLS_BLUE:
    text = "Blue";
    break;

	case TWLS_NONE:
    text = "None";
    break;

	case TWLS_WHITE:
    text = "White";
    break;

	case TWLS_UV:
    text = "UV";
    break;

	case TWLS_IR:
    text = "IR";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWLS 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert TWEI_MAGTYPE values to String
const char* convertTWEI_MAGTYPE_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWMD_MICR:
    text = "MICR";
    break;

	case TWMD_RAW:
    text = "RAW";
    break;

	case TWMD_INVALID:
    text = "Invalid";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWMD 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert ICAP_NOISEFILTER values to String
const char* convertICAP_NOISEFILTER_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWNF_NONE:
    text = "None";
    break;

	case TWNF_AUTO:
    text = "Auto";
    break;

	case TWNF_LONEPIXEL:
    text = "Low Pixel";
    break;

	case TWNF_MAJORITYRULE:
    text = "Majoriry Rule";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWNF 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert ICAP_ORIENTATION values to String
const char* convertICAP_ORIENTATION_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
//	case TWOR_ROT0:
//    text = "";
//    break;

	case TWOR_ROT90:
    text = "Orientation 90";
    break;

	case TWOR_ROT180:
    text = "Orientation 180";
    break;

//	case TWOR_ROT270:
//    text = "";
//    break;

	case TWOR_PORTRAIT:
    text = "Portrait";
    break;

	case TWOR_LANDSCAPE:
    text = "Landscape";
    break;

	case TWOR_AUTO:
    text = "Auto";
    break;

	case TWOR_AUTOTEXT:
    text = "Auto Text";
    break;

	case TWOR_AUTOPICTURE:
    text = "Auto Picture";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWOR 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert ICAP_OVERSCAN values to String
const char* convertICAP_OVERSCAN_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWOV_NONE:
    text = "None";
    break;

	case TWOV_AUTO:
    text = "Auto";
    break;

	case TWOV_TOPBOTTOM:
    text = "Top Bottom";
    break;

	case TWOV_LEFTRIGHT:
    text = "Left Right";
    break;

	case TWOV_ALL:
    text = "All";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWOV 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert ICAP_PLANARCHUNKY values to String
const char* convertICAP_PLANARCHUNKY_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWPC_CHUNKY:
    text = "Chunky";
    break;

	case TWPC_PLANAR:
    text = "Planar";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWPC 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert TWEI_PATCHCODE values to String
const char* convertTWEI_PATCHCODE_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWPCH_PATCH1:
    text = "Patch 1";
    break;

	case TWPCH_PATCH2:
    text = "Patch 2";
    break;

	case TWPCH_PATCH3:
    text = "Patch 3";
    break;

	case TWPCH_PATCH4:
    text = "Patch 4";
    break;

	case TWPCH_PATCH6:
    text = "Patch 6";
    break;

	case TWPCH_PATCHT:
    text = "Patch T";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWPCH 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert ICAP_PIXELFLAVOR values to String
const char* convertICAP_PIXELFLAVOR_toString(const TW_UINT16 _unItem)
{
  const char* text;

  switch(_unItem)
  {
  case TWPF_CHOCOLATE:
    text = "Chocolate";
    break;

  case TWPF_VANILLA:
    text = "Vanilla";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWPF 0x:%4X", _unItem);
      text = buff;
    }
    break;
  }

  return text;
}


//////////////////////////////////////////////////////////////////////////////
// Convert CAP_PRINTERMODE values to String
const char* convertCAP_PRINTERMODE_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWPM_SINGLESTRING:
    text = "Single String";
    break;

	case TWPM_MULTISTRING:
    text = "Multi String";
    break;

	case TWPM_COMPOUNDSTRING:
    text = "Compound String";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWPM 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert CAP_PRINTER values to String
const char* convertCAP_PRINTER_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWPR_IMPRINTERTOPBEFORE:
    text = "Imprinter Top Before";
    break;

	case TWPR_IMPRINTERTOPAFTER:
    text = "Imprinter Top After";
    break;

	case TWPR_IMPRINTERBOTTOMBEFORE:
    text = "Imprinter Bottom Before";
    break;

	case TWPR_IMPRINTERBOTTOMAFTER:
    text = "Imprinter Bottom After";
    break;

	case TWPR_ENDORSERTOPBEFORE:
    text = "Endorser Top Before";
    break;

	case TWPR_ENDORSERTOPAFTER:
    text = "Endorser Top After";
    break;

	case TWPR_ENDORSERBOTTOMBEFORE:
    text = "Endorser Bottom Before";
    break;

	case TWPR_ENDORSERBOTTOMAFTER:
    text = "Endorser Bottom After";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWPR 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert CAP_POWERSUPPLY values to String
const char* convertCAP_POWERSUPPLY_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWPS_EXTERNAL:
    text = "External";
    break;

	case TWPS_BATTERY:
    text = "Battery";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWPS 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert ICAP_PIXELTYPE values to String
const char* convertICAP_PIXELTYPE_toString(const TW_UINT16 _unItem)
{
  const char* text;

  switch(_unItem)
  {
  case TWPT_BW:
    text = "BW";
    break;

  case TWPT_GRAY:
    text = "GRAY";
    break;

  case TWPT_RGB:
    text = "RGB";
    break;

  case TWPT_PALETTE:
    text = "PALETTE";
    break;

  case TWPT_CMY:
    text = "CMY";
    break;

  case TWPT_CMYK:
    text = "CMYK";
    break;

  case TWPT_YUV:
    text = "YUV";
    break;

  case TWPT_YUVK:
    text = "YUVK";
    break;

  case TWPT_CIEXYZ:
    text = "CIEXYZ";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWPT 0x:%4X", _unItem);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert CAP_SEGMENTED values to String
const char* convertCAP_SEGMENTED_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWSG_NONE:
    text = "None";
    break;

	case TWSG_AUTO:
    text = "Auto";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWSG 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Convert ICAP_SUPPORTEDSIZES values to String
const char* convertICAP_SUPPORTEDSIZES_toString(const TW_UINT16 InfoID)
{
  const char* text;

  switch(InfoID)
  {
	case TWSS_NONE:
    text = "None";
    break;

	case TWSS_A4:
    text = "A4";
    break;

	case TWSS_JISB5:
    text = "JIS B5";
    break;

	case TWSS_USLETTER:
    text = "US Letter";
    break;

	case TWSS_USLEGAL:
    text = "US Legal";
    break;

	case TWSS_A5:
    text = "A5";
    break;

	case TWSS_ISOB4:
    text = "ISO B4";
    break;

	case TWSS_ISOB6:
    text = "ISO B6";
    break;

	case TWSS_USLEDGER:
    text = "US Ledger";
    break;

	case TWSS_USEXECUTIVE:
    text = "US Executive";
    break;

	case TWSS_A3:
    text = "A3";
    break;

	case TWSS_ISOB3:
    text = "ISO B3";
    break;

	case TWSS_A6:
    text = "A6";
    break;

	case TWSS_C4:
    text = "C4";
    break;

	case TWSS_C5:
    text = "C5";
    break;

	case TWSS_C6:
    text = "C6";
    break;

	case TWSS_4A0:
    text = "4A0";
    break;

	case TWSS_2A0:
    text = "2A0";
    break;

	case TWSS_A0:
    text = "A0";
    break;

	case TWSS_A1:
    text = "A1";
    break;

	case TWSS_A2:
    text = "A2";
    break;

	case TWSS_A7:
    text = "A7";
    break;

	case TWSS_A8:
    text = "A8";
    break;

	case TWSS_A9:
    text = "A9";
    break;

	case TWSS_A10:
    text = "A10";
    break;

	case TWSS_ISOB0:
    text = "ISO B0";
    break;

	case TWSS_ISOB1:
    text = "ISO B1";
    break;

	case TWSS_ISOB2:
    text = "ISO B2";
    break;

	case TWSS_ISOB5:
    text = "ISO B5";
    break;

	case TWSS_ISOB7:
    text = "ISO B7";
    break;

	case TWSS_ISOB8:
    text = "ISO B8";
    break;

	case TWSS_ISOB9:
    text = "ISO B9";
    break;

	case TWSS_ISOB10:
    text = "ISO B10";
    break;

	case TWSS_JISB0:
    text = "JIS B0";
    break;

	case TWSS_JISB1:
    text = "JIS B1";
    break;

	case TWSS_JISB2:
    text = "JIS B2";
    break;

	case TWSS_JISB3:
    text = "JIS B3";
    break;

	case TWSS_JISB4:
    text = "JIS B4";
    break;

	case TWSS_JISB6:
    text = "JIS B6";
    break;

	case TWSS_JISB7:
    text = "JIS B7";
    break;

	case TWSS_JISB8:
    text = "JIS B8";
    break;

	case TWSS_JISB9:
    text = "JIS B9";
    break;

	case TWSS_JISB10:
    text = "JIS B10";
    break;

	case TWSS_C0:
    text = "C0";
    break;

	case TWSS_C1:
    text = "C1";
    break;

	case TWSS_C2:
    text = "C2";
    break;

	case TWSS_C3:
    text = "C3";
    break;

	case TWSS_C7:
    text = "C7";
    break;

	case TWSS_C8:
    text = "C8";
    break;

	case TWSS_C9:
    text = "C9";
    break;

	case TWSS_C10:
    text = "C10";
    break;

	case TWSS_USSTATEMENT:
    text = "US Statement";
    break;

	case TWSS_BUSINESSCARD:
    text = "Business Card";
    break;

	case TWSS_MAXSIZE:
    text = "MAX size";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWSS 0x:%4X", InfoID);
      text = buff;
    }
    break;
  }

  return text;
}

//////////////////////////////////////////////////////////////////////////////
// Covert ICAP_XFERMECH values to String
const char* convertICAP_XFERMECH_toString(const TW_UINT16 _unItem)
{
  const char* text;

  switch(_unItem)
  {
  case TWSX_NATIVE:
    text = "Native";
    break;

  case TWSX_FILE:
    text = "File";
    break;

  case TWSX_MEMORY:
    text = "Memory";
    break;

  case TWSX_MEMFILE:
    text = "Memory of File";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWSX 0x:%4X", _unItem);
      text = buff;
    }
    break;
  }

  return text;
}


//////////////////////////////////////////////////////////////////////////////
// Covert ICAP_UNITS values to String
const char* convertICAP_UNITS_toString(const TW_UINT16 _unItem)
{
  const char* text;

  switch(_unItem)
  {
  case TWUN_INCHES:
    text = "Inches";
    break;

  case TWUN_CENTIMETERS:
    text = "Centimeters";
    break;

  case TWUN_PICAS:
    text = "Picas";
    break;

  case TWUN_POINTS:
    text = "Points";
    break;

  case TWUN_TWIPS:
    text = "Twips";
    break;

  case TWUN_PIXELS:
    text = "Pixels";
    break;

  case TWUN_MILLIMETERS:
    text = "Millimeters";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unknown TWUN 0x:%4X", _unItem);
      text = buff;
    }
    break;
  }

  return text;
}


const char* convertExtImageInfoItem_toString(TW_INFO &ImgInfo)
{
   char * buff = nextTempBuffer();

   if(TWRC_SUCCESS == ImgInfo.CondCode)
   {
      switch(ImgInfo.ItemType)
      {
       case TWTY_INT8:
       case TWTY_INT16:
       case TWTY_INT32:
       case TWTY_UINT8:
       case TWTY_UINT32:
       case TWTY_UINT16:
       case TWTY_BOOL:
         // If the size of the data is larger than 4 bytes (regardless of 32bit or 64bit OS) then the data
         // is no longer an array of data but a handle to the data.
         // If the data does fit inside then it is possible to have an array of data.
         if(getTWTYsize(ImgInfo.ItemType)*ImgInfo.NumItems <= sizeof(TW_UINT32))
         {
            char        *pTemp    = buff;
            const char  *pItem    = NULL;
            int          TempSize = TEMPBUFSIZE;

            for( int nItem=0; nItem<ImgInfo.NumItems; nItem++)
            {
               switch(ImgInfo.ItemType)
               {
                case TWTY_INT8:
                  SSNPRINTF(pTemp, TempSize, TempSize, "%d ", ((TW_INT8*)&ImgInfo.Item)[nItem]);
                  break;

                case TWTY_INT16:
                  SSNPRINTF(pTemp, TempSize, TempSize, "%d ", ((TW_INT16*)&ImgInfo.Item)[nItem]);
                  break;

                case TWTY_INT32:
                  SSNPRINTF(pTemp, TempSize, TempSize, "%d ", ((TW_INT32*)&ImgInfo.Item)[nItem]);
                  break;

                case TWTY_UINT8:
                  SSNPRINTF(pTemp, TempSize, TempSize, "%u ", ((TW_UINT8*)&ImgInfo.Item)[nItem]);
                  break;

                case TWTY_UINT32:
                  pItem = convertEI_Item_toString(ImgInfo.InfoID, ((TW_UINT32*)&ImgInfo.Item)[nItem]);
                  if(pItem)
                  {
                    SSNPRINTF(pTemp, TempSize, TempSize, "%s ", pItem);
                  }
                  else
                  {
                    SSNPRINTF(pTemp, TempSize, TempSize, "%u ", ((TW_UINT32*)&ImgInfo.Item)[nItem]);
                  }
                  break;

                case TWTY_UINT16:
                  pItem = convertEI_Item_toString(ImgInfo.InfoID, ((TW_UINT16*)&ImgInfo.Item)[nItem]);
                  if(pItem)
                  {
                    SSNPRINTF(pTemp, TempSize, TempSize, "%s ", pItem);
                  }
                  else
                  {
                    SSNPRINTF(pTemp, TempSize, TempSize, "%u ", ((TW_UINT16*)&ImgInfo.Item)[nItem]);
                  }
                  break;

                case TWTY_BOOL:
                  SSNPRINTF(pTemp, TempSize, TempSize, "%s ", ((TW_BOOL*)&ImgInfo.Item)[nItem]? "True" : "False");
                  break;
               }
               TempSize -= (int)strlen(pTemp);
               pTemp += strlen(pTemp);
               if(TempSize <= 0)
               {
                  break;
               }
            }
         }
         else
         {
            //Did not fit into the INT32
            char        *pTemp    = buff;
            const char  *pItem    = NULL;
            int          TempSize = TEMPBUFSIZE;

            BYTE * pBData = (BYTE *)_DSM_LockMemory((TW_HANDLE)ImgInfo.Item);

            for( int nItem=0; nItem<ImgInfo.NumItems; nItem++)
            {
               switch(ImgInfo.ItemType)
               {
                case TWTY_INT8:
                  SSNPRINTF(pTemp, TempSize, TempSize, "%d ", ((TW_INT8*)pBData)[nItem]);
                  break;

                case TWTY_INT16:
                  SSNPRINTF(pTemp, TempSize, TempSize, "%d ", ((TW_INT16*)pBData)[nItem]);
                  break;

                case TWTY_INT32:
                  SSNPRINTF(pTemp, TempSize, TempSize, "%d ", ((TW_INT32*)pBData)[nItem]);
                  break;

                case TWTY_UINT8:
                  SSNPRINTF(pTemp, TempSize, TempSize, "%u ", ((TW_UINT8*)pBData)[nItem]);
                  break;

                case TWTY_UINT32:
                  pItem = convertEI_Item_toString(ImgInfo.InfoID, ((TW_UINT32*)pBData)[nItem]);
                  if(pItem)
                  {
                    SSNPRINTF(pTemp, TempSize, TempSize, "%s ", pItem);
                  }
                  else
                  {
                    SSNPRINTF(pTemp, TempSize, TempSize, "%u ", ((TW_UINT32*)pBData)[nItem]);
                  }
                  break;

                case TWTY_UINT16:
                  pItem = convertEI_Item_toString(ImgInfo.InfoID, ((TW_UINT16*)pBData)[nItem]);
                  if(pItem)
                  {
                    SSNPRINTF(pTemp, TempSize, TempSize, "%s ", pItem);
                  }
                  else
                  {
                    SSNPRINTF(pTemp, TempSize, TempSize, "%u ", ((TW_UINT16*)pBData)[nItem]);
                  }
                  break;

                case TWTY_BOOL:
                  SSNPRINTF(pTemp, TempSize, TempSize, "%s ", ((TW_BOOL*)pBData)[nItem]? "True" : "False");
                  break;
               }
               TempSize -= (int)strlen(pTemp);
               pTemp += strlen(pTemp);
               if(TempSize <= 0)
               {
                  break;
               }
            }

             _DSM_UnlockMemory((TW_HANDLE)ImgInfo.Item);
         }
         break;

       case TWTY_STR32:
       case TWTY_STR64:
       case TWTY_STR128:
       case TWTY_STR255:
       case TWTY_STR1024:
         {
         char *chTest = (char *)_DSM_LockMemory((TW_HANDLE)ImgInfo.Item);

         SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "%.*s", getTWTYsize(ImgInfo.ItemType), chTest);

         _DSM_UnlockMemory((TW_HANDLE)ImgInfo.Item);
         }
         break;

       case TWTY_HANDLE:
         {
         // Need to look at the item to know how to handle :) this 
         // It might be a handle to a string or a handle to a chunk of memory
         SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Handle 0x:%8X", (TW_HANDLE)ImgInfo.Item);
         }
         break;

       case TWTY_FRAME:
         {
         TW_FRAME *pFrame = (TW_FRAME *)_DSM_LockMemory((TW_HANDLE)ImgInfo.Item);

         SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "%0.2f  %.2f  %.2f  %.2f",
                   FIX32ToFloat(pFrame->Left), FIX32ToFloat(pFrame->Top),
                   FIX32ToFloat(pFrame->Right), FIX32ToFloat(pFrame->Bottom));

         _DSM_UnlockMemory((TW_HANDLE)ImgInfo.Item);
         }
         break;

       default:
          SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unsupporetd Type 0x:%4X", ImgInfo.ItemType);
         break;
      }
   }
   else if(TWRC_INFONOTSUPPORTED == ImgInfo.CondCode)
   {
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Unsupporetd");
   }
   else if(TWRC_DATANOTAVAILABLE == ImgInfo.CondCode)
   {
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "Data Not Available");
   }
   else
   {
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "unknown failure %d", ImgInfo.CondCode);
   }

   return buff;
}

const char* convertReturnCode_toString(const TW_UINT16 _unItem)
{
  const char* text;

  switch(_unItem)
  {
  case TWRC_SUCCESS:
    text = "TWRC_SUCCESS";
    break;

  case TWRC_FAILURE:
    text = "TWRC_FAILURE";
    break;

  case TWRC_CHECKSTATUS:
    text = "TWRC_CHECKSTATUS";
    break;

  case TWRC_CANCEL:
    text = "TWRC_CANCEL";
    break;

  case TWRC_DSEVENT:
    text = "TWRC_DSEVENT";
    break;

  case TWRC_NOTDSEVENT:
    text = "TWRC_NOTDSEVENT";
    break;

  case TWRC_XFERDONE:
    text = "TWRC_XFERDONE";
    break;

  case TWRC_ENDOFLIST:
    text = "TWRC_ENDOFLIST";
    break;

  case TWRC_INFONOTSUPPORTED:
    text = "TWRC_INFONOTSUPPORTED";
    break;

  case TWRC_DATANOTAVAILABLE:
    text = "TWRC_DATANOTAVAILABLE";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "ReturnCode 0x:%4X",_unItem);
      text = buff;
    }
    break;
  }

  return text;
}

const char* convertConditionCode_toString(const TW_UINT16 _unItem)
{
  const char* text;

  switch(_unItem)
  {
  case TWCC_SUCCESS:
    text = "TWCC_SUCCESS";
    break;

  case TWCC_BUMMER:
    text = "TWCC_BUMMER";
    break;

  case TWCC_LOWMEMORY:
    text = "TWCC_LOWMEMORY";
    break;

  case TWCC_NODS:
    text = "TWCC_NODS";
    break;

  case TWCC_MAXCONNECTIONS:
    text = "TWCC_MAXCONNECTIONS";
    break;

  case TWCC_OPERATIONERROR:
    text = "TWCC_OPERATIONERROR";
    break;

  case TWCC_BADCAP:
    text = "TWCC_BADCAP";
    break;

  case TWCC_BADPROTOCOL:
    text = "TWCC_BADPROTOCOL";
    break;

  case TWCC_BADVALUE:
    text = "TWCC_BADVALUE";
    break;

  case TWCC_SEQERROR:
    text = "TWCC_SEQERROR";
    break;

  case TWCC_BADDEST:
    text = "TWCC_BADDEST";
    break;

  case TWCC_CAPUNSUPPORTED:
    text = "TWCC_CAPUNSUPPORTED";
    break;

  case TWCC_CAPBADOPERATION:
    text = "TWCC_CAPBADOPERATION";
    break;

  case TWCC_CAPSEQERROR:
    text = "TWCC_CAPSEQERROR";
    break;

  case TWCC_DENIED:
    text = "TWCC_DENIED";
    break;

  case TWCC_FILEEXISTS:
    text = "TWCC_FILEEXISTS";
    break;

  case TWCC_FILENOTFOUND:
    text = "TWCC_FILENOTFOUND";
    break;

  case TWCC_NOTEMPTY:
    text = "TWCC_NOTEMPTY";
    break;

  case TWCC_PAPERJAM:
    text = "TWCC_PAPERJAM";
    break;

  case TWCC_PAPERDOUBLEFEED:
    text = "TWCC_PAPERDOUBLEFEED";
    break;

  case TWCC_FILEWRITEERROR:
    text = "TWCC_FILEWRITEERROR";
    break;

  case TWCC_CHECKDEVICEONLINE:
    text = "TWCC_CHECKDEVICEONLINE";
    break;

  case TWCC_INTERLOCK:
    text = "TWCC_INTERLOCK";
    break;

  case TWCC_DAMAGEDCORNER:
    text = "TWCC_DAMAGEDCORNER";
    break;

  case TWCC_FOCUSERROR:
    text = "TWCC_FOCUSERROR";
    break;

  case TWCC_DOCTOOLIGHT:
    text = "TWCC_DOCTOOLIGHT";
    break;

  case TWCC_DOCTOODARK:
    text = "TWCC_DOCTOODARK";
    break;

  case TWCC_NOMEDIA:
    text = "TWCC_NOMEDIA";
    break;

  default:
    {
      char * buff = nextTempBuffer();
      SSNPRINTF(buff, TEMPBUFSIZE, TEMPBUFSIZE, "ConditionCode 0x:%4X",_unItem);
      text = buff;
    }
    break;
  }

  return text;
}
