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
 * @file TwainString.h
 * Convert TWAIN Constants to Strings
 * @author JFL Peripheral Solutions Inc.
 * @date Dec 2008
 */


#ifndef __TWAINSTRING_H__
#define __TWAINSTRING_H__

#pragma once

#include "CommonTWAIN.h"

/**
* converts the integer CAP value into string form
* @param[in] _unCap the cap
* @return the equivalent cap string
*/
const char* convertCAP_toString(const TW_UINT16 _unCap);

/**
* converts the integer CAP transfer method into string form
* @param[in] _unCap the cap
* @param[in] _unItem the value
* @param[in] _unType the TWAIN Type of the item
* @return the equivalent string
*/
const char* convertCAP_Item_toString(const TW_UINT16 _unCap, const TW_UINT32 _unItem, const TW_UINT16 _unType);

/**
* converts the integer CAP transfer method into string form
* @param[in] _unEI the ExtendedImageInfo
* @param[in] _unItem the value
* @return the equivalent string, or NULL if not found
*/
const char* convertEI_Item_toString(const TW_UINT16 _unEI, const TW_UINT32 _unItem);

/**
* converts the integer CAP transfer method into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_XFERMECH_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP unit into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_UNITS_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Pixel type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_PIXELTYPE_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Pixel flavor value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_PIXELFLAVOR_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Image File format type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_IMAGEFILEFORMAT_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Image File format type value into .xxx extention string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_IMAGEFILEFORMAT_toExt(const TW_UINT16 _unItem);

/**
* converts the integer CAP Compression type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_COMPRESSION_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Alarms type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertCAP_ALARMS_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP AutoSize type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_AUTOSIZE_toString(const TW_UINT16 _unItem);

/**
* converts the integer ICAP BarCode Search Mode type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_BARCODESEARCHMODE_toString(const TW_UINT16 _unItem);

/**
* converts the integer ICAP Bit Order type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_BITORDER_toString(const TW_UINT16 _unItem);

/**
* converts the integer ICAP Auto discard blank pages type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_AUTODISCARDBLANKPAGES_toString(const TW_UINT16 _unItem);

/**
* converts the integer ICAP Bitdepth Reduction type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_BITDEPTHREDUCTION_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Supported Barcode Types type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_SUPPORTEDBARCODETYPES_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Cameraside type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertCAP_CAMERASIDE_toString(const TW_UINT16 _unItem);

/**
* converts the integer TWEI Pageside type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertTWEI_PAGESIDE_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP ClearBuffers type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertCAP_CLEARBUFFERS_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Device Event type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertCAP_DEVICEEVENT_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Duplex type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertCAP_DUPLEX_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Feeder Alignment type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertCAP_FEEDERALIGNMENT_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Feeder Type type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_FEEDERTYPE_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Flash used2 type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_FLASHUSED2_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Feeder Order type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertCAP_FEEDERORDER_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Feeder Pocket type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertCAP_FEEDERPOCKET_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Flip Rotation type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_FLIPROTATION_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Filter type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_FILTER_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP ICC Profile type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_ICCPROFILE_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Image Filter type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_IMAGEFILTER_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Image Merge type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_IMAGEMERGE_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Job Control type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertCAP_JOBCONTROL_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP JPEG Quality type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_JPEGQUALITY_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Light Path type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_LIGHTPATH_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Light Source type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_LIGHTSOURCE_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Noise Filter type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_NOISEFILTER_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Orientation type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_ORIENTATION_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Overscan type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_OVERSCAN_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Planar Chunky type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_PLANARCHUNKY_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Printer Mode type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertCAP_PRINTERMODE_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Printer type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertCAP_PRINTER_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Power Supply type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertCAP_POWERSUPPLY_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Segmented type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertCAP_SEGMENTED_toString(const TW_UINT16 _unItem);

/**
* converts the integer CAP Supported Sizes type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertICAP_SUPPORTEDSIZES_toString(const TW_UINT16 _unItem);

/**
* converts the integer TWEI File System type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertTWEI_FILESYSTEM_toString(const TW_UINT16 _unItem);

/**
* converts the integer TWEI Barcode Rotation type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertTWEI_BARCODEROTATION_toString(const TW_UINT16 _unItem);

/**
* converts the integer TWEI Deskew Status type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertTWEI_DESKEWSTATUS_toString(const TW_UINT16 _unItem);

/**
* converts the integer TWEI MAG Type type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertTWEI_MAGTYPE_toString(const TW_UINT16 _unItem);

/**
* converts the integer TWEI Patch code type value into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertTWEI_PATCHCODE_toString(const TW_UINT16 _unItem);

/**
* converts the integer TWTY value into string form
* @param[in] _unItem the TWTY value
* @return the equivalent string
*/
const char* convertTWTY_toString(const TW_UINT16 _unItem);

/**
* retrieve the extended image info name for a given Info ID for the current image
* @param[in] InfoID the id to retrieve the info of
* @return string of the extended image info
*/
const char* convertExtImageInfoName_toString(int InfoID);

/**
* retrieve the extended image info value for a given Info
* @param[in] ImgInfo the TW_INFO to retrieve the extended image info of
* @return string of the extended image info value
*/
const char* convertExtImageInfoItem_toString(TW_INFO &ImgInfo);

/**
* converts the TWAIN Return Code into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertReturnCode_toString(const TW_UINT16 _unItem);

/**
* converts the TWAIN Condition Code into string form
* @param[in] _unItem the value
* @return the equivalent string
*/
const char* convertConditionCode_toString(const TW_UINT16 _unItem);

#endif // __TWAINSTRING_H__
