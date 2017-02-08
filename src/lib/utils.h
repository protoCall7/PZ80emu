/** \file */
//
//  utils.h
//  PZ80emu
//
//  Created by Peter Ezetta on 2/27/16.
//  Copyright (c) 2016 Peter Ezetta. All rights reserved.
//

#ifndef __PZ80emu__utils__
#define __PZ80emu__utils__

/** Determines if a bit is set or not
   \param val Bitfield to check
   \param bit Which bit in the field to check
   \return 0 == Checked Bit False, 1 == Checked Bit True.
 */

#define IS_SET(val, bit) !!((val) & (1 << (bit)))
#endif // defined __PZ80emu__utils__
