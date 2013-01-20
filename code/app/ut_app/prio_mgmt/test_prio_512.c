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
 *         This file contains test code to test priority management module.
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
void test_512_prio_mgmt( void )
{
	int32_t p = PRIO_MGMT_MAX_PRIO, hp, ret;
	uint8_t q[PRIO_MGMT_Q_SIZE];
    memset(q, 0, sizeof(q));
	do
	{
		PRIO_MGMT_SET( p, q );
		PRIO_MGMT_ISSET( p, q, ret );
		if( ret != TRUE )
			printf("Priority %d is not set even after setting it\n", p);
		PRIO_MGMT_HIGHEST_GET( hp, q  );
		if( hp != p )
			printf("Priority %d is the highest priority set, but it says %d\n", p, hp);

		p--;
	}while ( p >= 0 );
    if(p<0)p=0;
	
	do
	{
		PRIO_MGMT_HIGHEST_GET( hp, q  );
		if( hp != p )
			printf("Priority %d is the highest priority set, but it says %d\n", p, hp);

        PRIO_MGMT_CLR( p, q );

		p++;

	}while ( p != PRIO_MGMT_MAX_PRIO );
}


/**
	Section for private function defination which are part of this module
*/
