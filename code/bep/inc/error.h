typedef base_t err_t; 

/* Definitions for error constants. */

#define ERR_OK          0    /* No error, everything OK. */
#define ERR_MEM         1    /* Out of memory error.     */
#define ERR_BUF         2    /* Buffer error.            */
#define ERR_TIMEOUT     3    /* Timeout.                 */
#define ERR_NET_RTE     4    /* Routing problem.         */


#define ERR_ABRT        5    /* Connection aborted.      */
#define ERR_RST         6    /* Connection reset.        */
#define ERR_CLSD        7    /* Connection closed.       */
#define ERR_CONN        8    /* Not connected.           */
#define ERR_WNDFULL     9    /* Window size full, for TCP*/
                        
#define ERR_VAL         10    /* Illegal value.           */
                        
#define ERR_ARG         11   /* Illegal argument.        */
                        
#define ERR_USE         12   /* Is in use.          */
                        
#define ERR_IF          13   /* Low-level netif error    */
#define ERR_ISCONN      14   /* Already connected.       */
                        
#define ERR_INPROGRESS  15   /* Operation in progress    */
#define ERR_EXP         16   /* Expired.                 */
#define ERR_AGED        17   /* Aged.                    */
#define ERR_UNINIT      18   /* Not initilized.         */
#define ERR_WSTATE      19 
#define ERR_FAILURE     20
                        
