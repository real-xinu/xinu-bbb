/* ucalloc.c - ucalloc */

#include <xinu.h>

/*------------------------------------------------------------------------
 * ucalloc  -  Allocate memory from free uncached memory
 *------------------------------------------------------------------------
 */
char	*ucalloc (
		uint32	nbytes	/* Size of memory requested	*/
		)
{
	intmask	mask;		/* Saved interrupt mask	*/
	char	*retval;	/* Return value		*/

	if(nbytes == 0) {
		return (char *)SYSERR;
	}

	mask = disable();

	nbytes = (uint32) roundmb(nbytes);

	if(ucmemlist.mlength < nbytes) { /* Not enough free memory */
		restore(mask);
		return (char *)SYSERR;
	}

	/* Allocate memory from start of free memory */

	retval = (char *)ucmemlist.mnext;

	/* Update free block and remaining memory */

	ucmemlist.mnext = (struct memblk *)((char *)ucmemlist.mnext +
							nbytes);
	ucmemlist.mlength -= nbytes;

	restore(mask);
	return retval;
}
