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
* @file CommonTWAIN.h
* Common defines and typedefs used by the DS
* @author TWAIN Working Group
* @date April 2007
*/

#ifndef __COMMONTWAIN_H__
#define __COMMONTWAIN_H__

#include "Common.h"

#include <string>
using namespace std;

typedef struct _TW_GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[ 8 ];
} TW_GUID;
/**
* @def kTWAIN_DSM_DLL_NAME
* File name of the DSM library.
*/

#ifdef TWH_CMP_MSC
  #ifdef TWH_64BIT
    #define kTWAIN_DSM_DLL_NAME "TWAINDSM.dll"
  #else
    #define kTWAIN_DSM_DLL_NAME "TWAINDSM.dll"
  #endif // #ifdef TWH_64BIT
#elif defined(TWH_CMP_GNU)
  #define kTWAIN_DSM_DLL_NAME "libtwaindsm.so"
#else
  #error Sorry, we don't recognize this system...
#endif




/**
* A commonly used conversion function for converting float to TW_FIX32.
* @param[in] floater the float value to change to TW_FIX32
* @return the value as TW_FIX32
*/
TW_FIX32 FloatToFIX32 (float floater);

/**
* A commonly used conversion function for converting TW_FIX32 to float.
* @param[in] _fix32 the TW_FIX32 value to change to float
* @return the value as float
*/
float FIX32ToFloat(const TW_FIX32& _fix32);


#ifdef TWH_CMP_MSC
#pragma pack (push, before_twain)
#pragma pack (2)
#else
#pragma pack(2)
#endif // TWH_CMP_MSC

// The following structures combinations are implimented and found in the TWAIN specifications
//              BOOL  INT8  INT16  INT32  UINT8  UINT16  UINT32  STR32  STR64  STR128  STR255  STR1024  UNI512  FIX32  FRAME
// OneValue      x           x      x             x       x       x             x       x                        x      x
// Array                                   x      x       x       x                                              x      x
// Enumeration   x           x                    x       x       x                     x                        x      x
// Range                     x      x             x       x                                                      x      


/**
* TW_ONEVALUE that holds a TW_STR32 item
*/
typedef struct {
   TW_UINT16  ItemType;                            /**< Assigned TWAIN Type TWTY_STR32 */
   TW_STR32   Item;                                /**< TW_STR32 value being passed */
} TW_ONEVALUE_STR32, FAR * pTW_ONEVALUE_STR32;     /**< Pointer to TW_ONEVALUE that holds a TW_STR32 item */

/**
* TW_ONEVALUE that holds a TW_STR64 item
*/
typedef struct {
   TW_UINT16  ItemType;                            /**< Assigned TWAIN Type TWTY_STR64 */
   TW_STR64   Item;                                /**< TW_STR32 value being passed */
} TW_ONEVALUE_STR64, FAR * pTW_ONEVALUE_STR64;     /**< Pointer to TW_ONEVALUE that holds a TW_STR32 item */

/**
* TW_ONEVALUE that holds a TW_STR128 item
*/
typedef struct {
   TW_UINT16  ItemType;                            /**< Assigned TWAIN Type TWTY_STR128 */
   TW_STR128  Item;                                /**< TW_STR128 value being passed */
} TW_ONEVALUE_STR128, FAR * pTW_ONEVALUE_STR128;   /**< Pointer to TW_ONEVALUE that holds a TW_STR128 item */

/**
* TW_ONEVALUE that holds a TW_STR255 item
*/
typedef struct {
   TW_UINT16  ItemType;                            /**< Assigned TWAIN Type TWTY_STR255 */
   TW_STR255  Item;                                /**< TW_STR255 value being passed */
} TW_ONEVALUE_STR255, FAR * pTW_ONEVALUE_STR255;   /**< Pointer to TW_ONEVALUE that holds a TW_STR255 item */

/**
* TW_ONEVALUE that holds a TW_FIX32 item
*/
typedef struct {
   TW_UINT16  ItemType;                            /**< Assigned TWAIN Type TWTY_FIX32 */
   TW_FIX32   Item;                                /**< TW_FIX32 value being passed */
} TW_ONEVALUE_FIX32, FAR * pTW_ONEVALUE_FIX32;     /**< Pointer to TW_ONEVALUE that holds a TW_FIX32 item */

