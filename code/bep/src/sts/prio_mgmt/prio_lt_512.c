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
 * This file is part of the SEPS(Sibtal Embedded Platform Software) project. *
 */

/**
 * \file
 *         This file contains priority management APIs used in SEPS
 * \author
 *         Sibasis Purohit <sibasis@sibtal.com>
 */


/**
	Section to include files
*/

#include "common.h"
#define PRIO_MGMT_MAX_PRIO 511
#include "module\prio_mgmt.h"

/**
	Section for Private MACRO definition
*/

/**
	Section Private datatype
*/

/**
	Section for private function declaration which are part of this module
*/

/**
	Section for variable declaration
*/


/**
	Section for public function defination which are part of this module
*/

void PrioMgmt_Lt512HPGet( base_t *gprio, uint8_t* q )
{
    base_t level1_pos, level2_pos;
    
    if( 0 == q[0] ) *gprio = MAX_BASE_VALUE;
     
    level1_pos = PrioMgmt_LowestBitLookupTbl[q[0]];
    level2_pos = PrioMgmt_LowestBitLookupTbl[q[level1_pos + PRIO_MGMT_GRP2_START_IDX ]];
#if 0
    printf("( level1_pos << 3 ) << 3 = %d, ( level2_pos << 3 ) = %d,\
        PrioMgmt_LowestBitLookupTbl[q[ PRIO_MGMT_GRP3_START_IDX + (level2_pos<<3) ]] = %d, PRIO_MGMT_GRP3_START_IDX = %d, prio = %d, %d\n", 
        ( level1_pos << 3 ) << 3, ( level2_pos << 3 ),
        PrioMgmt_LowestBitLookupTbl[q[ PRIO_MGMT_GRP3_START_IDX + (( level1_pos << 3 ) +
        PrioMgmt_LowestBitLookupTbl[q[ PRIO_MGMT_GRP2_START_IDX + level2_pos]])]], PRIO_MGMT_GRP3_START_IDX,
        (( level1_pos << 3 ) << 3 ) + ( level2_pos << 3 ) +
        (PrioMgmt_LowestBitLookupTbl[q[ PRIO_MGMT_GRP3_START_IDX + (( level1_pos << 3 ) +
        PrioMgmt_LowestBitLookupTbl[q[ PRIO_MGMT_GRP2_START_IDX + level2_pos]])]]),
        PrioMgmt_LowestBitLookupTbl[q[ PRIO_MGMT_GRP3_START_IDX + (( level1_pos << 3 ) + level2_pos )]]);
#endif
    *gprio = (( level1_pos << 3 ) << 3 ) + ( level2_pos << 3 ) +
        (PrioMgmt_LowestBitLookupTbl[q[ PRIO_MGMT_GRP3_START_IDX + (( level1_pos << 3 ) + level2_pos )]]);
}

void PrioMgmt_Lt512Set( base_t prio, uint8_t* q )
{
    base_t level1, level1_pos = prio;

    base_t level2, level3, level2_pos, level3_pos;
    level3 = prio >> 3;
    level3_pos = prio - ( level3 << 3 );
    level2 = level3 >> 3;
    level2_pos = level3 - ( level2 << 3 );
    
    level1_pos = level2;
    q[ 0 ] |= ( 1 << level1_pos );
    q[ PRIO_MGMT_GRP2_START_IDX + level2 ] |= ( 1 << level2_pos );
    q[ PRIO_MGMT_GRP3_START_IDX + level3 ] |= ( 1 << level3_pos );
}

void PrioMgmt_Lt512IsSet( base_t prio, uint8_t* q, base_t *ret )
{    
    base_t level2, level3, level2_pos, level3_pos;
	*ret = FALSE;
    level3 = prio >> 3;
    level3_pos = prio - ( level3 << 3 );
    level2 = level3 >> 3;
    level2_pos = level3 - ( level2 << 3 );
#if 0
    printf("q[0] = %x\n", q[0]);
    printf("level2 = %d, PRIO_MGMT_GRP2_START_IDX = %d, q[level2 + PRIO_MGMT_GRP2_START_IDX ] = %x\n", level2, PRIO_MGMT_GRP2_START_IDX, q[level2 + PRIO_MGMT_GRP2_START_IDX ]);
    printf("level3 = %d, PRIO_MGMT_GRP3_START_IDX = %d, q[level3 + PRIO_MGMT_GRP3_START_IDX ] = %x\n", level3, PRIO_MGMT_GRP3_START_IDX, q[level3 + PRIO_MGMT_GRP3_START_IDX ]);
#endif    
    if(( q[ 0 ] & ( 1 << level2 )) &&
        ( q[level2 + PRIO_MGMT_GRP2_START_IDX ] & ( 1 << level2_pos )) &&
        ( q[PRIO_MGMT_GRP3_START_IDX + level3] & ( 1 << level3_pos )))
        *ret = TRUE;

}

void PrioMgmt_Lt512Clr( base_t prio, uint8_t* q )
{    
    base_t level2, level3, level2_pos, level3_pos;
    level3 = prio >> 3;
    level3_pos = prio - ( level3 << 3 );
    level2 = level3 >> 3;
    level2_pos = level3 - ( level2 << 3 );

    q[ level3 + PRIO_MGMT_GRP3_START_IDX ] &= ~( 1 << level3_pos );
    if( 0 == q[ level3 + PRIO_MGMT_GRP3_START_IDX ] )
    {
        q[ level2 + PRIO_MGMT_GRP2_START_IDX ] &= ~( 1 << level2_pos );
        if( 0 == q[ level2 + PRIO_MGMT_GRP2_START_IDX ] )
            q[ 0 ] &= ~( 1 << level2 );
    }
}

/**
	Section for private function defination which are part of this module
*/


