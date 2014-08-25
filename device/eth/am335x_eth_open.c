/* am335x_eth_open.c - am335x_eth_open */

#include <xinu.h>

/*-----------------------------------------------------------------------
 * am335x_eth_open - allocate buffers for the ethernet driver
 *-----------------------------------------------------------------------
 */
int32	am335x_eth_open (
			struct	ether *ethptr
			)
{
	struct	am335x_eth_csreg *csrptr;	/* Ethernet CSR pointer	*/
	struct	am335x_eth_rx_desc *rdescptr;	/* Rx desc pointer	*/
	struct	am335x_eth_tx_desc *tdescptr;	/* Tx desc pointer	*/
	struct	netpacket *pktptr;		/* packet pointer	*/
	int32	i;

	csrptr = ethptr->csr;

	/* Initialize the rx ring size field */
	ethptr->rxRingSize = AM335X_ETH_RX_RING_SIZE;

	/* Allocate memory for the rx ring */
	ethptr->rxRing = (void*)getmem(sizeof(struct am335x_eth_rx_desc)*
					ethptr->rxRingSize);
	if((int32)ethptr->rxRing == SYSERR) {
		return SYSERR;
	}

	/* Zero out the rx ring */
	memset((char*)ethptr->rxRing, NULLCH,
		sizeof(struct am335x_eth_rx_desc)*ethptr->rxRingSize);

	/* Allocate memory for rx buffers */
	ethptr->rxBufs = (void*)getmem(sizeof(struct netpacket) *
					ethptr->rxRingSize);
	if((int32)ethptr->rxBufs == SYSERR) {
		return SYSERR;
	}

	/* Zero out the rx buffers */
	memset((char *)ethptr->rxBufs, NULLCH, sizeof(struct netpacket) *
						ethptr->rxRingSize);

	/* Initialize the rx ring */

	rdescptr = (struct am335x_eth_rx_desc *)ethptr->rxRing;
	pktptr = (struct netpacket *)ethptr->rxBufs;

	for(i = 0; i < ethptr->rxRingSize; i++) {
		rdescptr->next = rdescptr + 1;
		rdescptr->buffer = (uint32)pktptr->net_ethdst;
		rdescptr->buflen = sizeof(struct netpacket) - 2;
		rdescptr->bufoff = 0;
		rdescptr->stat = AM335X_ETH_RDS_OWN;
		rdescptr++;
		pktptr++;
	}
	(--rdescptr)->next = NULL;

	ethptr->rxHead = 0;
	ethptr->rxTail = 0;
	ethptr->isem = semcreate(0);
	if((int32)ethptr->isem == SYSERR) {
		return SYSERR;
	}

	/* initialize the tx ring size */
	ethptr->txRingSize = AM335X_ETH_TX_RING_SIZE;

	/* Allocate memory for tx ring */
	ethptr->txRing = (void*)getmem(sizeof(struct am335x_eth_tx_desc)*
					ethptr->txRingSize);
	if((int32)ethptr->txRing == SYSERR) {
		return SYSERR;
	}

	/* Zero out the tx ring */
	memset((char*)ethptr->txRing, NULLCH,
		sizeof(struct am335x_eth_tx_desc)*ethptr->txRingSize);

	/* Allocate memory for tx buffers */
	ethptr->txBufs = (void*)getmem(sizeof(struct netpacket) *
					ethptr->txRingSize);
	if((int32)ethptr->txBufs == SYSERR) {
		return SYSERR;
	}

	/* Zero out the tx buffers */
	memset((char*)ethptr->txBufs, NULLCH, sizeof(struct netpacket) *
						ethptr->txRingSize);

	/* Initialize the tx ring */

	tdescptr = (struct am335x_eth_tx_desc *)ethptr->txRing;
	pktptr = (struct netpacket *)ethptr->txBufs;

	for(i = 0; i < ethptr->txRingSize; i++) {
		tdescptr->next = NULL;
		tdescptr->buffer = (uint32)pktptr->net_ethdst;
		tdescptr->buflen = sizeof(struct netpacket) - 2;
		tdescptr->bufoff = 0;
		tdescptr->stat = (AM335X_ETH_TDS_SOP |
				  AM335X_ETH_TDS_EOP |
				  AM335X_ETH_TDS_DIR |
				  AM335X_ETH_TDS_P1);
		tdescptr++;
		pktptr++;
	}

	ethptr->txHead = 0;
	ethptr->txTail = 0;
	ethptr->osem = semcreate(ethptr->txRingSize);
	if((int32)ethptr->osem == SYSERR) {
		return SYSERR;
	}

	/* Enable the ALE and put it into bypass mode */
	csrptr->ale->ctrl = (AM335X_ETH_ALECTL_EN |
			     AM335X_ETH_ALECTL_BY);

	/* Put the ports 0, 1 in forwarding state */
	csrptr->ale->portctl[0] = AM335X_ETH_ALEPCTL_FWD;
	csrptr->ale->portctl[1] = AM335X_ETH_ALEPCTL_FWD;

	/* Start the rx and tx processes in DMA */
	csrptr->cpdma->tx_ctrl = 1;
	csrptr->cpdma->rx_ctrl = 1;

	/* Initialize the head desc pointers for tx and rx */
	csrptr->stateram->tx_hdp[0] = 0;
	csrptr->stateram->rx_hdp[0] = (uint32)ethptr->rxRing;

	/* Enable Rx and Tx in MAC */
	csrptr->sl->macctrl |= AM335X_ETH_SLCTL_EN;

	/* Set interrupt vectors */
	set_evec(AM335X_ETH_TXINT, am335x_eth_intr);
	set_evec(AM335X_ETH_RXINT, am335x_eth_intr);

	/* Enable the CPDMA interrupts */
	csrptr->cpdma->tx_intmask_set = 0x1;
	csrptr->cpdma->rx_intmask_set = 0x1;

	/* Route the interrupts to core 0 */
	csrptr->wr->c0_tx_en = 0x1;
	csrptr->wr->c0_rx_en = 0x1;
	return OK;
}
