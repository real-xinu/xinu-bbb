/* open.c - open */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  open  -  open a device (some devices ignore name and mode parameters)
 *------------------------------------------------------------------------
 */
syscall	open(
	  did32		descrp,		/* descriptor for device	*/
	  char		*name,		/* name to use, if any		*/
	  char		*mode		/* mode for device, if any	*/
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
	retval = (*devptr->dvopen) (devptr, name, mode);
	restore(mask);
	return retval;
}