/**
* TW_ONEVALUE that holds a TW_FRAME item
*/
typedef struct {
   TW_UINT16  ItemType;                            /**< Assigned TWAIN Type TWTY_FRAME */
   TW_FRAME   Item;                                /**< TW_FRAME structure being passed */
} TW_ONEVALUE_FRAME, FAR * pTW_ONEVALUE_FRAME;     /**< Pointer to TW_ONEVALUE that holds a TW_FRAME item */

/**
* TW_ARRAY that holds a TW_UINT8 item
*/
typedef struct {
   TW_UINT16  ItemType;                            /**< Assigned TWAIN Type TWTY_UINT8 */
   TW_UINT32  NumItems;                            /**< How many items in ItemList */
   TW_UINT8   ItemList[1];                         /**< Array of TW_UINT8 structures starts here */
} TW_ARRAY_UINT8, FAR * pTW_ARRAY_UINT8;           /**< Pointer to TW_ARRAY that holds a TW_UINT8 item */

/**
* TW_ARRAY that holds a TW_UINT16 item
*/
typedef struct {
   TW_UINT16  ItemType;                            /**< Assigned TWAIN Type TWTY_UINT16 */
   TW_UINT32  NumItems;                            /**< How many items in ItemList */
   TW_UINT16  ItemList[1];                         /**< Array of TW_UINT16 structures starts here */
} TW_ARRAY_UINT16, FAR * pTW_ARRAY_UINT16;         /**< Pointer to TW_ARRAY that holds a TW_UINT16 item */

/**
* TW_ARRAY that holds a TW_UINT32 item
*/
typedef struct {
   TW_UINT16  ItemType;                            /**< Assigned TWAIN Type TWTY_UINT32 */
   TW_UINT32  NumItems;                            /**< How many items in ItemList */
   TW_UINT32  ItemList[1];                         /**< Array of TW_UINT32 structures starts here */
} TW_ARRAY_UINT32, FAR * pTW_ARRAY_UINT32;         /**< Pointer to TW_ARRAY that holds a TW_UINT32 item */

/**
* TW_ARRAY that holds a TW_STR32 item
*/
typedef struct {
   TW_UINT16  ItemType;                            /**< Assigned TWAIN Type TWTY_STR32 */
   TW_UINT32  NumItems;                            /**< How many items in ItemList */
   TW_STR32   ItemList[1];                         /**< Array of TW_STR32 structures starts here */
} TW_ARRAY_STR32, FAR * pTW_ARRAY_STR32;           /**< Pointer to TW_ARRAY that holds a TW_STR32 item */

/**
* TW_ARRAY that holds a TW_FIX32 item
*/
typedef struct {
   TW_UINT16  ItemType;                            /**< Assigned TWAIN Type TWTY_FIX32 */
   TW_UINT32  NumItems;                            /**< How many items in ItemList */
   TW_FIX32   ItemList[1];                         /**< Array of TW_FIX32 structures starts here */
} TW_ARRAY_FIX32, FAR * pTW_ARRAY_FIX32;           /**< Pointer to TW_ARRAY that holds a TW_FIX32 item */

/**
* TW_ARRAY that holds a TW_FRAME item
*/
typedef struct {
   TW_UINT16  ItemType;                            /**< Assigned TWAIN Type TWTY_FRAME */
   TW_UINT32  NumItems;                            /**< How many items in ItemList */
   TW_FRAME   ItemList[1];                         /**< Array of TW_FRAME structures starts here */
} TW_ARRAY_FRAME, FAR * pTW_ARRAY_FRAME;           /**< Pointer to TW_ARRAY that holds a TW_FRAME item */

/**
* TW_ENUMERATION that holds a TW_BOOL item
*/
typedef struct {
   TW_UINT16  ItemType;                            /**< Assigned TWAIN Type TWTY_BOOL */
   TW_UINT32  NumItems;                            /**< How many items in ItemList */
   TW_UINT32  CurrentIndex;                        /**< Current value is in ItemList[CurrentIndex] */
   TW_UINT32  DefaultIndex;                        /**< Powerup value is in ItemList[DefaultIndex] */
   TW_BOOL    ItemList[1];                         /**< Array of ItemType values starts here       */
} TW_ENUMERATION_BOOL, FAR * pTW_ENUMERATION_BOOL; /**< Pointer to TW_ENUMERATION that holds an array TW_BOOL items */

