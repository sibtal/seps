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
 *         This file contains Simple Thread Scheduler APIs used in SEPS
 * \author
 *         Sibasis Purohit <sibasis@sibtal.com>
 */

#ifndef _STS_CFG_H_
#define _STS_CFG_H_
#ifdef __cplusplus
extern "C" {
#endif
/**
	Section to include files
*/

/**
 *****************************************************************************
  @file sts_cfg.h
  @brief Simple Thread Scheduler(STS) configuration header.
     This file contains the configuration for Simple Thread  
     Scheduler module.
 *****************************************************************************/

/**
 *****************************************************************************
 * @ingroup sysdoc
 *
 * @{
 *****************************************************************************/


/**
 *****************************************************************************
 * @defgroup STS Simple Thread Scheduler API
 * @brief This module provides a simple thread shcedulling mechanism, based on
 *  the priority of the thread.
 *  Always the IDLE thread should be the lowest priority. ALWAYS REMEMBER TO 
 *  CHANGE IDLE_THREAD_PRIO and PRIO_MGMT_MAX_PRIO when a new thread is added.
******************************************************************************/

/**
 *****************************************************************************
 * 
 * @}     END of System Doc group include
 *****************************************************************************/
#define SST_HIGHEST_THREAD_PRIO     0
#define SST_LOWEST_THREAD_PRIO      STS_MAX_THREAD_PRIO

#define TIMER_THREAD_PRIO           0
#define USART_DRIVER_THREAD_PRIO    1
#define SPI_DRIVER_THREAD_PRIO      2
#define USB_DRIVER_THREAD_PRIO      3
#define GPIO_DRIVER_THREAD_PRIO     4
#define ADC_DRIVER_THREAD_PRIO      5
#define PWM_DRIVER_THREAD_PRIO      6
#define I2C_DRIVER_THREAD_PRIO      7
#define SSC_DRIVER_THREAD_PRIO      8

#define A_THREAD_PRIO      9
#define B_THREAD_PRIO      10

#define IDLE_THREAD_PRIO            11
#define STS_MAX_THREAD_PRIO         12


/**
	Section for function declaration which are part of this module
*/

#ifdef __cplusplus
}
#endif
#endif /* STS_CFG_H_ */
