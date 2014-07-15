struct	am335x_eth_cpdma {
	uint32	tx_idver;
	uint32	tx_ctrl;
	uint32	tx_teardown;
	uint32	rx_idver;
	uint32	rx_ctrl;
	uint32	rx_teardown;
	uint32	reset;
	uint32	dmactrl;
	uint32	dmastat;
	uint32	rx_buf_off;
	uint32	emctrl;
	uint32	tx_pri_rate[8];
	byte	res1[48];
	uint32	tx_intstat_raw;
	uint32	tx_intstat_masked;
	uint32	tx_intmask_set;
	uint32	tx_intmask_clear;
	uint32	in_vector;
	uint32	eoi_vector;
	byte	res2[8];
	uint32	rx_intstat_raw;
	uint32	rx_intstat_masked;
	uint32	rx_intmask_set;
	uint32	rx_intmask_clear;
	uint32	dma_intstat_raw;
	uint32	dma_intstat_masket;
	uint32	dma_intmask_set;
	uint32	dma_intmask_clear;
	uint32	rx_pendthresh[8];
	uint32	rx_freebuffer[8];
};

struct	am335x_eth_stateram {
	uint32	tx_hdp[8];
	uint32	rx_hdp[8];
	uint32	tx_cp[8];
	uint32	rx_cp[8];
};

struct	am335x_eth_sl {
	uint32	idver;
	uint32	macctrl;
	uint32	macstat;
	uint32	reset;
	uint32	rx_maxlen;
	uint32	bofftest;
	uint32	rx_pause;
	uint32	tx_pause;
	uint32	emctrl;
	uint32	rx_pri_map;
	uint32	tx_gap;
};

struct	am335x_eth_mdio {
	uint32	ver;
	uint32	ctrl;
	uint32	alive;
	uint32	link;
	uint32	linkintraw;
	uint32	linkintmasked;
	byte	res1[8];
	uint32	userintraw;
	uint32	userintmasked;
	uint32	userintmaskset;
	uint32	userintmaskclr;
	byte	res2[80];
	uint32	useraccess0;
	uint32	userphysel0;
	uint32	useraccess1;
	uint32	userphysel1;
};

#define AM335X_ETH_MDIOCTL_EN	0x40000000

#define AM335X_ETH_MDIOUA_GO	0x80000000 /* Perorm MDIO access*/
#define AM335X_ETH_MDIOUA_WR	0x40000000 /* Write access	*/
#define AM335X_ETH_MDIOUA_ACK	0x20000000 /* Read Ack		*/
#define AM335X_ETH_MDIOUA_DM	0x0000ffff /* MDIO Data Mask	*/

struct	am335x_eth_csreg {
	volatile struct	am335x_eth_cpdma 	*cpdma;
	volatile struct	am335x_eth_stateram 	*stateram;
	volatile struct	am335x_eth_sl	 	*sl;
	volatile struct	am335x_eth_mdio	 	*mdio;
};

struct	am335x_eth_rx_desc {
	struct	am335x_eth_rx_desc *next;
	uint32	buffer;
	uint16	buflen;
	uint16	bufoff;
	uint16	packlen;
	uint16	stat;
};

#define AM335X_ETH_RDS_SOP	0x8000	/* Start of packet	*/
#define AM335X_ETH_RDS_EOP	0x4000
#define AM335X_ETH_RDS_OWN	0x2000
#define AM335X_ETH_RDS_EOQ	0x1000

struct	am335x_eth_tx_desc {
	struct	am335x_eth_tx_desc *next;
	uint32	buffer;
	uint16	buflen;
	uint16	bufoff;
	uint16	packlen;
	uint16	stat;
};

extern	am335x_eth_phy_read(volatile struct am335x_eth_mdio *, byte, byte, uint32 *);
extern	am335x_eth_phy_write(volatile struct am335x_eth_mdio *, byte, byte, uint32);