/**
* TW_ENUMERATION that holds a TW_INT16 item
*/
typedef struct {
   TW_UINT16  ItemType;                            /**< Assigned TWAIN Type TWTY_INT16 */
   TW_UINT32  NumItems;                            /**< How many items in ItemList */
   TW_UINT32  CurrentIndex;                        /**< Current value is in ItemList[CurrentIndex] */
   TW_UINT32  DefaultIndex;                        /**< Powerup value is in ItemList[DefaultIndex] */
   TW_INT16   ItemList[1];                         /**< Array of ItemType values starts here       */
} TW_ENUMERATION_INT16, FAR * pTW_ENUMERATION_INT16;/**< Pointer to TW_ENUMERATION that holds an array TW_INT16 items */

/**
* TW_ENUMERATION that holds a TW_INT32 item
*/
typedef struct {
   TW_UINT16  ItemType;                            /**< Assigned TWAIN Type TWTY_INT32 */
   TW_UINT32  NumItems;                            /**< How many items in ItemList */
   TW_UINT32  CurrentIndex;                        /**< Current value is in ItemList[CurrentIndex] */
   TW_UINT32  DefaultIndex;                        /**< Powerup value is in ItemList[DefaultIndex] */
   TW_INT32   ItemList[1];                         /**< Array of ItemType values starts here       */
} TW_ENUMERATION_INT32, FAR * pTW_ENUMERATION_INT32;/**< Pointer to TW_ENUMERATION that holds an array TW_UINT32 items */

/**
* TW_ENUMERATION that holds a TW_UINT16 item
*/
typedef struct {
   TW_UINT16  ItemType;                            /**< Assigned TWAIN Type TWTY_UINT16 */
   TW_UINT32  NumItems;                            /**< How many items in ItemList */
   TW_UINT32  CurrentIndex;                        /**< Current value is in ItemList[CurrentIndex] */
   TW_UINT32  DefaultIndex;                        /**< Powerup value is in ItemList[DefaultIndex] */
   TW_UINT16  ItemList[1];                         /**< Array of ItemType values starts here       */
} TW_ENUMERATION_UINT16, FAR * pTW_ENUMERATION_UINT16;/**< Pointer to TW_ENUMERATION that holds an array TW_UINT16 items */

/**
* TW_ENUMERATION that holds a TW_UINT32 item
*/
typedef struct {
   TW_UINT16  ItemType;                            /**< Assigned TWAIN Type TWTY_UINT32 */
   TW_UINT32  NumItems;                            /**< How many items in ItemList */
   TW_UINT32  CurrentIndex;                        /**< Current value is in ItemList[CurrentIndex] */
   TW_UINT32  DefaultIndex;                        /**< Powerup value is in ItemList[DefaultIndex] */
   TW_UINT32  ItemList[1];                         /**< Array of ItemType values starts here       */
} TW_ENUMERATION_UINT32, FAR * pTW_ENUMERATION_UINT32;/**< Pointer to TW_ENUMERATION that holds an array TW_UINT32 items */

/**
* TW_ENUMERATION that holds a TW_STR32 item
*/
typedef struct {
   TW_UINT16  ItemType;                            /**< Assigned TWAIN Type TWTY_STR32 */
   TW_UINT32  NumItems;                            /**< How many items in ItemList */
   TW_UINT32  CurrentIndex;                        /**< Current value is in ItemList[CurrentIndex] */
   TW_UINT32  DefaultIndex;                        /**< Powerup value is in ItemList[DefaultIndex] */
   TW_STR32   ItemList[1];                         /**< Array of ItemType values starts here       */
} TW_ENUMERATION_STR32, FAR * pTW_ENUMERATION_STR32;/**< Pointer to TW_ENUMERATION that holds an array TW_STR32 items */

