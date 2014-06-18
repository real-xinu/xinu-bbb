/* seek.c - seek */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  seek  -  position a random access device
 *------------------------------------------------------------------------
 */
syscall	seek(
	  did32		descrp,		/* descriptor for device	*/
	  uint32	pos		/* position			*/
	)
{
	intmask		mask;		/* saved interrupt mask		*/
	struct dentry	*devptr;	/* entry in device switch table	*/
	int32		retval;		/* value to return to caller	*/

	mask = disable();
	if (isbaddev(descrp)) {
		restore(mask);
		return SYSERR;
	}
	devptr = (struct dentry *) &devtab[descrp];
	retval = (*devptr->dvseek) (devptr, pos);
	restore(mask);
	return retval;
}
