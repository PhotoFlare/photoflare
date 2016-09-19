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
 * @file TwainApp_ui.cpp
 * User interface for TWAIN Application
 * @author JFL Peripheral Solutions Inc.
 * @date October 2006
 */

#ifdef _WINDOWS
  #include "stdafx.h"
#endif

#include <iostream>
#include <sstream>
using namespace std;

#include "TwainApp_ui.h"
#include "TwainString.h"


//////////////////////////////////////////////////////////////////////////////
string getErrorString_UnexpectedType(const TW_UINT16 _unReceived)
{
  ostringstream errMsg;  
  
  errMsg 
    << "The type is an unexpected value. "
    << " got " << convertTWTY_toString(_unReceived);  
  
  return errMsg.str();
}

//////////////////////////////////////////////////////////////////////////////
void printOptions()
{
  cout
    << "\n"
    << "Please enter an option\n"
    << "----------------------\n"
    << "q    - quit\n"
    << "h    - help\n"
    << "cdsm - connect to the dsm\n"
    << "xdsm - disconnect from the dsm\n"
    << "lds  - list data sources available\n"
    << "pds# - print identity structure for data source with id#. ex: pds2\n"
    << "cds# - connect to data source with id#. ex: cds2\n"
    << "xds  - disconnect from data source\n"
    << "caps - negotiate capabilities\n"
    << "scan - start the scan\n"
    << endl;
  return;
}

//////////////////////////////////////////////////////////////////////////////
void printMainCaps()
{
  cout
    << "\n"
    << "Capabilites\n"
    << "-----------\n"
    << "q - done negotiating, go back to main menu\n"
    << "h - help\n"
    << "1 - ICAP_XFERMECH\n"
    << "2 - ICAP_PIXELTYPE\n"
    << "3 - ICAP_BITDEPTH\n"
    << "4 - ICAP_XRESOLUTION\n"
    << "5 - ICAP_YRESOLUTION\n"
    << "6 - ICAP_FRAMES\n"
    << "7 - ICAP_UNITS\n"
    << endl;
  return;
}

//////////////////////////////////////////////////////////////////////////////
void print_ICAP(const TW_UINT16 _unCap, pTW_ONEVALUE _pCap)
{
  if(0 == _pCap)
  {
    return;
  }

  cout
    << "\n"
    << convertCAP_toString(_unCap) << "\n"
    << "--------------\n"
    << "Showing supported types. * indicates current setting.\n\n"
    << "q - done\n";
  
  switch(_pCap->ItemType)
  {
    case TWTY_FIX32:
    {
      pTW_FIX32 pFix32 = (pTW_FIX32)&_pCap->Item;
      cout << "1 - " << pFix32->Whole << "." << pFix32->Frac << "*\n" << endl;
    }
    break;

    case TWTY_FRAME:
    {
      pTW_FRAME pframe = (pTW_FRAME)&_pCap->Item;

      cout
        << "1 - Frame Data:*\n"
        << "\tLeft,\tTop,\tRight,\tBottom\n" 
        << "\t" << FIX32ToFloat(pframe->Left) << ",\t"
        << FIX32ToFloat(pframe->Top) << ",\t"
        << FIX32ToFloat(pframe->Right) << ",\t"
        << FIX32ToFloat(pframe->Bottom) << "\n"
        << "\n"
        << endl;
    }
    break;

    case TWTY_INT8:
    case TWTY_INT16:
    case TWTY_INT32:
    case TWTY_UINT8:
    case TWTY_UINT16:
    case TWTY_UINT32:
    case TWTY_BOOL:
    {
      cout << "1 - " << convertCAP_Item_toString(_unCap, _pCap->Item, _pCap->ItemType) << "*" << endl;
    }
    break;

    case TWTY_STR32:
    {
      cout << "1 - " << (pTW_STR32)&_pCap->ItemType << "*" << endl;
    }
    break;

    case TWTY_STR64:
    {
      cout << "1 - " << (pTW_STR64)&_pCap->ItemType << "*" << endl;
    }
    break;

    case TWTY_STR128:
    {
      cout << "1 - " << (pTW_STR128)&_pCap->ItemType << "*" << endl;
    }
    break;

    case TWTY_STR255:
    {
      cout << "1 - " << (pTW_STR255)&_pCap->ItemType << "*" << endl;
    }
    break;

    default:
    {
      cerr << getErrorString_UnexpectedType(_pCap->ItemType) << endl;
    }
    break;

  }
  return;
}

