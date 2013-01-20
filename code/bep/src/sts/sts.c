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

/**
	Section to include files
*/

#include "common.h"
#include "error.h"
#include "module\list.h"
#include "sts\sts.h"

#define PRIO_MGMT_MAX_PRIO STS_MAX_THREAD_PRIO
#include "module\prio_mgmt.h"

/**
 *****************************************************************************
 * @ingroup STS
 * @brief MACROs for operating ready queue.
 *    These MACROs are used to operate on ready queue.
 *****************************************************************************/

#define STS_RESRC_ID_MUTEX 0
#define STS_RESRC_ID_SEM   1

/**
 *****************************************************************************
 @ingroup STS
 @brief STS state enum.
    Enum defining the state of the STS Scheduler.
 *****************************************************************************/
typedef enum STS_STATE_ENUM
{    
    STS_NOT_STATRTED,
    STS_LOCKED,
    STS_RUNNING
}STS_STATE_T;

/**
 *****************************************************************************
 @ingroup STS
 @brief STS thread list.
    This array holds the tcb pointers. The size of this array is defined by
    STS_MAX_THREAD_PRIO, i.e. the maximum number of thread the user wants to 
    have in the system.   
 *****************************************************************************/
STS_TCB_T * Sts_TcbArr[STS_MAX_THREAD_PRIO]; 


/**
 *****************************************************************************
 @ingroup STS
 @brief STS Ready queue.
    The ready queue. The corresponding bit for a particular thread is set if
    the thread is ready to run. The bit is reset once the thread is executed by
    the scheduler.
 @see  RDY_TBL_SIZE.
 *****************************************************************************/
//uint8_t Sts_ReadyQ[PRIO_MGMT_Q_SIZE]; 
CREATE_PRIORITY_QUEUE(Sts_ReadyQ);
/**
 *****************************************************************************
 @ingroup STS
 @brief STS Running thread.
    Stores the running thread, priority and TCB pointer.
 *****************************************************************************/
base_t Sts_RunningThreadPrio;
STS_TCB_T *Sts_RunningThreadTcb;
/**
 *****************************************************************************
 @ingroup STS
 @brief STS idle routine and the idle routine context.
    Stores the idle routine address and the context to be passed in the 
    routine.
 *****************************************************************************/
STS_IDLE_ROUTINE_T Sts_IdleRoutine;
void *Sts_IdleRoutineCtx;

/**
 *****************************************************************************
 @ingroup STS
 @brief STS state.
    Stores the state of the STS Scheduler.
 *****************************************************************************/
STS_STATE_T Sts_State;


/*****************************************************************************
   Private function
*****************************************************************************/
void Sts_Schedule( void );
/*****************************************************************************
   Public function
*****************************************************************************/
void sts_init( void )
{
    /* Initialize all the global varibales of STS */
    Sts_State = STS_NOT_STATRTED;

    memset( Sts_ReadyQ, 0, sizeof(Sts_ReadyQ)); /* Clear the ready list */
    
    memset( Sts_TcbArr, 0, sizeof( Sts_TcbArr ));

    Sts_IdleRoutine = NULL;
    Sts_IdleRoutineCtx = NULL;

    Sts_RunningThreadPrio = MAX_BASE_VALUE;
}

void Sts_RegisterIdleRoutine( STS_IDLE_ROUTINE_T Fn, void *pCtx )
{
    Sts_IdleRoutine = Fn;
    Sts_IdleRoutineCtx = pCtx;
}

void Sts_OnIdle( void )
{
    if( Sts_IdleRoutine != NULL )Sts_IdleRoutine( Sts_IdleRoutineCtx );
}

void Sts_Start(void)
{
    Sts_State = STS_RUNNING;
    while( 1 )
    {
        Irq_Disable();
        //If any thread is ready at least one bit has to be set
        if( Sts_ReadyQ[0] )
        {            
            Sts_Schedule();
        }
        else
        {
            Irq_Enable();
            Sts_OnIdle();            
        }
    }
}

