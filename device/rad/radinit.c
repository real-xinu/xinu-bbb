/* radinit.c - radinit */

#include <xinu.h>

struct	radcblk radtab[1];

/*------------------------------------------------------------------------
 * radinit  -  Initialize the radio device
 *------------------------------------------------------------------------
 */
devcall	radinit (
		 struct	dentry *devptr	/* Address in device table	*/
		)
{
	struct	radcblk *radptr;	/* Pointer to control block	*/

	kprintf("radinit, minor %d\n", devptr->dvminor);

	/* Initialize the control block pointer */

	radptr = &radtab[devptr->dvminor];

	/* Clear the control block */

	memset(radptr, 0, sizeof(struct radcblk));

	/* Set the SPI device */

	if(devptr->dvminor == 0) {
		radptr->spidev = SPI1;
	}
	else {
		radptr->spidev = SPI0;
	}

	/* Set the address length and the MTU */

	radptr->addrLen = RAD_ADDR_LEN;
	radptr->mtu = 128;

	/* Put the device in 802.15.4 mode */

	ticc1200_write(radptr, TICC1200_ADDR_PKTCFG2,
			       TICC1200_PKTCFG2_FGMODE);

	return OK;
}