//////////////////////////////////////////////////////////////////////////////
void print_ICAP(const TW_UINT16 _unCap, pTW_ENUMERATION _pCap)
{
  if(0 == _pCap)
  {
    return;
  }

  cout
    << "\n"
    << convertCAP_toString(_unCap) << "\n"
    << "--------------\n"
    << "Showing supported types. * indicates current setting.\n\n"
    << "q - done\n";

  for(TW_UINT32 x = 0; x < _pCap->NumItems; ++x)
  {
    switch(_pCap->ItemType)
    {
      case TWTY_UINT32:
        {
          cout << x << " - " << convertCAP_Item_toString(_unCap, ((pTW_UINT32)(&_pCap->ItemList))[x], _pCap->ItemType);
        }
        break;

      case TWTY_INT32:
        {
          cout << x << " - " << convertCAP_Item_toString(_unCap, ((pTW_INT32)(&_pCap->ItemList))[x], _pCap->ItemType);
        }
        break;

      case TWTY_UINT16:
        {
          cout << x << " - " << convertCAP_Item_toString(_unCap, ((pTW_UINT16)(&_pCap->ItemList))[x], _pCap->ItemType);
        }
        break;

      case TWTY_INT16:
        {
          cout << x << " - " << convertCAP_Item_toString(_unCap, ((pTW_INT16)(&_pCap->ItemList))[x], _pCap->ItemType);
        }
        break;

      case TWTY_UINT8:
        {
          cout << x << " - " << convertCAP_Item_toString(_unCap, ((pTW_UINT8)(&_pCap->ItemList))[x], _pCap->ItemType);
        }
        break;

      case TWTY_INT8:
        {
          cout << x << " - " << convertCAP_Item_toString(_unCap, ((pTW_INT8)(&_pCap->ItemList))[x], _pCap->ItemType);
        }
        break;

      case TWTY_FIX32:
        {
          cout << x << " - " << ((pTW_FIX32)(&_pCap->ItemList))[x].Whole << "." << (int)( ( ((pTW_FIX32)(&_pCap->ItemList))[x].Frac/65536.0 + .0005 )*1000 );
        }
        break;

      case TWTY_FRAME:
        {
          pTW_FRAME pframe = &((pTW_FRAME)(&_pCap->ItemList))[x];
          cout << x << " - Frame Data:\n"
               << "\tLeft,\tTop,\tRight,\tBottom\n" 
               << "\t" << FIX32ToFloat(pframe->Left) << ",\t"
               << FIX32ToFloat(pframe->Top) << ",\t"
               << FIX32ToFloat(pframe->Right) << ",\t"
               << FIX32ToFloat(pframe->Bottom) ;
        }
        break;

      case TWTY_STR32:
        {
          cout << "1 - " << &((pTW_STR32)(&_pCap->ItemList))[x];
        }
        break;

      case TWTY_STR64:
        {
          cout << "1 - " << &((pTW_STR64)(&_pCap->ItemList))[x];
        }
        break;

      case TWTY_STR128:
        {
          cout << "1 - " << &((pTW_STR128)(&_pCap->ItemList))[x];
        }
        break;

      case TWTY_STR255:
        {
          cout << "1 - " << &((pTW_STR255)(&_pCap->ItemList))[x];
        }
        break;


      default:
        {
          cerr << getErrorString_UnexpectedType(_pCap->ItemType) << endl;
        }
        break;
    }

    if(x == _pCap->CurrentIndex)
    {
      cout << "*";
    }

    cout << "\n";
  }

  cout << endl;
  return;
}
//////////////////////////////////////////////////////////////////////////////
