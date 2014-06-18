/* write.c - write */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  write  -  write one or more bytes to a device
 *------------------------------------------------------------------------
 */
syscall	write(
	  did32		descrp,		/* descriptor for device	*/
	  char		*buffer,	/* address of buffer		*/
	  uint32	count		/* length of buffer		*/
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
	retval = (*devptr->dvwrite) (devptr, buffer, count);
	restore(mask);
	return retval;
}
