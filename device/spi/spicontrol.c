/* spicontrol.c - spicontrol */

#include <xinu.h>

/*------------------------------------------------------------------------
 * spicontrol  -  Control function for SPI device
 *------------------------------------------------------------------------
 */
devcall	spicontrol (
	struct	dentry	*devptr,	/* Entry in device switch table	*/
	int32	func,			/* Control function		*/
	int32	arg1,			/* Argument 1, if needed	*/
	int32	arg2			/* Argument 2, if needed	*/
	)
{
	struct	spi_csreg *csrptr;	/* SPI control and status regs	*/
	struct	spi_transfer *msg;
	int	i;

	csrptr = (struct spi_csreg *)devptr->dvcsr;

	msg = (struct spi_transfer *)arg1;

	switch(func) {

	 case SPI_CTRL_TRANSFER:

		kprintf("spicontrol: transfer\n");
		for(i = 0; i < msg->length; i++) {

			while((csrptr->ch[0].chstat & SPI_CHSTAT_TXS) == 0);
			kprintf("writing %x\n", msg->txbuf[i]);
			csrptr->ch[0].chtx = msg->txbuf[i];
			while((csrptr->ch[0].chstat & SPI_CHSTAT_RXS) == 0);
			msg->rxbuf[i] = csrptr->ch[0].chrx;
			kprintf("read %x\n", msg->rxbuf[i]);
		}
		csrptr->ch[0].chconf &= ~SPI_CHCONF_FORCE;
		csrptr->ch[0].chconf |= SPI_CHCONF_FORCE;
		break;
	}

	return OK;
}