/**
* TW_ENUMERATION that holds a TW_STR64 item
*/
typedef struct {
   TW_UINT16  ItemType;                            /**< Assigned TWAIN Type TWTY_STR64 */
   TW_UINT32  NumItems;                            /**< How many items in ItemList */
   TW_UINT32  CurrentIndex;                        /**< Current value is in ItemList[CurrentIndex] */
   TW_UINT32  DefaultIndex;                        /**< Powerup value is in ItemList[DefaultIndex] */
   TW_STR64   ItemList[1];                         /**< Array of ItemType values starts here       */
} TW_ENUMERATION_STR64, FAR * pTW_ENUMERATION_STR64;/**< Pointer to TW_ENUMERATION that holds an array TW_STR32 items */

/**
* TW_ENUMERATION that holds a TW_STR128 item
*/
typedef struct {
   TW_UINT16  ItemType;                            /**< Assigned TWAIN Type TWTY_STR128 */
   TW_UINT32  NumItems;                            /**< How many items in ItemList */
   TW_UINT32  CurrentIndex;                        /**< Current value is in ItemList[CurrentIndex] */
   TW_UINT32  DefaultIndex;                        /**< Powerup value is in ItemList[DefaultIndex] */
   TW_STR128  ItemList[1];                         /**< Array of ItemType values starts here       */
} TW_ENUMERATION_STR128, FAR * pTW_ENUMERATION_STR128;/**< Pointer to TW_ENUMERATION that holds an array TW_STR32 items */

/**
* TW_ENUMERATION that holds a TW_STR255 item
*/
typedef struct {
   TW_UINT16  ItemType;                            /**< Assigned TWAIN Type TWTY_STR255 */
   TW_UINT32  NumItems;                            /**< How many items in ItemList */
   TW_UINT32  CurrentIndex;                        /**< Current value is in ItemList[CurrentIndex] */
   TW_UINT32  DefaultIndex;                        /**< Powerup value is in ItemList[DefaultIndex] */
   TW_STR255  ItemList[1];                         /**< Array of ItemType values starts here       */
} TW_ENUMERATION_STR255, FAR * pTW_ENUMERATION_STR255;/**< Pointer to TW_ENUMERATION that holds an array TW_STR255 items */

/**
* TW_ENUMERATION that holds a TW_FIX32 item
*/
typedef struct {
   TW_UINT16  ItemType;                            /**< Assigned TWAIN Type TWTY_FIX32 */
   TW_UINT32  NumItems;                            /**< How many items in ItemList */
   TW_UINT32  CurrentIndex;                        /**< Current value is in ItemList[CurrentIndex] */
   TW_UINT32  DefaultIndex;                        /**< Powerup value is in ItemList[DefaultIndex] */
   TW_FIX32   ItemList[1];                         /**< Array of ItemType values starts here       */
} TW_ENUMERATION_FIX32, FAR * pTW_ENUMERATION_FIX32;/**< Pointer to TW_ENUMERATION that holds an array TW_FIX32 items */

/**
* TW_ENUMERATION that holds a TW_FRAME item
*/
typedef struct {
   TW_UINT16  ItemType;                            /**< Assigned TWAIN Type TWTY_FRAME */
   TW_UINT32  NumItems;                            /**< How many items in ItemList */
   TW_UINT32  CurrentIndex;                        /**< Current value is in ItemList[CurrentIndex] */
   TW_UINT32  DefaultIndex;                        /**< Powerup value is in ItemList[DefaultIndex] */
   TW_FRAME   ItemList[1];                         /**< Array of TW_FRAME structures starts here */
} TW_ENUMERATION_FRAME, FAR * pTW_ENUMERATION_FRAME;/**< Pointer to TW_ENUMERATION that holds a TW_FRAME item */

/**
* TW_RANGE that holds a TW_FIX32 item
*/
typedef struct {
   TW_UINT16  ItemType;
   TW_FIX32  MinValue;                             /* Starting value in the range.           */
   TW_FIX32  MaxValue;                             /* Final value in the range.              */
   TW_FIX32  StepSize;                             /* Increment from MinValue to MaxValue.   */
   TW_FIX32  DefaultValue;                         /* Power-up value.                        */
   TW_FIX32  CurrentValue;                         /* The value that is currently in effect. */
} TW_RANGE_FIX32, FAR * pTW_RANGE_FIX32;           /**< Pointer to TW_RANGE that holds an array TW_FIX32 items */

#ifdef TWH_CMP_MSC
#pragma pack (pop, before_twain)
#else
#pragma pack() // reset
#endif // TWH_CMP_MSC

