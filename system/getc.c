/* getc.c - getc */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  getc  -  obtain one byte from a device
 *------------------------------------------------------------------------
 */
syscall	getc(
	  did32		descrp		/* descriptor for device	*/
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
	retval = (*devptr->dvgetc) (devptr);
	restore(mask);
	return retval;
}
