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
#define PRIO_MGMT_MAX_PRIO 4095
#include "module\prio_mgmt.h"

/*
                                               ----------------------------
                                            7 /  /  /  /  /  /  /  /  /   |
                                             .............................|
                                         1  ........................... . |
            0   1  2  3  4  5  6  7    0 /  /  /  /  /  /  /  /  / | | .  |
    ----   -------------------------    -------------------------  |/| .  |
  0 |  |   |  |  |  |  |  |  |  |  |    |  |  |  |  |  |  |  |  | /| | .  |
    ----   -------------------------    -------------------------  |/| .  |
  1 |  |   |  |  |  |  |  |  |  |  |    |  |  |  |  |  |  |  |  | /| | .  |
    ----   -------------------------    -------------------------  |/| .  |
  2 |  |   |  |  |  |  |  |  |  |  |    |  |  |  |  |  |  |  |  | /| | .  |
    ----   -------------------------    -------------------------  |/| .  |
  3 |  |   |  |  |  |  |  |  |  |  |    |  |  |  |  |  |  |  |  | /| | .  | 
    ----   -------------------------    -------------------------  |/| .  |
  4 |  |   |  |  |  |  |  |  |  |  |    |  |  |  |  |  |  |  |  | /| | .  | 
    ----   -------------------------    -------------------------  |/| .  |
  5 |  |   |  |  |  |  |  |  |  |  |    |  |  |  |  |  |  |  |  | /| | .  |
    ----   -------------------------    -------------------------  |/| . / 
  6 |  |   |  |  |  |  |  |  |  |  |    |  |  |  |  |  |  |  |  | /| | .  
    ----   -------------------------    -------------------------  |/
  7 |  |   |  |  |  |  |  |  |  |  |    |  |  |  |  |  |  |  |  | /
    ----   -------------------------    -------------------------  

*/

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


void PrioMgmt_Lt4096HPGet( base_t *gprio, uint8_t* q )
{
    base_t prio = 0, level1;
    base_t level2, level3, level4, level1_pos, level2_pos, level3_pos, level4_pos;
    if( 0 == q[0] ) *gprio = MAX_BASE_VALUE;
     
    level1 = 0;
    level1_pos = PrioMgmt_LowestBitLookupTbl[q[0]];
    
    level2 = level1_pos;
    level2_pos = PrioMgmt_LowestBitLookupTbl[q[ PRIO_MGMT_GRP2_START_IDX + level2 ]];
    
    level3 = (level2 << 3) + level2_pos;
    level3_pos = PrioMgmt_LowestBitLookupTbl[q[PRIO_MGMT_GRP3_START_IDX + level3 ]];
    
    level4 = ( level3 << 3 ) + level3_pos;
    level4_pos = PrioMgmt_LowestBitLookupTbl[q[PRIO_MGMT_GRP4_START_IDX + level4 ]];
    
    prio = ((( level1_pos << 3 ) << 3 ) << 3 ) + (( level2_pos << 3 ) << 3 ) + ( level3_pos << 3 ) +
        (PrioMgmt_LowestBitLookupTbl[q[ PRIO_MGMT_GRP4_START_IDX + (( level1_pos << 3 ) << 3 ) + 
        ( level2_pos << 3 ) + level3_pos ]]);

    *gprio = prio;
}

void PrioMgmt_Lt4096Set( base_t prio, uint8_t* q )
{
    base_t level1_pos;
    base_t level2, level3, level4, level2_pos, level3_pos, level4_pos; 
    level4 = prio >> 3;     
    level4_pos = prio - ( level4 << 3 );
    level3 = level4 >> 3;
    level3_pos = level4 - ( level3 << 3 );
    level2 = level3 >> 3;
    level2_pos = level3 - ( level2 << 3 );
    
    level1_pos = level2;
    q[ 0 ] |= ( 1 << level1_pos );
    q[ PRIO_MGMT_GRP2_START_IDX + level2 ] |= ( 1 << level2_pos );
    q[ PRIO_MGMT_GRP3_START_IDX + level3 ] |= ( 1 << level3_pos );
    q[ PRIO_MGMT_GRP4_START_IDX + level4 ] |= ( 1 << level4_pos );

}

void PrioMgmt_Lt4096IsSet( base_t prio, uint8_t* q, base_t *ret )
{    
    base_t level1_pos;
    base_t level2, level3, level4, level2_pos, level3_pos, level4_pos; 
	*ret = FALSE;
    level4 = prio >> 3;   
    level4_pos = prio - ( level4 << 3 );
    level3 = level4 >> 3;
    level3_pos = level4 - ( level3 << 3 );
    level2 = level3 >> 3;
    level2_pos = level3 - ( level2 << 3 );
    
    level1_pos = level2;
    if(( q[ 0 ] & ( 1 << level1_pos )) &&
        ( q[ PRIO_MGMT_GRP2_START_IDX + level2 ] & ( 1 << level2_pos )) &&
        ( q[PRIO_MGMT_GRP3_START_IDX + level3 ] & ( 1 << level3_pos )) &&
        ( q[PRIO_MGMT_GRP4_START_IDX + level4] & ( 1 << level4_pos )))
        *ret = TRUE;

}

void PrioMgmt_Lt4096Clr( base_t prio, uint8_t* q )
{
    base_t level1_pos;

    base_t level2, level3, level4, level2_pos, level3_pos, level4_pos; 
    level4 = prio >> 3;   
    level4_pos = prio - ( level4 << 3 );
    level3 = level4 >> 3;
    level3_pos = level4 - ( level3 << 3 );
    level2 = level3 >> 3;
    level2_pos = level3 - ( level2 << 3 );
    
    level1_pos = level2;

    q[ level4 + PRIO_MGMT_GRP4_START_IDX ] &= ~( 1 << level4_pos );
    if( 0 == q[ level4 + PRIO_MGMT_GRP4_START_IDX ] )
    {
        q[ level3 + PRIO_MGMT_GRP3_START_IDX ] &= ~( 1 << level3_pos );
        if( 0 == q[ level3 + PRIO_MGMT_GRP3_START_IDX ] )
        {
            q[ level2 + PRIO_MGMT_GRP2_START_IDX ] &= ~( 1 << level2_pos );
            if( 0 == q[ level2 + PRIO_MGMT_GRP2_START_IDX ] )
                q[ 0 ] &= ~( 1 << level1_pos );
        }
    }
}


/**
	Section for private function defination which are part of this module
*/

