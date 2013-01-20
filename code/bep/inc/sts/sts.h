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

#ifndef _STS_H_
#define _STS_H_
#ifdef __cplusplus
extern "C" {
#endif
/**
	Section to include files
*/
/* sts_cfg.h file should define SST_HIGHEST_THREAD_PRIO and 
   SST_LOWEST_THREAD_PRIO macros
*/

#include "sts_cfg.h"

/**
 *****************************************************************************
  @file sts.h
  @brief Simple Thread Scheduler(STS) public header.
     This file contains the public APIs and structures for Simple Thread  
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

/*
** ============================================================================
** Structures & Unions Type Definitions
** ============================================================================
*/

#define SST_EVENT_PRIO_NONE 0

/**
 *****************************************************************************
  @ingroup STS
  @brief Structure for the thread entry function.
     Structure difinition for the thread entry function.
 *****************************************************************************/
typedef void (*STS_THREAD_T)( void *pCtx );

/**
 *****************************************************************************
  @ingroup STS
  @brief Structure for resource information stored in TCB.
     Structure difinition for the resource object information stored in TCB
     (Thread Control Block).
 *****************************************************************************/
typedef struct STS_RESRC_TCB_STRUCT 
{
    LIST_ITEM_T *pNext;
    base_t       Event;
}STS_RESRC_TCB_T;

/**
 *****************************************************************************
  @ingroup STS
  @brief Structure for TCB.
     Structure difinition for TCB (Thread Control Block).
 *****************************************************************************/
typedef struct STS_TCB_STRUCT 
{
    STS_THREAD_T    pEntryFn; /**< Thread entry function */
    void            *pCtx;     /**< Thread entry function parameter */    
    base_t          Prio;      /**< Priority of the thread */
    STS_RESRC_TCB_T Resrc;
    uint8_t         EventSize; /**< Size of the event array defined below */
    /**< Stores the events for the thread, 0th bit of the lowest index i.e. 0 
    is the highest priortiy event. If multiple events are set for the thread
    the highest priority event is returned when Sts_GetEvent is called */
    uint8_t         Event[1];  
}STS_TCB_T;

/**
 *****************************************************************************
  @ingroup STS
  @brief Structure for Resource used in STS.
     Structure difinition for resource used in STS.
 *****************************************************************************/
typedef struct STS_RESRC_STRUCT 
{
    STS_TCB_T *pOwner;
    LIST_T    WaitList;
}STS_RESRC_T;

/**
 *****************************************************************************
  @ingroup STS
  @brief Structure for Semaphore used in STS.
     Structure difinition for semaphore used in STS.
 *****************************************************************************/
typedef struct STS_SEM_STRUCT 
{
    uint8_t     ResrcId;
    uint8_t     Count;
    uint8_t     CurrCount;
    STS_RESRC_T Resrc;
}STS_SEM_T;

/**
 *****************************************************************************
  @ingroup STS
  @brief Structure for Semaphore used in STS.
     Structure difinition for semaphore used in STS.
 *****************************************************************************/
typedef struct STS_MUTEX_STRUCT 
{
    uint8_t     ResrcId;
    uint16_t    PrioCeiling; 
    STS_RESRC_T Resrc;
}STS_MUTEX_T;

/**
 *****************************************************************************
  @ingroup STS
  @brief Idle routine type difinition.
     Difines the type of the idle routine.
 *****************************************************************************/
typedef void (*STS_IDLE_ROUTINE_T)( void *pCtx );

/**
 * @ingroup STS
 * @brief STS initilization 
 *    This API initializes the STS.
 * @retval void
 * @note
 *    This API has to be called before calling any other APIs of STS module.
 */
void Sts_Init( void );

/**
 * @ingroup STS
 * @brief STS Start
 *    This API starts the STS module. The control doesn't return from this 
 *    fucntion call. This API takes care of scheduling the highest priorty
 *    thread. User should call this API once it has created at least one
 *    thread.
 * @retval void
 * @note
 *    - The control doesn't return from this fucntion call. 
 *    - User should call this API after calling sts_init API. 
 *    - User has to make sure to create a thread before calling ths API, or it
 *    can register the IDLE routine. This is ensure user gets the control to
 *    create other threads needed in application.
 */
void Sts_Start(void);

/**
 * @ingroup STS
 * @brief Creates a Thread. 
 *    This APIis called to create a thread. User has to provided the memory for
 *    TCB used for the thread. The TCB should be defined in global space. If 
 *    user has alread created a thread with the same priority this API will 
 *    return failure.
 * @param pTcb - IN Pointer to the TCB memory for the thread.
 * @param EntryFn - IN Entry function of the thread.
 * @param pCtx - IN Context of the thread. This context is passed when the 
 *                  entry_fn is called.
 * @param Prio - IN Priority of the thread.
 * @param Event - IN Initial event to be set for th thread.
 * @retval sbase_t, if STS creates the thread successfully it returns ERR_OK else
 *  it returns ERR_ARG. ERR_ARG will be return if user has already created a
 *  thread with same priority.
 * @note
 *    - The TCB memory passed has to be defined as global variable.
 *    - User should call this API after calling sts_init API.
 *    - If a non-sero value is passed in event, then the thread will be put 
 *    into ready queue, and entry_fn will be called once the priority of this 
 *    thread becomes the highest priority in the ready queue.
 */
sbase_t Sts_ThreadCreate( STS_TCB_T *pTcb, STS_THREAD_T EntryFn, 
    base_t Prio, void *pCtx, base_t Event );

/**
 * @ingroup STS
 * @brief Idle Thread registration. 
 *    This API is called to register Idle routine. This routine is called when 
 *    there is no thread is ready to run. User can put the system to low power
 *    state in this routine
 * @param Fn - IN Idle routine.
 * @param pCtx - IN Context to be passed in the routine.
 * @retval void.
 * @note    
 */
void Sts_RegisterIdleRoutine( STS_IDLE_ROUTINE_T Fn, void *pCtx );

/**
 * @ingroup STS
 * @brief Event post to a thread. 
 *    This API is called to post an event to a thread.
 * @param Prio -  IN Priority of the thread to post event.
 * @param Event - IN Event to post.
 * @retval int. Returns ERR_OK if it successfully post the event to the thread
 *  with the priority mentioned, else return ERR_ARG, if there is not thread
 *  created with the specified priority.
 * @note    
 */
sbase_t Sts_PostEvent( base_t Prio, base_t Event );

/**
 * @ingroup STS
 * @brief Gets the highest priority event set. 
 *    This API is called to find the highest event set. Once this API is called
 *    the event will be cleared.
 * @retval int. Returns the highest priority event set.
 * @note    
 */
base_t Sts_GetEvent( void );

/**
 * @ingroup STS
 * @brief Creates mutex. 
 *    This API is called to create mutex.
 * @param pEvent - IN Pointer to the mutex.
 * @param PrioCeiling - IN Priority ceiling.
 * @retval void.
 * @note 
 *      - The mutex pointer should points to the global storage.
 */
void Sts_MutexCreate( STS_MUTEX_T *pMutex, uint32_t PrioCeiling );

/**
 * @ingroup STS
 * @brief Mutex Lock. 
 *    This API is called to lock the mutex. The priority of the calling thread 
 *     is raised to prio ceilling specified.
 * @param pEvent - IN Pointer to the mutex.
 * @retval int. Returns the highest priority event set.
 * @note    
 */
sbase_t Sts_MutexLock(STS_MUTEX_T *pMutex, base_t Event );

/**
 * @ingroup STS
 * @brief Mutex Unlock. 
 *    This API is called to unlock the mutex. The priorty of the thread is 
 *    reduced back to its original priority.
 * @param pEvent - IN Pointer to the mutex.
 * @retval int. Returns the highest priority event set.
 * @note    
 */
sbase_t Sts_MutexUnlock(STS_MUTEX_T *pMutex);

/**
 * @ingroup STS
 * @brief Creates semaphore. 
 *    This API is called to create semaphore.
 * @param pEvent - IN Pointer to the semaphore.
 * @param Count - IN Semaphore count.
 * @param InitCount - IN Initial Semaphore count.
 * @retval void.
 * @note 
 *      - The semaphore pointer should points to the global storage.
 */
void Sts_SemCreate( STS_SEM_T *pSem, uint8_t Count, uint8_t InitCount );

/**
 * @ingroup STS
 * @brief Get semaphore. 
 *    This API is called to get semaphore. If semaphore is not availabe, STS
 *    posts the event passed in this API to the calling thread once it is 
 *    available, and the thread is the highest priorty waiting for it.
 * @param pEvent - IN Pointer to the semaphore.
 * @param Event - IN event to be passed when semaphore is available.
 * @retval void.
 * @note 
 */
void Sts_SemGet( STS_SEM_T *pSem, base_t Event );

/**
 * @ingroup STS
 * @brief Release semaphore. 
 *    This API is called to release semaphore. If any thread wiating for the
 *    semaphore, is moved to ready queue and is scheduled once it the thread
 *    is the highest priority in the ready queue.
 * @param pEvent - IN Pointer to the semaphore.
 * @param pEvent - IN event to be passed when semaphore is available.
 * @retval void.
 * @note 
 */
void Sts_SemRelease( STS_SEM_T *pSem );


/**
 * @ingroup STS
 * @brief Current running thread. 
 *    This API returns the priority of the currenly running thread.
 * @retval base_t, currenlty running thread priority.
 * @note 
 */
base_t Sts_CurrentRunniungThread( void );



/**
	Section for function declaration which are part of this module
*/

#ifdef __cplusplus
}
#endif
#endif /* STS_H_ */
