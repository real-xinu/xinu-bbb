/* 3c905C_intr.c - _3c905C_intr */

#include <xinu.h>

/*------------------------------------------------------------------------
 * _3c905C_rxPackets - handler for receiver interrupts
 *------------------------------------------------------------------------
 */
void	_3c905C_rxPackets(
	struct	ether *ethptr		/* ptr to control block		*/
	)
{
	struct	_3c905C_desc *descptr;	/* ptr to ring descriptor	*/
	uint32	tail;			/* pos to insert next packet	*/
	uint32	status;			/* status of item in ring 	*/
	int	numdesc;		/* num. of descriptor reclaimed	*/
	//struct 	_3c905C_desc *ringptr;

	//kprintf("\n");
	//ringptr = (struct _3c905C_desc *)ethptr->rxRing;
	//for (numdesc = 0; numdesc < ethptr->rxRingSize; numdesc++) {
	//	kprintf("_3c905C_rxPackets: desc %2d; addr %08x; next %08x, buff %08x\n", 
	//			numdesc + 1, ringptr, 
	//			ringptr->next, ringptr->buffer_addr);
	//	ringptr++;
	//}

	for (numdesc = 0; numdesc < 10; numdesc++) {

		/* Insert new arrived packet to the tail */

		tail = ethptr->rxTail;
		descptr = (struct _3c905C_desc *)ethptr->rxRing + tail;
		status = descptr->status;
		

		if (status & _3C905C_DESC_RXDERROR) {
			unsigned char rx_error = status >> 16;

	//		kprintf("_3c905C_rxPackets: desc-status: %x, error_status: %08x\n", status, rx_error);
			descptr->status = 0;
			descptr->length = _3C905C_FLAG_LASTFRAG | ETH_BUF_SIZE;

		}


		if ((status & _3C905C_DESC_RXDCOMPLETE) == 0 ) {
			break;
		}

	//	kprintf("_3c905C_rxPackets: rxStatus: %x\n", inw(ethptr->iobase + 0x18));
		ethptr->rxTail
			= (ethptr->rxTail + 1) % ethptr->rxRingSize;
	}
	
	_3c905C_cmd(ethptr, _3C905C_CMD_UPUNSTALL, 0);

	//kprintf("_3c905C_rxPackets: numdesc: %d\n", numdesc);
	signaln(ethptr->isem, numdesc);

	return;
}

/*------------------------------------------------------------------------
 * _3c905C_txPackets - handler for transmitter interrupts
 *------------------------------------------------------------------------
 */
void	_3c905C_txPackets(
	struct	ether *ethptr		/* ptr to control block		*/
	)
{
	struct 	_3c905C_desc *descptr; 	/* ptr to ring descriptor 	*/
	uint32 	head; 			/* pos to reclaim descriptor 	*/
	char 	*pktptr; 		/* ptr used during packet copy 	*/
	int 	numdesc; 		/* num. of descriptor reclaimed */
	uint8 	txstatus;

	for (numdesc = 0; numdesc < ethptr->txRingSize; numdesc++) {
		head = ethptr->txHead;
		descptr = (struct _3c905C_desc *)ethptr->txRing + head;

		if (!(descptr->status & _3C905C_FLAG_DOWNCOMPLETE))
			break;

		descptr->next = 0;
		descptr->status = 0;
		descptr->length = 0;
		
		pktptr = (char *)descptr->buffer_addr;
		memset(pktptr, '\0', ETH_BUF_SIZE);

		ethptr->txHead
			= (ethptr->txHead + 1) % ethptr->txRingSize;

		//kprintf("_3c905C_txPackets: txHead: %d, txTail: %d\n", 
		//	ethptr->txHead, ethptr->txTail);
	}
	
	txstatus = inb(ethptr->iobase + 0x1b);
	//kprintf("_3c905C_txPackets: tx_status: %x\n\n", txstatus);
	if (txstatus & 0x88) { 	/* transmitter encountered 16 collisions */
		_3c905C_cmd(ethptr, _3C905C_CMD_TXRESET, 0x0000);
		_3c905C_cmd(ethptr, _3C905C_CMD_TXENABLE, 0x0000);
	}

	signaln(ethptr->osem, numdesc);

	return;
}

/*------------------------------------------------------------------------
 * ethInterrupt - decode and handle interrupt from an Ethernet device
 *------------------------------------------------------------------------
 */
interrupt _3c905C_intr(
	struct 	ether *ethptr)
{
	uint32	status;

	/* Disable device interrupt */
	_3c905C_cmd(ethptr, _3C905C_CMD_SETINTRENB, 0);

	/* Obtain status bits from device */

	status = inw(ethptr->iobase + _3C905C_REG_STATUS);

	//kprintf("_3c905C_intr: status: %x, ", status);

	/* Not our interrupt */

	if ((status & _3C905C_STATUS_INTLATCH) == 0) {
		_3c905C_cmd(ethptr, _3C905C_CMD_SETINTRENB,
				_3C905C_IRQ_MASK);
		return;
	}

	sched_cntl(DEFER_START);

	/* Handle receiver interrupts */

	if (status & _3C905C_STATUS_UPCOMPLETE) {
		//kprintf("receiver interrupt");
		_3c905C_cmd(ethptr, _3C905C_CMD_ACKINTR,
				_3C905C_STATUS_UPCOMPLETE);
		_3c905C_rxPackets(ethptr);
	}

	/* Handle transmitter interrupts */

	if (status & _3C905C_STATUS_DOWNCOMPLETE) {
		//kprintf("transmitter interrupt");
		_3c905C_cmd(ethptr, _3C905C_CMD_ACKINTR,
				_3C905C_STATUS_DOWNCOMPLETE);
		_3c905C_txPackets(ethptr);
	}

	//kprintf("\n");

	/* Acknowledge the interrupt */

	_3c905C_cmd(ethptr, _3C905C_CMD_ACKINTR,
			_3C905C_STATUS_INTLATCH);

	/* Enable device interrupt */

	_3c905C_cmd(ethptr, _3C905C_CMD_SETINTRENB,
			_3C905C_STATUS_INTLATCH | 
			_3C905C_STATUS_DOWNCOMPLETE |
			_3C905C_STATUS_UPCOMPLETE);


	sched_cntl(DEFER_STOP);

	return;
}
