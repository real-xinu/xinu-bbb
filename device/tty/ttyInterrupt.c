/* ttyInterrupt.c - ttyInterrupt */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  ttyInterrupt - handle an interrupt for a tty (serial) device
 *------------------------------------------------------------------------
 */
void ttyInterrupt(uint32 xnum) {
	struct	dentry	*devptr;	/* address of device control blk*/
	struct	ttycblk	*typtr;		/* pointer to ttytab entry	*/	
	struct	uart_csreg *csrptr;	/* address of UART's CSR	*/
	uint32	iir = 0;		/* interrupt identification	*/
	uint32	lsr = 0;		/* line status			*/


	/* Get CSR address of the device (assume console for now) */

	if(xnum == 72)
	devptr = (struct dentry *) &devtab[CONSOLE];
	else
	devptr = (struct dentry *) &devtab[RADIO];
	csrptr = (struct uart_csreg *) devptr->dvcsr;
	//kprintf("ttyint %d\n",xnum);
	/* Obtain a pointer to the tty control block */

	typtr = &ttytab[ devptr->dvminor ];
	//kprintf("ttyintr iir %x\n", csrptr->iir);
	/* Decode hardware interrupt request from UART device */

        /* Check interrupt identification register */
        iir = csrptr->iir;
	//kprintf("iir %x\n", iir);
        if (iir & UART_IIR_IRQ) {
		return;
        }

	/* Decode the interrupt cause based upon the value extracted	*/
	/* from the UART interrupt identification register.  Clear	*/
	/* the interrupt source and perform the appropriate handling	*/
	/* to coordinate with the upper half of the driver		*/

        /* Decode the interrupt cause */
	//kprintf("we have an inter\n");
        iir &= UART_IIR_IDMASK;		/* mask off the interrupt ID */
        switch (iir) {

	    /* Receiver line status interrupt (error) */

	    case UART_IIR_RLSI:
		//kprintf("line status inter, %x\n", csrptr->lsr);
		//kprintf("buffer %x\n", csrptr->buffer);
		lsr = csrptr->lsr;
		if(lsr & UART_LSR_BI) { /* Break Interrupt */
			lsr = csrptr->buffer; /* Read the RHR register to acknowledge */
		}
		return;

	    /* Receiver data available or timed out */

	    case UART_IIR_RDA:
	    case UART_IIR_RTO:

		sched_cntl(DEFER_START);

		/* While chars avail. in UART buffer, call ttyInter_in	*/
		//kprintf("calling ttyinter_in");
		//while ( (inb( (int)&csrptr->lsr) & UART_LSR_DR) != 0) {
		while ( (csrptr->lsr & UART_LSR_DR) != 0) {
			ttyInter_in(typtr, csrptr);
                }

		sched_cntl(DEFER_STOP);

		return;

            /* Transmitter output FIFO is empty (i.e., ready for more)	*/

	    case UART_IIR_THRE:
		//kprintf("calling ttyinter_out");
		/* Read from LSR to clear interrupt */
		//lsr = inb( (int)&csrptr->lsr );
		ttyInter_out(typtr, csrptr);
		return;

	    /* Modem status change (simply ignore) */

	    case UART_IIR_MSC:
		return;
	    }
}
