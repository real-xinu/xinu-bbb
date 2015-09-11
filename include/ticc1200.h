/* ticc1200.h */

#define	TICC1200_COM_READ	0x80	/* Read command			*/
#define	TICC1200_COM_WRITE	0x00	/* Write command		*/

#define	TICC1200_COM_BURST	0x40	/* Burst Access			*/

#define	TICC1200_COM_EXT	0x2F	/* Extended register access	*/

#define	TICC1200_COM_SFIFO	0x3F	/* Standard FIFO access		*/
#define	TICC1200_COM_DFIFO	0x3E	/* Direct FIFO access		*/

/* Register addresses in normal space */

#define	TICC1200_ADDR_PKTCFG2	0x26	/* Packet configuration 2	*/
#define	TICC1200_ADDR_PKTCFG1	0x27	/* Packet configuration 1	*/

/* Register addresses in extended space */

#define	TICC1200_ADDR_RXFIRST	0xD2
#define	TICC1200_ADDR_TXFIRST	0xD3
#define	TICC1200_ADDR_RXLAST	0xD4
#define	TICC1200_ADDR_TXLAST	0xD5

#define	TICC1200_PKTCFG2_FGMODE	0x20	/* 802.15.4 mode		*/

#define	TICC1200_PKTCFG1_CRCOFF	0x00	/* CRC off			*/
#define	TICC1200_PKTCFG1_CRC1	0x02
#define	TICC1200_PKTCFG1_CRC2	0x04
#define	TICC1200_PKTCFG1_CRC3	0x06

#define	TICC1200_PKTCFG1_AS	0x01	/* Append status		*/
