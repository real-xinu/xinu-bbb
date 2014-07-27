/* am335x_eth_write.c - am335x_eth_write */

#include <xinu.h>

/*------------------------------------------------------------------------
 * am335x_eth_write - enqueue a packet for transmission
 *------------------------------------------------------------------------
 */
int32	am335x_eth_write (
			struct	ether *ethptr,
			void	*buf,
			uint32	count
			)
{
	struct	am335x_eth_csreg *csrptr;	/* Ethernet CSR pointer	*/
	struct	am335x_eth_tx_desc *tdescptr;	/* Tx Desc. pointer	*/
	struct	am335x_eth_tx_desc *prev;	/* Prev. Desc. pointer	*/

	kprintf("OUT :");
	pdump(buf);
	/* Get the pointer to the Ethernet CSR */
	csrptr = (struct am335x_eth_csreg *)ethptr->csr;

	/* Wait for an empty slot in the queue */
	wait(ethptr->osem);

	/* Get the pointer to the next descriptor */
	tdescptr = (struct am335x_eth_tx_desc *)ethptr->txRing +
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
	tdescptr->stat = (AM335X_ETH_TDS_SOP |	/* Start of packet	*/
			  AM335X_ETH_TDS_EOP |	/* End of packet	*/
			  AM335X_ETH_TDS_OWN |	/* Own flag set for DMA	*/
			  AM335X_ETH_TDS_DIR |	/* Directed packet	*/
			  AM335X_ETH_TDS_P1);	/* Output port is port1	*/

	/* Copy the packet into the Tx buffer */
	memcpy((char *)tdescptr->buffer, buf, count);

	/* Insert the descriptor into Tx queue */

	if(csrptr->stateram->tx_hdp[0] == 0) {
		/* Tx queue is empty, this desc. will be the first */
		csrptr->stateram->tx_hdp[0] = (uint32)tdescptr;
	}
	else {
		/* Tx queue not empty, insert at end */
		prev = (struct am335x_eth_tx_desc *)
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