sbase_t Sts_ThreadCreate( STS_TCB_T *pTcb, STS_THREAD_T EntryFn, 
    base_t Prio, void *pCtx, base_t Event )
{
    sbase_t Ret = ERR_ARG;
    base_t IsSet; 
    PRIO_MGMT_ISSET( Prio, Sts_ReadyQ, IsSet )
    if( IsSet )
    {
        goto Sts_ThreadCreate_Exit;
    }
    
    /**< Initialize the task queue */
    if( Event != SST_EVENT_PRIO_NONE )
    {
        base_t EventId = Event >> 3;
        /**< Check if the event passed is a valid event for this thread */
        if( pTcb->EventSize >=  EventId )
        {
            /**< Set the event in the appropriate position */
            pTcb->Event[ EventId ] |= 1 << ( Event - ( EventId << 3));

            PRIO_MGMT_SET( Prio, Sts_ReadyQ ); 

        }
        else
        {
            Ret = ERR_ARG;
            goto Sts_ThreadCreate_Exit;
        }
    }
    pTcb->pEntryFn = EntryFn;
    pTcb->Prio = Prio;
    pTcb->pCtx = pCtx;    
    Sts_TcbArr[ Prio ] = pTcb;
    Ret = ERR_OK;
    
    /**< If the user has passed any event then put the thread in ready queue and 
    call the scheduler to findout the highest priority thread and schedule it */
    if( Event != SST_EVENT_PRIO_NONE )
    {       
        /**< Set in the TCB LIST */
        PRIO_MGMT_SET( Prio, Sts_ReadyQ );
        /**< Call the scheduler to findout the highest priotiry thread in ready 
        state and to execute it*/
        Sts_Schedule();
    }    
    
Sts_ThreadCreate_Exit:
    return Ret;
}

void Sts_Schedule( void )
{
    base_t CurntPrio, Prio;
    STS_TCB_T *pCurntTcb;
    /**< This API is assumed to be called with ISR DISABLED condition */
    if( Sts_State != STS_RUNNING )
    {
        Irq_Enable();
        return;
    }
    /**< Stores the currently running priority in the local variable */
    CurntPrio = Sts_RunningThreadPrio; 
    pCurntTcb = Sts_RunningThreadTcb;

    /**< Gets the highest priority Thread from the Ready queue */
    PRIO_MGMT_HIGHEST_GET( Prio, Sts_ReadyQ );
    
    /**< If the highes priorty thread is */
    while( CurntPrio > Prio )
    {        
        STS_TCB_T *pTcb = Sts_TcbArr[ Prio ];
        
        /**< Update the current running priortiy */
        Sts_RunningThreadPrio = Prio;
        Sts_RunningThreadTcb = pTcb;
        /**< Enable IRQ and call the thread */
        Irq_Enable();
        pTcb->pEntryFn( Sts_TcbArr[ Prio ]->pCtx ); 
        Irq_Disable();

        /**< If there is no event set for the current thread, then rmove it 
        the thread from the ready queue */
        if( pTcb->Event == 0 )
            PRIO_MGMT_CLR( Prio, Sts_ReadyQ );

        /**< Find the next highest priority Thread from the Ready queue */
        PRIO_MGMT_HIGHEST_GET( Prio, Sts_ReadyQ );
    }
    /**< Update the current running thread to the old thread priorty from where
    the scheduler was called */
    Sts_RunningThreadPrio = CurntPrio;
    Sts_RunningThreadTcb = pCurntTcb;
    
    Irq_Enable();
}

sbase_t Sts_PostEvent( base_t Prio, base_t Event )
{
    STS_TCB_T *pTcb;
    sbase_t ret = ERR_ARG; 
    uint8_t EventId ;

    Event--;
    EventId = ( Event >> 3 );

    Irq_Disable();
    
    pTcb = Sts_TcbArr[ Prio ];
    
    /**< Check if the event passed is a valid event for this thread */
    if( pTcb->EventSize >=  EventId )
    {
        /**< Set the event in the appropriate position */
        pTcb->Event[ EventId ] |= 1 << ( Event - ( EventId << 3));

        PRIO_MGMT_SET( Prio, Sts_ReadyQ ); 
    
    
        ret = ERR_OK;

        Sts_Schedule();
    }
    else
    {
        Irq_Enable();
    }
    return ret;
}

base_t Sts_GetEvent( void )
{
    STS_TCB_T *pTcb = Sts_RunningThreadTcb;
    base_t i, Event = SST_EVENT_PRIO_NONE;
    Irq_Disable();
    /**< Loop for the sizeof event to operate on each byte of event */
    for( i = 0; i < pTcb->EventSize; i++ )
    {
        /**< If the byte is none-zero then pass to lbs_lookup_tbl as index to
        find the lowest bit set in the byte */
        if(pTcb->Event[i])
        {
            Event = PrioMgmt_LowestBitLookupTbl[pTcb->Event[i]];
            /**< Clear the event */
            pTcb->Event[i] &= ~( 1 << Event );
            /**< Calculate the actual value of the event based on its bit 
            position. Add highest_prio_event to the value of loop * 8, i.e. 
            right shift loop 3 times */
            Event = Event + ( i << 3 ) + 1;
            break;
        }
    }
    Irq_Enable();
    return Event;
}

