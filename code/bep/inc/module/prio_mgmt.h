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
 * This file is part of the SEPS(Sibtal Embedded Platform Software) project.
 *
 */

/**
 * \file
 *         This file contains priority management APIs used in SEPS
 * \author
 *         Sibasis Purohit <sibasis@sibtal.com>
 */

#ifndef _PRIO_MGMT_H_
#define _PRIO_MGMT_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
	Section to include files
*/

#ifndef PRIO_MGMT_MAX_PRIO
#error PRIO_MGMT_MAX_PRIO is not defined 
#endif

#define PRIO_MGMT_MAX_NUM_PRIO (PRIO_MGMT_MAX_PRIO + 1)
/**
	Section to include files
*/
#if ( PRIO_MGMT_MAX_PRIO < 64 )
	#include "sts\prio_mgmt\prio_lt_64.h"
#endif
#if ( PRIO_MGMT_MAX_PRIO > 63 ) && ( PRIO_MGMT_MAX_PRIO < 512 )
	#include "sts\prio_mgmt\prio_lt_512.h"
#endif
#if ( PRIO_MGMT_MAX_PRIO > 511 ) && ( PRIO_MGMT_MAX_PRIO < 4096 ) 
	#include "sts\prio_mgmt\prio_lt_4096.h"
#endif

/**
 * @ingroup PrioMgmt
 *
 * @{
 */


/**
 * @defgroup Priority management module provides API to manage priority.
 * @brief Priority management module provides API to manage priority. 
 * It defines the size of the Priority management queue based on maximum
 * number of priorities needed. All the priorities are managed in a bit map
 * fashioned in the priority queue. User has to define PRIO_MGMT_MAX_PRIO prior  
 * to including this header file. This header file defines PRIO_MGMT_Q_SIZE
 * MACRO based on the value provided in PRIO_MGMT_MAX_PRIO to tell what should
 * be the size of the priority queue in bytes.
 * This module provides APIs to set/get/clear the highest priority in the 
 * priority queue. 
 * Lower the value higher the priority. 
*/

/**
	Section for MACRO definition
*/


/* Size of priority management queue */
#define MIN_INDEX  ( PRIO_MGMT_MAX_NUM_PRIO >> 3 )
#if PRIO_MGMT_MAX_NUM_PRIO > ( MIN_INDEX << 3 )//If it is not divisible by 8
#if ( PRIO_MGMT_GRP_LEVEL == 1 ) //for PRIO_MGMT_MAX_PRIO < 64
#define PRIO_MGMT_Q_SIZE   ((( PRIO_MGMT_MAX_NUM_PRIO ) >> 3 ) + 1 + 1 )
#elif ( PRIO_MGMT_GRP_LEVEL == 2 ) //for PRIO_MGMT_MAX_PRIO < 512
#define  PRIO_MGMT_Q_SIZE   ((PRIO_MGMT_MAX_NUM_PRIO) >> 3 + PRIO_MGMT_GRP2_SIZE + 1 + 1 )
#elif ( PRIO_MGMT_GRP_LEVEL == 3 ) //for PRIO_MGMT_MAX_PRIO < 4096
#define PRIO_MGMT_Q_SIZE   ((PRIO_MGMT_MAX_NUM_PRIO) >> 3 + PRIO_MGMT_GRP2_SIZE + \
    PRIO_MGMT_GRP3_SIZE + 1 + 1 )
#elif
#error NOT SUPPORTED //for PRIO_MGMT_MAX_PRIO > 4096 not supported
#endif  /* ( PRIO_MGMT_GRP_LEVEL == 1 ) */

#else   /* PRIO_MGMT_MAX_PRIO > ( MIN_INDEX << 3 ) */

#if ( PRIO_MGMT_GRP_LEVEL == 1 ) //for PRIO_MGMT_MAX_PRIO < 64
#define PRIO_MGMT_Q_SIZE   ((( PRIO_MGMT_MAX_NUM_PRIO ) >> 3 ) + 1 )
#elif ( PRIO_MGMT_GRP_LEVEL == 2 ) //for PRIO_MGMT_MAX_PRIO < 512
#define  PRIO_MGMT_Q_SIZE   (((PRIO_MGMT_MAX_NUM_PRIO) >> 3 ) + PRIO_MGMT_GRP2_SIZE + 1 )
#elif ( PRIO_MGMT_GRP_LEVEL == 3 ) //for PRIO_MGMT_MAX_PRIO < 4096
#define PRIO_MGMT_Q_SIZE   (((PRIO_MGMT_MAX_NUM_PRIO) >> 3) + PRIO_MGMT_GRP2_SIZE + \
    PRIO_MGMT_GRP3_SIZE + 1 )
#else
#error NOT SUPPORTED //for PRIO_MGMT_MAX_PRIO > 4096 not supported
#endif
#endif

#define CREATE_PRIORITY_QUEUE( QName ) uint8_t  (QName)

/**
 * @ingroup PrioMgmt
 * @brief Get the highest priority in the priority queue 
 *    This API writes the highest priority set in the queue "q" in "hp". If 
 * PRIO_MGMT_MAX_PRIO is less than 65 then this MACRO maps to PRIO_MGMT_LT_65_HP_GET else
 * it maps to either PrioMgmt_Lt513HPGet or PrioMgmt_Lt4097HPGet function.
 * @param hp - OUT Highest priority set in the priority queue.
 * @param q - IN priority queue name.
 * @retval void
 * @note
 *    
 */
