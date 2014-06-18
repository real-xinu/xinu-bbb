/* ethRead.c  - ethRead */

#include <xinu.h>

/*------------------------------------------------------------------------
 * ethRead - read a packet from an Ethernet device
 *------------------------------------------------------------------------
 */
devcall	ethRead (
	struct	dentry	*devptr,	/* entry in device switch table	*/
	void	*buf,			/* buffer to hold packet	*/
	uint32	len			/* length of buffer		*/
		)
{
	struct	ether	*ethptr;	/* ptr to entry in ethertab	*/

	ethptr = &ethertab[devptr->dvminor];

	/* Verify Ethernet interface is up and arguments are valid */

	if ((ETH_STATE_UP != ethptr->state)
			|| (len < ETH_HDR_LEN)) {
		return SYSERR;
	}

	return ethptr->ethRead(ethptr, buf, len);
}
