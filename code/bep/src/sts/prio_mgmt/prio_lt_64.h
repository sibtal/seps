/*
 * Copyright (c) 2013, Sibtal.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY SIBTAL AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the XXXX project.
 *
 */

/**
 * \file
 *         One line description of the file
 * \author
 *         Author Name <mail id>
 */

#ifndef _PRIO_LT_64_H_
#define _PRIO_LT_64_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
	Section to include files
*/

/**
 * @ingroup groupName
 *
 * @{
 */


/**
 * @defgroup group one line description
 * @brief Description of the module in detail.
*/

/**
	Section for MACRO definition
*/


#define PRIO_MGMT_MIN_GRP2_SIZE ( PRIO_MGMT_MAX_NUM_PRIO >> 3 )
#if PRIO_MGMT_MAX_NUM_PRIO  > ( PRIO_MGMT_MIN_GRP2_SIZE << 3 ) //If it is not divisible by 8
#define PRIO_MGMT_GRP2_SIZE ( PRIO_MGMT_MIN_GRP2_SIZE  + 1  )
#else
#define PRIO_MGMT_GRP2_SIZE ( PRIO_MGMT_MIN_GRP2_SIZE )
#endif /* PRIO_MGMT_MAX_PRIO  > ( PRIO_MGMT_MIN_GRP2_SIZE << 3 ) */

#define PRIO_MGMT_GRP1_SIZE 1
#define PRIO_MGMT_GRP2_START_IDX PRIO_MGMT_GRP1_SIZE
#define PRIO_MGMT_GRP_LEVEL 1


#define PRIO_MGMT_LT_64_HP_GET( hp, q )										\
{																			\
    base_t level1;															\
    if( 0 == q[0] ) hp = MAX_BASE_VALUE;									\
    level1 = PrioMgmt_LowestBitLookupTbl[q[0]];								\
    hp = ( level1 << 3 ) +													\
		PrioMgmt_LowestBitLookupTbl[q[level1 + PRIO_MGMT_GRP2_START_IDX ]];	\
}

#define PRIO_MGMT_LT_64_SET( p, q )											\
{																			\
    base_t level1, level1_pos = p;											\
    level1 = p >> 3;														\
    level1_pos -= ( level1 << 3 );											\
    q[ 0 ] |= ( 1 << level1 );												\
    q[ level1 + 1 ] |= ( 1 << level1_pos );									\
}

#define PRIO_MGMT_LT_64_ISSET( p, q, ret )									\
{																			\
    base_t level1, level1_pos = p;											\
	ret = FALSE;															\
    level1 = p >> 3;														\
    level1_pos -= ( level1 << 3 );											\
    if(( q[ 0 ] & ( 1 << level1 )) &&										\
       ( q[ level1 + PRIO_MGMT_GRP2_START_IDX ] & ( 1 << level1_pos )))		\
		ret = TRUE;															\
}

#define PRIO_MGMT_LT_64_CLR( p, q )											\
{																			\
    base_t level1, level1_pos = p;											\
    level1 = p >> 3;														\
    level1_pos -= ( level1 << 3 );											\
    q[ level1 + PRIO_MGMT_GRP2_START_IDX ] &= ~( 1 << level1_pos );			\
    if( 0 == q[ level1 + PRIO_MGMT_GRP2_START_IDX ] )						\
        q[ 0 ] &= ~( 1 << level1 );											\
}


/**
	Section datatype
*/

/**
	Section for function declaration not part of this module
*/

/**
	Section for variable declaration
*/

/**
	Section for function declaration which are part of this module
*/

#ifdef __cplusplus
}
#endif
#endif /* _PRIO_LT_64_H_ */