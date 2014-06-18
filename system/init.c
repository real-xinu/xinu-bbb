/* init.c - init */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  init  -  initialize a device and its driver
 *------------------------------------------------------------------------
 */
syscall	init(
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
	retval = (*devptr->dvinit) (devptr);
	restore(mask);
	return retval;
}
