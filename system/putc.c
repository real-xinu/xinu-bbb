/* putc.c - putc */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  putc  -  send one character of data (byte) to a device
 *------------------------------------------------------------------------
 */
syscall	putc(
	  did32		descrp,		/* descriptor for device	*/
	  char		ch		/* character to send		*/
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
	retval = (*devptr->dvputc) (devptr, ch);
	restore(mask);
	return retval;
}
