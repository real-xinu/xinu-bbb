/* am335x_eth_init.c - am335x_eth_init */

#include <xinu.h>

/*-----------------------------------------------------------------------
 * am335x_eth_init - initialize the ethernet hardware
 *-----------------------------------------------------------------------
 */
int32	am335x_eth_init	(
			struct	ether *ethptr
			)
{
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
