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

#ifndef _LIST_H_
#define _LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
	Section to include files
*/


    /**
 * @ingroup PrioMgmt
 *
 * @{
 */


/**
 * @defgroup List module provides API to manage list.
 * @brief List APIs provides facilities to add, remove item from the list. 
*/

/**
	Section for MACRO definition
*/

/*
**=========================================================================
**  Structures
**=========================================================================
*/

/**
 *****************************************************************************
  @ingroup List
  @brief Structure for the List Item.
     Structure difinition for the List Item.
 *****************************************************************************/
typedef struct LIST_ITEM
{
    struct LIST_ITEM *pNext;
} LIST_ITEM_T;

/**
 *****************************************************************************
  @ingroup List
  @brief Structure for List.
     Structure difinition for the List.
 *****************************************************************************/
typedef struct LIST
{
    LIST_ITEM_T *pStart;
} LIST_T;


/**
	Section for function declaration which are part of this module
*/

/**
 * @ingroup List
 * @brief List initilization 
 *    This API initializes the List.
 * @retval void
 * @note
 *    This API has to be called before calling any other APIs of List module.
 */
INLINE void List_Init( LIST_T *pList )
{
    pList->pStart = NULL;
}


/**
 * @ingroup List
 * @brief List Add 
 *    Adds an entry to the Start of a linked list.
 * @retval void
 * @note
 *    
 */
INLINE void List_EntryAddAtStart( 
      LIST_T *pList,     /**< the list    */
      LIST_ITEM_T* pItem  /**< the pointer */
      )
{
    pItem->pNext = NULL;

    if ( pList->pStart == NULL )
    {
        // This is the first
        pList->pStart = pItem;
    }
    else
    {
        // Add to front of the queue
        pItem->pNext  = pList->pStart;
        pList->pStart = pItem;
    }
}

/**
 * @ingroup List
 * @brief List Add 
 *    Adds an entry to the End of a linked list.
 * @retval void
 * @note
 *    
 */
void List_EntryAddAtEnd( 
      LIST_T *pList,     /**< the list    */
      LIST_ITEM_T* pItem  /**< the pointer */
      );


/**
 ** \brief Adds an entry to a specific place in a linked list
 **
 */
INLINE void List_EntryAddAfter( 
    LIST_T *pList,     /**< the list    */
    LIST_ITEM_T* pItem, /**< the pointer */
    LIST_ITEM_T* pLItem /**< location where to add entry */
    )
{
    if ( pLItem == NULL )
    {
        List_EntryAddAtStart( pList, pItem );
        return;
    }        

    pItem->pNext = pLItem->pNext;
    pLItem->pNext = pItem;
}

INLINE LIST_ITEM_T* List_EntryScanNext(
    LIST_T* pList,     /**< the list    */
    LIST_ITEM_T* pItem /**< the pointer */
    )
{
    if( NULL != pItem )
    {
        return pItem->pNext;
    }
    else
    {
        return pList->pStart;
    }
}

INLINE bool List_EntryIsStart(
    LIST_T* pList,     /**< the list    */
    LIST_ITEM_T* pItem /**< the pointer */
    )
{
    return pList->pStart == pItem;
}

INLINE LIST_ITEM_T* List_EntryGetStart(
    LIST_T *pList     /**< the list    */
    )
{
    return pList->pStart;
}
INLINE void List_EntrySetStart(
    LIST_T* pList,     /**< the list    */
    LIST_ITEM_T* pItem /**< the pointer */
    )
{
    pList->pStart = pItem;
}

INLINE LIST_ITEM_T* List_EntryGetPrev(
    LIST_T *pList,     /**< the list    */
    LIST_ITEM_T* pItem /**< the pointer */
    )
{
    LIST_ITEM_T *pCurrent, *pPrev;
    pCurrent = pPrev = pList->pStart;

    while(( pCurrent != pItem ) && ( pCurrent != NULL )) 
    {
        pPrev = pCurrent;
        pCurrent = pCurrent->pNext;
    }
    if( pCurrent == pPrev ) pPrev = NULL;

    return pPrev;
}


/**
 ** \brief Deletes a list entry from a linked list, and frees up the memory
 */
INLINE void List_EntryDeleteNext(
    LIST_T *pList,    /**< the list from which to delete entry */
    LIST_ITEM_T* pItem /**< previous entry which has to be deleted */
    )
{
    LIST_ITEM_T *a;
    LIST_ITEM_T *b;
    a = pList->pStart;

    if ( a == NULL )
        return;

    /* If we are removing the first entry */
    if (( pList->pStart == pItem ) || ( NULL == pItem ))
    {
        pList->pStart = a->pNext;                
        return;  
    }   
    
    /* Get the one we want to remove */
    b = pItem->pNext;
    
    /* Remove the unwanted item */
    pItem->pNext = b->pNext;
       
    
    return;
}

LIST_ITEM_T* List_EntryDelete(
    LIST_T *pList,    /**< the list from which to delete entry */
    LIST_ITEM_T* pItem /**< entry to delete */
    );



#ifdef __cplusplus
}
#endif
#endif /* _LIST_H_ */
