/* eth_a_init.c - eth_a_init */

#include <xinu.h>

struct	eth_a_csreg eth_a_regs;

/*-----------------------------------------------------------------------
 * eth_a_init - initialize the TI AM335X ethernet hardware
 *-----------------------------------------------------------------------
 */
int32	eth_a_init	(
			struct	dentry *devptr
			)
{
	struct	ether *ethptr;
	struct	eth_a_tx_desc *tdescptr;
	struct	eth_a_rx_desc *rdescptr;
	struct	netpacket *pktptr;
	struct	eth_a_csreg *csrptr;
	uint32	phyreg;
	int32	retval;
	int32	i;

	ethptr = &ethertab[devptr->dvminor];

	csrptr = &eth_a_regs;
	ethptr->csr = csrptr;

	csrptr->ale = ETH_AM335X_ALE_ADDR;
	csrptr->cpdma = ETH_AM335X_CPDMA_ADDR;
	csrptr->sl = ETH_AM335X_SL1_ADDR;
	csrptr->stateram = ETH_AM335X_STATERAM_ADDR;
	csrptr->ss = ETH_AM335X_SS_ADDR;
	csrptr->wr = ETH_AM335X_WR_ADDR;
	csrptr->mdio = ETH_AM335X_MDIO_ADDR;

	csrptr->cpdma->reset = 1;
	while(csrptr->cpdma->reset == 1);

	csrptr->sl->reset = 1;
	while(csrptr->sl->reset == 1);

	csrptr->wr->reset = 1;
	while(csrptr->wr->reset == 1) ;

	csrptr->ss->reset = 1;
	while(csrptr->ss->reset == 1) ;

	csrptr->mdio->ctrl |= ETH_AM335X_MDIOCTL_EN;

	retval = eth_a_phy_reset(csrptr->mdio, 0);
	if(retval == SYSERR) {
		kprintf("Cannot reset Ethernet PHY\n");
		return SYSERR;
	}

	retval = eth_a_phy_read(csrptr->mdio, ETH_PHY_CTLREG, 0, &phyreg);
	if(retval == SYSERR) {
		return SYSERR;
	}

	if( (phyreg & ETH_PHY_CTLREG_SM) == ETH_PHY_10M ) {
		kprintf("Ethernet Link is Up. Speed is 10Mbps\n");
	}
	else if( (phyreg & ETH_PHY_CTLREG_SM) == ETH_PHY_100M ) {
		kprintf("Ethernet Link is Up. Speed is 100Mbps\n");
	}
	else if( (phyreg & ETH_PHY_CTLREG_SM) == ETH_PHY_1000M ) {
		kprintf("Ethernet Link is Up. Speed is 1000Mbps\n");
	}
	else {
		return SYSERR;
	}

	if(phyreg & ETH_PHY_CTLREG_FD) {
		kprintf("Link is Full Duplex\n");
		csrptr->sl->macctrl |= ETH_AM335X_SLCTL_FD;
	}
	else {
		kprintf("Link is Half Duplex\n");
	}

	/* Initialize the rx ring size field */
	ethptr->rxRingSize = ETH_AM335X_RX_RING_SIZE;

	/* Allocate memory for the rx ring */
	ethptr->rxRing = (void*)getmem(sizeof(struct eth_a_rx_desc)*
					ethptr->rxRingSize);
	if((int32)ethptr->rxRing == SYSERR) {
		return SYSERR;
	}

	/* Zero out the rx ring */
	memset((char*)ethptr->rxRing, NULLCH,
		sizeof(struct eth_a_rx_desc)*ethptr->rxRingSize);

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

	rdescptr = (struct eth_a_rx_desc *)ethptr->rxRing;
	pktptr = (struct netpacket *)ethptr->rxBufs;

	for(i = 0; i < ethptr->rxRingSize; i++) {
		rdescptr->next = rdescptr + 1;
		rdescptr->buffer = (uint32)pktptr->net_ethdst;
		rdescptr->buflen = sizeof(struct netpacket) - 2;
		rdescptr->bufoff = 0;
		rdescptr->stat = ETH_AM335X_RDS_OWN;
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
	ethptr->txRingSize = ETH_AM335X_TX_RING_SIZE;

	/* Allocate memory for tx ring */
	ethptr->txRing = (void*)getmem(sizeof(struct eth_a_tx_desc)*
					ethptr->txRingSize);
	if((int32)ethptr->txRing == SYSERR) {
		return SYSERR;
	}

	/* Zero out the tx ring */
	memset((char*)ethptr->txRing, NULLCH,
		sizeof(struct eth_a_tx_desc)*ethptr->txRingSize);

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

	tdescptr = (struct eth_a_tx_desc *)ethptr->txRing;
	pktptr = (struct netpacket *)ethptr->txBufs;

	for(i = 0; i < ethptr->txRingSize; i++) {
		tdescptr->next = NULL;
		tdescptr->buffer = (uint32)pktptr->net_ethdst;
		tdescptr->buflen = sizeof(struct netpacket) - 2;
		tdescptr->bufoff = 0;
		tdescptr->stat = (ETH_AM335X_TDS_SOP |
				  ETH_AM335X_TDS_EOP |
				  ETH_AM335X_TDS_DIR |
				  ETH_AM335X_TDS_P1);
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
	csrptr->ale->ctrl = (ETH_AM335X_ALECTL_EN | 
			     ETH_AM335X_ALECTL_BY);

	/* Put the ports 0, 1 in forwarding state */
	csrptr->ale->portctl[0] = ETH_AM335X_ALEPCTL_FWD;
	csrptr->ale->portctl[1] = ETH_AM335X_ALEPCTL_FWD;

	/* Start the rx and tx processes in DMA */
	csrptr->cpdma->tx_ctrl = 1;
	csrptr->cpdma->rx_ctrl = 1;

	/* Initialize the head desc pointers for tx and rx */
	csrptr->stateram->tx_hdp[0] = 0;
	csrptr->stateram->rx_hdp[0] = (uint32)ethptr->rxRing;

	/* Enable Rx and Tx in MAC */
	csrptr->sl->macctrl |= ETH_AM335X_SLCTL_EN;

	/* Set interrupt vectors */
	set_evec(ETH_AM335X_TXINT, (uint32)devptr->dvintr);
	set_evec(ETH_AM335X_RXINT, (uint32)devptr->dvintr);

	/* Enable the CPDMA interrupts */
	csrptr->cpdma->tx_intmask_set = 0x1;
	csrptr->cpdma->rx_intmask_set = 0x1;

	/* Route the interrupts to core 0 */
	csrptr->wr->c0_tx_en = 0x1;
	csrptr->wr->c0_rx_en = 0x1;

	return OK;
}

/*-----------------------------------------------------------------------
 * eth_a_phy_reset - Reset an Ethernet PHY
 *-----------------------------------------------------------------------
 */
int32	eth_a_phy_reset (
			volatile struct	eth_a_mdio *mdio,
			byte	phyadr
			)
{
	int32	retval;
	uint32	phyreg;

	/* Read the PHY Control Register */

	retval = eth_a_phy_read(mdio, ETH_PHY_CTLREG, phyadr, &phyreg);
	if(retval == SYSERR) {
		return SYSERR;
	}

	/* Set the Reset bit and write the register */

	phyreg |= ETH_PHY_CTLREG_RESET;
	eth_a_phy_write(mdio, ETH_PHY_CTLREG, phyadr, phyreg);

	sleep(1);

	/* Check if the reset operation is complete */

	retval = eth_a_phy_read(mdio, ETH_PHY_CTLREG, phyadr, &phyreg);
	if(retval == SYSERR) {
		return SYSERR;
	}

	if( (phyreg & ETH_PHY_CTLREG_RESET) != 0 ) {
		return SYSERR;
	}

	sleep(1);

	/* Check if the Link is established */

	retval = eth_a_phy_read(mdio, ETH_PHY_STATREG, phyadr, &phyreg);
	if(retval == SYSERR) {
		return SYSERR;
	}

	if( (phyreg & ETH_PHY_STATREG_LINK) == 0 ) {
		return SYSERR;
	}

	return OK;
}
/*-----------------------------------------------------------------------
 * eth_a_phy_read - read a PHY register
 *-----------------------------------------------------------------------
 */
int32	eth_a_phy_read (
			volatile struct	eth_a_mdio *mdio,
			byte	regadr,
			byte	phyadr,
			uint32	*value
			)
{

	/* Ethernet PHY has only 32 registers */

	if(regadr > 31) {
		return SYSERR;
	}

	/* Only 32 possible PHY addresses */

	if(phyadr > 31) {
		return SYSERR;
	}

	/* Wait for the previous access to complete */

	while( (mdio->useraccess0 & ETH_AM335X_MDIOUA_GO) != 0 );

	/* Start the access */

	mdio->useraccess0 = (ETH_AM335X_MDIOUA_GO) |
			    (regadr << 21) |
			    (phyadr << 16);

	/* Wait until the access is complete */

	while( (mdio->useraccess0 & ETH_AM335X_MDIOUA_GO) != 0 );

	/* Check if the access was successful */

	if( (mdio->useraccess0 & ETH_AM335X_MDIOUA_ACK) == 0 ) {
		return SYSERR;
	}

	/* Copy the value read */

	(*value) = mdio->useraccess0 & ETH_AM335X_MDIOUA_DM;

	return OK;
}

/*-----------------------------------------------------------------------
 * eth_a_phy_write - write a PHY register
 *-----------------------------------------------------------------------
 */
int32	eth_a_phy_write (
			volatile struct	eth_a_mdio *mdio,
			byte	regadr,
			byte	phyadr,
			uint32	value
			)
{

	/* There are only 32 PHY registers */

	if(regadr > 31) {
		return SYSERR;
	}

	/* There are only 32 possible PHY addresses */

	if(phyadr > 31) {
		return SYSERR;
	}

	/* Wait for the previous access to complete */

	while( (mdio->useraccess0 & ETH_AM335X_MDIOUA_GO) != 0);

	/* Start the access */

	mdio->useraccess0 = ETH_AM335X_MDIOUA_GO |
			    ETH_AM335X_MDIOUA_WR |
			    (regadr << 21) |
			    (phyadr << 16) |
			    (value & 0xffff);

	/* Wait for the access to complete */

	while( (mdio->useraccess0 & ETH_AM335X_MDIOUA_GO) != 0);

	return OK;
}
