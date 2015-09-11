/* ticc1200_regs.c - ticc1200_read, ticc1200_write, ticc1200_read_ext, ticc1200_write_ext */

#include <xinu.h>

/*------------------------------------------------------------------------
 * ticc1200_read  -  Read a normal register
 *------------------------------------------------------------------------
 */
int32	ticc1200_read (
		       struct	radcblk *radptr,/* Contro block pointer	*/
		       byte	addr,		/* Register address	*/
		       byte	*value		/* Value pointer	*/
		      )
{
	byte	rxbuf[2];	/* Receive buffer	*/
	byte	txbuf[2];	/* Transmit buffer	*/
	struct	spi_transfer msg;
				/* SPI transfer message	*/

	txbuf[0] = (TICC1200_COM_READ | addr);
	txbuf[1] = 0;

	memset(rxbuf, 0, 2);

	msg.txbuf = txbuf;
	msg.rxbuf = rxbuf;
	msg.length = 2;

	control(radptr->spidev, SPI_CTRL_TRANSFER, (int32)&msg, 0);

	(*value) = rxbuf[1];

	return OK;
}

/*------------------------------------------------------------------------
 * ticc1200_write  -  Write a normal register
 *------------------------------------------------------------------------
 */
int32	ticc1200_write (
			struct	radcblk *radptr,/* Control block pointer*/
			byte	addr,		/* Register address	*/
			byte	value		/* Value to be written	*/
		       )
{
	byte	txbuf[2];	/* Transmit buffer	*/
	byte	rxbuf[2];	/* Receive buffer	*/
	struct	spi_transfer msg;
				/* SPI transfer message	*/

	txbuf[0] = (TICC1200_COM_WRITE | addr);
	txbuf[1] = value;

	msg.txbuf = txbuf;
	msg.rxbuf = rxbuf;
	msg.length = 2;

	control(radptr->spidev, SPI_CTRL_TRANSFER, (int32)&msg, 0);

	return OK;
}

/*------------------------------------------------------------------------
 * ticc1200_read_ext  -  Read a register from the extended space
 *------------------------------------------------------------------------
 */
int32	ticc1200_read_ext (
			   struct radcblk *radptr,/* Control block ptr	*/
			   byte	  addr,		  /* Register address	*/
			   byte	  *value	  /* Value pointer	*/
			  )
{
	byte	txbuf[3];	/* Transmit buffer	*/
	byte	rxbuf[3];	/* Receive buffer	*/
	struct	spi_transfer msg;
				/* SPI transfer message	*/

	txbuf[0] = (TICC1200_COM_EXT | TICC1200_COM_READ);
	txbuf[1] = addr;
	txbuf[2] = 0;

	memset(rxbuf, 0, 3);

	msg.txbuf = txbuf;
	msg.rxbuf = rxbuf;
	msg.length = 3;

	control(radptr->spidev, SPI_CTRL_TRANSFER, (int32)&msg, 0);

	(*value) = rxbuf[2];

	return OK;
}

/*------------------------------------------------------------------------
 * ticc1200_write_ext  -  Write a register in extended space
 *------------------------------------------------------------------------
 */
int32	ticc1200_write_ext (
			    struct radcblk *radptr,/* Control block ptr	*/
			    byte   addr,	   /* Register address	*/
			    byte   value	   /* Value		*/
			   )
{
	byte	txbuf[3];	/* Transmit buffer	*/
	byte	rxbuf[3];	/* Receive buffer	*/
	struct 	spi_transfer msg;
				/* SPI transfer message	*/

	txbuf[0] = (TICC1200_COM_EXT | TICC1200_COM_WRITE);
	txbuf[1] = addr;
	txbuf[2] = value;

	msg.txbuf = txbuf;
	msg.rxbuf = rxbuf;
	msg.length = 3;

	control(radptr->spidev, SPI_CTRL_TRANSFER, (int32)&msg, 0);

	return OK;
}
