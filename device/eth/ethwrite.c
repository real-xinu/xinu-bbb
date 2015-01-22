/* ethwrite.c - ethwrite */

#include <xinu.h>

/*------------------------------------------------------------------------
 * ethwrite - enqueue a packet for transmission on TI AM335X Ethernet
 *------------------------------------------------------------------------
 */
int32	ethwrite (
		struct	dentry *devptr,
		void	*buf,
		uint32	count
	)
{
	struct	ethcblk *ethptr;	/* Ether entry pointer	*/
	struct	eth_a_csreg *csrptr;	/* Ethernet CSR pointer	*/
	struct	eth_a_tx_desc *tdescptr;/* Tx Desc. pointer	*/
	struct	eth_a_tx_desc *prev;	/* Prev. Desc. pointer	*/

	ethptr = &ethertab[devptr->dvminor];

	/* Get the pointer to the Ethernet CSR */
	csrptr = (struct eth_a_csreg *)ethptr->csr;

	/* Wait for an empty slot in the queue */
	wait(ethptr->osem);

	/* Get the pointer to the next descriptor */
	tdescptr = (struct eth_a_tx_desc *)ethptr->txRing +
							ethptr->txTail;

	/* Adjust count if greater than max. possible packet size */
	if(count > PACKLEN) {
		count = PACKLEN;
	}

	/* Initialize the descriptor */
	tdescptr->next = NULL;
	tdescptr->buflen = count;
	tdescptr->bufoff = 0;
	tdescptr->packlen = count;
	tdescptr->stat = (ETH_AM335X_TDS_SOP |	/* Start of packet	*/
			  ETH_AM335X_TDS_EOP |	/* End of packet	*/
			  ETH_AM335X_TDS_OWN |	/* Own flag set for DMA	*/
			  ETH_AM335X_TDS_DIR |	/* Directed packet	*/
			  ETH_AM335X_TDS_P1);	/* Output port is port1	*/

	/* Copy the packet into the Tx buffer */
	memcpy((char *)tdescptr->buffer, buf, count);

	/* Insert the descriptor into Tx queue */

	if(csrptr->stateram->tx_hdp[0] == 0) {
		/* Tx queue is empty, this desc. will be the first */
		csrptr->stateram->tx_hdp[0] = (uint32)tdescptr;
	}
	else {
		/* Tx queue not empty, insert at end */
		prev = (struct eth_a_tx_desc *)
				csrptr->stateram->tx_hdp[0];
		while(prev->next != NULL) {
			prev = prev->next;
		}
		prev->next = tdescptr;
	}

	/* Increment the tail index of the Tx ring */
	ethptr->txTail++;
	if(ethptr->txTail >= ethptr->txRingSize) {
		ethptr->txTail = 0;
	}

	return count;
}
