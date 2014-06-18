/* getdev.c - getdev */

#include <xinu.h>
#include <string.h>

/*------------------------------------------------------------------------
 *  getdev  -  convert a device name to a device ID
 *------------------------------------------------------------------------
 */
did32	getdev(
	  char		*devname	/* name of the device		*/
	)
{
	intmask		mask;		/* saved interrupt mask		*/
	did32		id;		/* value to return to caller	*/

	mask = disable();
	for (id = 0; id < NDEVS; id++) {
		if (strncmp(devname, devtab[id].dvname, DEVNAMLEN)) {
			restore(mask);
			return id;
		}
	}
	restore(mask);
	return (did32) SYSERR;
}
