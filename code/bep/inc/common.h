
#ifndef _COMMON_H_
#define _COMMON_H_


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

typedef int (*cb_t)( void* pCtx, void* pInfo );

typedef struct callback_s
{
	cb_t  cb;
	void* pCtx;
}callback_t;

typedef void* HANDLE;

#if defined (CYGWIN) || defined (WINDOWS)

typedef uint32_t base_t;
typedef int32_t  sbase_t;
/* Timer configuration:- high and low precision timer (hp & lp) */
typedef uint64_t stime_t;

#define MAX_BASE_VALUE (~((base_t)0x0))

typedef base_t bool;

#define TRUE 1
#define FALSE 0

#define INLINE static

typedef uint32_t irq_state_t;

#define Irq_Disable() 

#define Irq_Enable() 

#define ASSERT(str) printf("%s at line num = %d, in func = %s\n\r", str, __LINE__, __FUNCTION__ );



#endif /* defined (CYGWIN) || defined (WINDOWS) */

#endif /* _COMMON_H_ */
