/* control.c - control */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  control  -  control a device or a driver (e.g., set the driver mode)
 *------------------------------------------------------------------------
 */
syscall	control(
	  did32		descrp,		/* descriptor for device	*/
	  int32		func,		/* specific control function	*/
	  int32		arg1,		/* specific argument for func	*/
	  int32		arg2		/* specific argument for func	*/
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
	retval = (*devptr->dvcntl) (devptr, func, arg1, arg2);
	restore(mask);
	return retval;
}
