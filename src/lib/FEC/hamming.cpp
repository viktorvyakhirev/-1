/***************************************************************************
*               Hamming Encoding and Decoding Implementation
*
*   File    : hamming.c
*   Purpose : Implementation Hamming encode and decode routines.  Contains
*             different style functions for Hamming encoding and decoding.
*   Author  : Michael Dipperstein
*   Date    : December 29, 2004
*
****************************************************************************
*   UPDATES
*
*   $Id: hamming.c,v 1.4 2007/09/19 13:08:14 michael Exp $
*   $Log: hamming.c,v $
*   Revision 1.4  2007/09/19 13:08:14  michael
*   Licensed under LGPL V3.
*
*   Revision 1.3  2005/10/24 12:33:12  michael
*   corrected errors in decode matrices pointed out by Ivan Piasini
*   <furettoo@gmail.com>
*
*   Revision 1.2  2005/01/03 14:41:56  michael
*   Renamed code generator matrix G so that it matches documentation.
*
*   Revision 1.1.1.1  2005/01/02 05:06:45  michael
*   Initial version
*
*
****************************************************************************
*
* Hamming: A collection of ANSI C Hamming Encoding/Decoding routines
* Copyright (C) 2004, 2007 by Michael Dipperstein (mdipperstein@gmail.com)
*
* This file is part of the Hamming library.
*
* The Hamming library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License as
* published by the Free Software Foundation; either version 3 of the
* License, or (at your option) any later version.
*
* The Hamming library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
* General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
***************************************************************************/

/***************************************************************************
*                             INCLUDED FILES
***************************************************************************/
#include "hamming.h"

/***************************************************************************
*                                CONSTANTS
***************************************************************************/

/* table of Hamming codes hammingCodes[x] is the x encoded */
const uint8_t hammingCodes[DATA_VALUES] =
{
    0x00,   /* 0 */
    0x71,   /* 1 */
    0x62,   /* 2 */
    0x13,   /* 3 */
    0x54,   /* 4 */
    0x25,   /* 5 */
    0x36,   /* 6 */
    0x47,   /* 7 */
    0x38,   /* 8 */
    0x49,   /* 9 */
    0x5A,   /* A */
    0x2B,   /* B */
    0x6C,   /* C */
    0x1D,   /* D */
    0x0E,   /* E */
    0x7F    /* F */
};

/* packed table convering encoded value (with error) to original data */
/* hammingPackedDecodeValues[code / 2] & 0x0F = original data for odd codes */
/* hammingPackedDecodeValues[code / 2] >> 4 = original data for even codes */
const uint8_t hammingPackedDecodeValues[CODE_VALUES] =
{
    0x00, 0x03, 0x05, 0xE7,     /* 0x00 to 0x07 */
    0x09, 0xEB, 0xED, 0xEE,     /* 0x08 to 0x0F */
    0x03, 0x33, 0x4D, 0x63,     /* 0x10 to 0x17 */
    0x8D, 0xA3, 0xDD, 0xED,     /* 0x18 to 0x1F */
    0x05, 0x2B, 0x55, 0x65,     /* 0x20 to 0x27 */
    0x8B, 0xBB, 0xC5, 0xEB,     /* 0x28 to 0x2F */
    0x81, 0x63, 0x65, 0x66,     /* 0x30 to 0x37 */
    0x88, 0x8B, 0x8D, 0x6F,     /* 0x38 to 0x3F */
    0x09, 0x27, 0x47, 0x77,     /* 0x40 to 0x47 */
    0x99, 0xA9, 0xC9, 0xE7,     /* 0x48 to 0x4F */
    0x41, 0xA3, 0x44, 0x47,     /* 0x50 to 0x57 */
    0xA9, 0xAA, 0x4D, 0xAF,     /* 0x58 to 0x5F */
    0x21, 0x22, 0xC5, 0x27,     /* 0x60 to 0x67 */
    0xC9, 0x2B, 0xCC, 0xCF,     /* 0x68 to 0x6F */
    0x11, 0x21, 0x41, 0x6F,     /* 0x70 to 0x77 */
    0x81, 0xAF, 0xCF, 0xFF      /* 0x78 to 0x7F */
};

/***************************************************************************
*   Function   : HammingTableEncode
*   Description: This function uses a lookup table to determine the
*                Hamming code for a DATA_BITS long value.  The lookup table
*                is based on the codes generated by the matrix G (above).
*   Parameters : data - DATA_BITS long series of data bits to encode.
*   Effects    : None
*   Returned   : Hamming code used to represent data
***************************************************************************/
uint8_t HammingTableEncode(uint8_t data)
{
    return hammingCodes[data];
}

/***************************************************************************
*   Function   : HammingPackedTableDecode
*   Description: This function uses a packed lookup table to determine the
*                value encoded by a CODE_BITS long code.  It assumes that
*                the code is based on the codes generated by the matrix G
*                (above), but errors are allowed.  The value returned is
*                the value with the code nearest (with the least error) to
*                the code passed as a parameter.
*                NOTE: The table packing will only work if the size of a
*                      table entry is larger than 2 * DATA_BITS.
*   Parameters : code - CODE_BITS long series of code bits to decode.
*   Effects    : None
*   Returned   : Nearest value to encoded data
***************************************************************************/
uint8_t HammingTableDecode(uint8_t code)
{
    uint8_t decoded;

    decoded = hammingPackedDecodeValues[code / 2];

    if (code % 2)
    {
        /* even value.  keep LS nibble */
        decoded &= (0xFF >> DATA_BITS);
    }
    else
    {
        /* odd value.  move MS nibble to LS half */
        decoded >>= DATA_BITS;
    }

    return decoded;
}
