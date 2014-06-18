/* bufinit.c - bufinit */

#include <xinu.h>

struct	bpentry	buftab[NBPOOLS];		/* buffer pool table		*/
bpid32	nbpools;

/*------------------------------------------------------------------------
 *  bufinit  --  initialize the buffer pool data structure
 *------------------------------------------------------------------------
 */
status	bufinit(void)
{
	nbpools = 0;
	return OK;
}
