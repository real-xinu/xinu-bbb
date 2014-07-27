/* am335x_eth_read.c am335x_eth_read */

#include <xinu.h>

/*------------------------------------------------------------------------
 * am335x_eth_read - read an incoming Ethernet packet
 *------------------------------------------------------------------------
 */
int32	am335x_eth_read (
			struct	ether *ethptr,
			void	*buf,
			uint32	count
			)
{
	struct	am335x_eth_csreg *csrptr;	/* Ethernet CSR pointer	*/
	struct	am335x_eth_rx_desc *rdescptr;	/* Rx Desc. pointer	*/
	struct	am335x_eth_rx_desc *prev;	/* Prev Rx desc pointer	*/
	uint32	retval;				/* Num of bytes returned*/

	/* Get the pointer to Ethernet CSR */
	csrptr = (struct am335x_eth_csreg *)ethptr->csr;

	/* Wait for a packet */
	//kprintf("waiting\n");
	wait(ethptr->isem);
	//kprintf("signaled\n");

	/* Get pointer to the descriptor */
	rdescptr = (struct am335x_eth_rx_desc *)ethptr->rxRing +
						ethptr->rxHead;

	/* Read the packet length */
	retval = rdescptr->packlen;
	if(retval > count) {
		retval = count;
	}

	/* Copy the packet into user provided buffer */
	memcpy((char *)buf, (char *)rdescptr->buffer, retval);

	/* Initialize the descriptor for next packet */
	rdescptr->stat = AM335X_ETH_RDS_OWN;
	rdescptr->bufoff = 0;
	rdescptr->buflen = 1518;
	rdescptr->packlen = 0;
	rdescptr->next = NULL;

	/* Insert the descriptor into Rx queue */
	prev = (struct am335x_eth_rx_desc *)csrptr->stateram->rx_hdp[0];
	if(prev == NULL) {
		kprintf("hdp 0, adding %x\n", rdescptr);
		csrptr->stateram->rx_hdp[0] = (uint32)rdescptr;
	}
	else {
		while(prev->next != NULL) {
			prev = prev->next;
		}
		prev->next = rdescptr;
	}

	/* Increment the head index of rx ring */
	ethptr->rxHead++;
	if(ethptr->rxHead >= ethptr->rxRingSize) {
		ethptr->rxHead = 0;
	}

	return retval;
}