void Sts_ResrcCreate( STS_RESRC_T *pResrc )
{
    List_Init( &pResrc->WaitList );
    pResrc->pOwner = NULL;
}
sbase_t Sts_ResrcGet( STS_RESRC_T *pResrc, base_t Event )
{
    sbase_t ret = ERR_USE;
    STS_TCB_T *pTcb;
    Irq_Disable();
    pTcb = Sts_RunningThreadTcb;
    if( pResrc->pOwner == NULL )
    {
        pResrc->pOwner = pTcb;
        ret = ERR_OK;
    }
    else
    {
        LIST_ITEM_T *pPrev, *pNext;
        /**< Sort the list based on the priority of the thread wiating for the 
        resource */
        pPrev = pNext = pResrc->WaitList.pStart;
        while( pNext != NULL )
        {           
            /**< Resource list points to the resource pointer. To get the 
            pointer to the starting location of TCB, offset of Resrc is 
            subtracted from Resrc pointer */
            STS_TCB_T *pCurTcb = (STS_TCB_T *)((uint8_t*)pNext - 
                offsetof(STS_TCB_T, Resrc )); 
            if( pCurTcb->Prio < pTcb->Prio )
                break;
            else
            {
                pPrev = pNext;
                pNext = pNext->pNext; 
            }
        }
        List_EntryAddAfter( &pResrc->WaitList, pPrev, pTcb->Resrc.pNext );
        pTcb->Resrc.Event = Event;
    }
    Irq_Enable();
    return ret;
}
void Sts_ResrcRelease( STS_RESRC_T *pResrc )
{    
    STS_TCB_T *pTcb;
    STS_RESRC_TCB_T *pNext;
    Irq_Disable();
    /**< Get the thread waiting for the resource from the head of the list. */
    pNext = (STS_RESRC_TCB_T *)List_EntryGetStart( &pResrc->WaitList );
    if( NULL != pNext )
    {
        /**< */
        List_EntryDelete( &pResrc->WaitList, (LIST_ITEM_T*)pNext );
        pTcb = (STS_TCB_T *)((uint8_t*)pNext - offsetof(STS_TCB_T, Resrc));
        pResrc->pOwner = pTcb;
        Irq_Enable();
        Sts_PostEvent( pTcb->Prio, pTcb->Resrc.Event );
    }
    else
        Irq_Enable();
}

void Sts_MutexCreate( STS_MUTEX_T *pMutex, uint32_t PrioCeiling )
{
    pMutex->ResrcId = STS_RESRC_ID_MUTEX;
    pMutex->PrioCeiling = PrioCeiling;
    Sts_ResrcCreate( &pMutex->Resrc );
}

sbase_t Sts_MutexLock(STS_MUTEX_T *pMutex, base_t Event )
{
    sbase_t ret = Sts_ResrcGet( &pMutex->Resrc, Event );
    if( ERR_OK == ret )
    {
        STS_TCB_T *pTcb;
        Irq_Disable();
        pTcb = Sts_RunningThreadTcb;

        Sts_RunningThreadPrio = pMutex->PrioCeiling;        

        Irq_Enable();
    }

    return ret;
}

sbase_t Sts_MutexUnlock(STS_MUTEX_T *pMutex)
{
    sbase_t ret = ERR_ARG;
    STS_TCB_T *pTcb;
    Irq_Disable();
    pTcb = Sts_RunningThreadTcb;
    if( pMutex->Resrc.pOwner == pTcb )
    {
        Sts_RunningThreadPrio = pTcb->Prio;
        Irq_Enable();
        Sts_ResrcRelease( &pMutex->Resrc );
        ret = ERR_OK;
    }
    Irq_Enable();
    return ret;
}

void Sts_SemCreate( STS_SEM_T *pSem, uint8_t Count, uint8_t InitCount )
{
    pSem->ResrcId = STS_RESRC_ID_SEM;
    pSem->Count = Count;
    pSem->CurrCount = InitCount;
    Sts_ResrcCreate( &pSem->Resrc );
}

sbase_t Sts_SemLock(STS_SEM_T *pSem, base_t Event )
{
    sbase_t ret = ERR_USE;
    Irq_Disable();

    if( pSem->CurrCount < pSem->Count )
    {
        pSem->CurrCount++;
        Irq_Enable();
        ret = Sts_ResrcGet( &pSem->Resrc, Event );        
    }
    else
    {
        Irq_Disable();
    }

    return ret;
}

sbase_t Sts_SemUnlock( STS_SEM_T *pSem )
{
    sbase_t ret = ERR_ARG;

    Irq_Disable();
    if(  pSem->CurrCount > 0 )
    {
        ret = ERR_OK;
        pSem->CurrCount--;
        Irq_Enable();
        Sts_ResrcRelease( &pSem->Resrc );
    }
    
    return ret;
}
base_t Sts_CurrentRunniungThread()
{
    if( Sts_State == STS_LOCKED )return MAX_BASE_VALUE;
    else return Sts_RunningThreadPrio;
}