#ifdef TWH_CMP_GNU
#pragma pack(1)

/**
* Structure contains information about the type, size, and layout of a file 
* that contains a DIB. 
*/
typedef struct tagBITMAPFILEHEADER
{
    WORD  bfType;                                 /**< Specifies the file type, must be BM. */
    DWORD bfSize;                                 /**< Specifies the size, in bytes, of the bitmap file. */
    WORD  bfReserved1;                            /**< Reserved; must be zero. */
    WORD  bfReserved2;                            /**< Reserved; must be zero. */
    DWORD bfOffBits;                              /**< Specifies the offset, in bytes, from the beginning of 
                                                  the BITMAPFILEHEADER structure to the bitmap bits. */
} BITMAPFILEHEADER;

#pragma pack() // reset
#endif // TWH_CMP_GNU


/**
* Get the current value from a Capability as a TW_UINT32.
* @param[in] pCap a pointer to the capability to retrieve the current value
* @param[out] val the value retrieved from the capability
* @return true if successful
*/
bool getCurrent(TW_CAPABILITY *pCap, TW_UINT32& val);

/**
* Get the current value from a Capability as a string for capabilities of 
* types TWTY_STR32, TWTY_STR64, TWTY_STR128, and TWTY_STR256
* @param[in] pCap a pointer to the capability to retrieve the current value
* @param[out] val the value retrieved from the capability
* @return true if successful
*/
bool getCurrent(TW_CAPABILITY *pCap, string& val);

/**
* Get the current value from a Capability as a TW_FIX32.
* @param[in] pCap a pointer to the capability to retrieve the current value
* @param[out] val the value retrieved from the capability
* @return true if successful
*/
bool getCurrent(TW_CAPABILITY *pCap, TW_FIX32& val);

/**
* Get the current value from a Capability as a TW_FRAME.
* @param[in] pCap a pointer to the capability to retrieve the current value
* @param[out] val the value retrieved from the capability
* @return true if successful
*/
bool getCurrent(TW_CAPABILITY *pCap, TW_FRAME& val);

/**
* Get an item value from an array of values from a TW_ENUMERATION or TW_ARRAY 
* type Capability as a TW_UINT32.
* @param[in] pCap a pointer to the capability to retrieve the value
* @pCount[in] item the 0 based location in the array to retrieve the item.
* @param[out] val the value retrieved from the capability
* @return true if successful. false if no value returned
*/
bool GetItem(TW_CAPABILITY *pCap, TW_UINT32 item, TW_UINT32& val);

/**
* Get an item value from an array of values from a TW_ENUMERATION or TW_ARRAY
* containing types TWTY_STR32, TWTY_STR64, TWTY_STR128, and TWTY_STR256
* @param[in] pCap a pointer to the capability to retrieve the value
* @pCount[in] item the 0 based location in the array to retrieve the item.
* @param[out] val the value retrieved from the capability
* @return true if successful. false if no value returned
*/
bool GetItem(TW_CAPABILITY *pCap, TW_UINT32 item, string& val);

/**
* Get an item value from an array of values from a TW_ENUMERATION or TW_ARRAY
* containing type TWTY_FIX32
* @param[in] pCap a pointer to the capability to retrieve the value
* @pCount[in] item the 0 based location in the array to retrieve the item.
* @param[out] val the value retrieved from the capability
* @return true if successful. false if no value returned
*/
bool GetItem(TW_CAPABILITY *pCap, TW_UINT32 item, TW_FIX32& val);

/**
* Get an item value from an array of values from a TW_ENUMERATION or TW_ARRAY
* containing type TWTY_FRAME
* @param[in] pCap a pointer to the capability to retrieve the value
* @pCount[in] item the 0 based location in the array to retrieve the item.
* @param[out] val the value retrieved from the capability
* @return true if successful. false if no value returned
*/
bool GetItem(TW_CAPABILITY *pCap, TW_UINT32 item, TW_FRAME& val);

/**
* Get the size of TWAIN type
* @param[in] ItemType the TWAIN type to return the size for
* @return the size of the type returned
*/
int getTWTYsize(TW_UINT16  ItemType);

#endif // __COMMONTWAIN_H__

