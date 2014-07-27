/* am335x_eth_init.c - am335x_eth_init */

#include <xinu.h>

struct	am335x_eth_csreg am335x_eth_regs;

/*-----------------------------------------------------------------------
 * am335x_eth_init - initialize the ethernet hardware
 *-----------------------------------------------------------------------
 */
int32	am335x_eth_init	(
			struct	ether *ethptr
			)
{
	struct	am335x_eth_csreg *csrptr;
	uint32	phyreg;
	int32	retval;

	csrptr = &am335x_eth_regs;
	ethptr->csr = csrptr;

	csrptr->ale = AM335X_ETH_ALE_ADDR;
	csrptr->cpdma = AM335X_ETH_CPDMA_ADDR;
	csrptr->sl = AM335X_ETH_SL1_ADDR;
	csrptr->stateram = AM335X_ETH_STATERAM_ADDR;
	csrptr->ss = AM335X_ETH_SS_ADDR;
	csrptr->wr = AM335X_ETH_WR_ADDR;
	csrptr->mdio = AM335X_ETH_MDIO_ADDR;

	csrptr->cpdma->reset = 1;
	while(csrptr->cpdma->reset == 1);

	csrptr->sl->reset = 1;
	while(csrptr->sl->reset == 1);

	csrptr->wr->reset = 1;
	while(csrptr->wr->reset == 1) ;

	csrptr->ss->reset = 1;
	while(csrptr->ss->reset == 1) ;

	csrptr->mdio->ctrl |= AM335X_ETH_MDIOCTL_EN;

	retval = am335x_eth_phy_reset(csrptr->mdio, 0);
	if(retval == SYSERR) {
		kprintf("Cannot reset Ethernet PHY\n");
		return SYSERR;
	}

	retval = am335x_eth_phy_read(csrptr->mdio, ETH_PHY_CTLREG, 0, &phyreg);
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
		csrptr->sl->macctrl |= AM335X_ETH_SLCTL_FD;
	}
	else {
		kprintf("Link is Half Duplex\n");
	}

	return OK;
}

/*-----------------------------------------------------------------------
 * am335x_eth_phy_reset - Reset an Ethernet PHY
 *-----------------------------------------------------------------------
 */
int32	am335x_eth_phy_reset (
				volatile struct	am335x_eth_mdio *mdio,
				byte	phyadr
			     )
{
	int32	retval;
	uint32	phyreg;

	/* Read the PHY Control Register */

	retval = am335x_eth_phy_read(mdio, ETH_PHY_CTLREG, phyadr, &phyreg);
	if(retval == SYSERR) {
		return SYSERR;
	}

	/* Set the Reset bit and write the register */

	phyreg |= ETH_PHY_CTLREG_RESET;
	am335x_eth_phy_write(mdio, ETH_PHY_CTLREG, phyadr, phyreg);

	sleep(1);

	/* Check if the reset operation is complete */

	retval = am335x_eth_phy_read(mdio, ETH_PHY_CTLREG, phyadr, &phyreg);
	if(retval == SYSERR) {
		return SYSERR;
	}

	if( (phyreg & ETH_PHY_CTLREG_RESET) != 0 ) {
		return SYSERR;
	}

	sleep(1);

	/* Check if the Link is established */

	retval = am335x_eth_phy_read(mdio, ETH_PHY_STATREG, phyadr, &phyreg);
	if(retval == SYSERR) {
		return SYSERR;
	}

	if( (phyreg & ETH_PHY_STATREG_LINK) == 0 ) {
		return SYSERR;
	}

	return OK;
}
/*-----------------------------------------------------------------------
 * am335x_eth_phy_read - read a PHY register
 *-----------------------------------------------------------------------
 */
int32	am335x_eth_phy_read (
				volatile struct	am335x_eth_mdio *mdio,
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

	while( (mdio->useraccess0 & AM335X_ETH_MDIOUA_GO) != 0 );

	/* Start the access */

	mdio->useraccess0 = (AM335X_ETH_MDIOUA_GO) |
			    (regadr << 21) |
			    (phyadr << 16);

	/* Wait until the access is complete */

	while( (mdio->useraccess0 & AM335X_ETH_MDIOUA_GO) != 0 );

	/* Check if the access was successful */

	if( (mdio->useraccess0 & AM335X_ETH_MDIOUA_ACK) == 0 ) {
		return SYSERR;
	}

	/* Copy the value read */

	(*value) = mdio->useraccess0 & AM335X_ETH_MDIOUA_DM;

	return OK;
}

/*-----------------------------------------------------------------------
 * am335x_eth_phy_write - write a PHY register
 *-----------------------------------------------------------------------
 */
int32	am335x_eth_phy_write (
				volatile struct	am335x_eth_mdio *mdio,
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

	while( (mdio->useraccess0 & AM335X_ETH_MDIOUA_GO) != 0);

	/* Start the access */

	mdio->useraccess0 = AM335X_ETH_MDIOUA_GO |
			    AM335X_ETH_MDIOUA_WR |
			    (regadr << 21) |
			    (phyadr << 16) |
			    (value & 0xffff);

	/* Wait for the access to complete */

	while( (mdio->useraccess0 & AM335X_ETH_MDIOUA_GO) != 0);

	return OK;
}