#if ( PRIO_MGMT_MAX_PRIO < 64 )
	#define PRIO_MGMT_HIGHEST_GET( hp, q ) PRIO_MGMT_LT_64_HP_GET( hp, q )
#endif
#if ( PRIO_MGMT_MAX_PRIO > 63 ) && ( PRIO_MGMT_MAX_PRIO < 512 )
	#define PRIO_MGMT_HIGHEST_GET( hp, q ) PrioMgmt_Lt512HPGet( &hp, q )
#endif
#if ( PRIO_MGMT_MAX_PRIO > 511 ) && ( PRIO_MGMT_MAX_PRIO < 4096 ) 
	#define PRIO_MGMT_HIGHEST_GET( hp, q ) PrioMgmt_Lt4096HPGet( &hp, q )
#endif

/**
 * @ingroup PrioMgmt
 * @brief Set the priority in the priority queue 
 *    This API sets the priority in the priority queue "q". If 
 * PRIO_MGMT_MAX_PRIO is less than 65 then this MACRO maps to PRIO_MGMT_LT_65_HP_SET else
 * it maps to either PrioMgmt_Lt513HPSet or PrioMgmt_Lt4097HPSet function.
 * @param p - IN Priority to be set in the priority queue.
 * @param q - IN priority queue name.
 * @retval void
 * @note
 *    
 */
#if ( PRIO_MGMT_MAX_PRIO < 64 )
	#define PRIO_MGMT_SET( p, q ) PRIO_MGMT_LT_64_SET( p, q )
#endif
#if ( PRIO_MGMT_MAX_PRIO > 63 ) && ( PRIO_MGMT_MAX_PRIO < 512 )
	#define PRIO_MGMT_SET( p, q ) PrioMgmt_Lt512Set( p, q )
#endif
#if ( PRIO_MGMT_MAX_PRIO > 511 ) && ( PRIO_MGMT_MAX_PRIO < 4096 ) 
	#define PRIO_MGMT_SET( p, q ) PrioMgmt_Lt4096Set( p, q )
#endif

/**
 * @ingroup PrioMgmt
 * @brief Checks whether the priority is set in the priority queue 
 *    This API checks if the priority is set in the priority queue "q". If 
 * PRIO_MGMT_MAX_PRIO is less than 65 then this MACRO maps to PRIO_MGMT_LT_65_ISSET else
 * it maps to either PrioMgmt_Lt513IsSet or PrioMgmt_Lt4097IsSet function.
 * @param p - IN Priority to be checked in the priority queue.
 * @param q - IN priority queue name.
 * @param ret - OUT returns TRUE(1) or FALSE(0).
 * @retval void
 * @note
 *    
 */
#if ( PRIO_MGMT_MAX_PRIO < 64 )
	#define PRIO_MGMT_ISSET( p, q, ret ) PRIO_MGMT_LT_64_ISSET( p, q, ret )
#endif
#if ( PRIO_MGMT_MAX_PRIO > 63 ) && ( PRIO_MGMT_MAX_PRIO < 512 )
	#define PRIO_MGMT_ISSET( p, q, ret ) PrioMgmt_Lt512IsSet( p, q, &ret )
#endif
#if ( PRIO_MGMT_MAX_PRIO > 511 ) && ( PRIO_MGMT_MAX_PRIO < 4096 ) 
	#define PRIO_MGMT_ISSET( p, q, ret ) PrioMgmt_Lt4096IsSet( p, q, &ret )
#endif

/**
 * @ingroup PrioMgmt
 * @brief Clears the priority in the priority queue 
 *    This API clears the priority in the priority queue "q". If PRIO_MGMT_MAX_PRIO is
 * less than 65 then this MACRO maps to PRIO_MGMT_LT_65_Clr else
 * it maps to either PrioMgmt_Lt513Clr or PrioMgmt_Lt4097Clr function.
 * @param p - IN Priority to be cleared in the priority queue.
 * @param q - IN priority queue name.
 * @retval void
 * @note
 *    
 */
#if ( PRIO_MGMT_MAX_PRIO < 64 )
	#define PRIO_MGMT_CLR( p, q ) PRIO_MGMT_LT_64_CLR( p, q )
#endif
#if ( PRIO_MGMT_MAX_PRIO > 63 ) && ( PRIO_MGMT_MAX_PRIO < 512 )
	#define PRIO_MGMT_CLR( p, q ) PrioMgmt_Lt512Clr( p, q )
#endif
#if ( PRIO_MGMT_MAX_PRIO > 511 ) && ( PRIO_MGMT_MAX_PRIO < 4096 ) 
	#define PRIO_MGMT_CLR( p, q ) PrioMgmt_Lt4096Clr( p, q )
#endif



/**
	Section datatype
*/

/**
	Section for function declaration not part of this module
*/

/**
	Section for variable declaration
*/
extern uint8_t const  PrioMgmt_LowestBitLookupTbl[];

/**
	Section for function declaration which are part of this module
*/

#ifdef __cplusplus
}
#endif
#endif /* _PRIO_MGMT_H_ */