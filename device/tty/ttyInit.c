/* ttyInit.c - ttyInit */

#include <xinu.h>

struct	ttycblk	ttytab[Ntty];

/*------------------------------------------------------------------------
 *  ttyInit - initialize buffers and modes for a tty line
 *------------------------------------------------------------------------
 */
devcall	ttyInit(
	  struct dentry	*devptr		/* entry in device switch table	*/
	)
{
	struct	ttycblk	*typtr;		/* pointer to ttytab entry	*/
	struct	uart_csreg *uptr;	/* address of UART's CSRs	*/

	typtr = &ttytab[ devptr->dvminor ];

	/* Initialize values in the tty control block */

	typtr->tyihead = typtr->tyitail = 	/* set up input queue	*/
		&typtr->tyibuff[0];		/*    as empty		*/
	typtr->tyisem = semcreate(0);		/* input semaphore	*/
	typtr->tyohead = typtr->tyotail = 	/* set up output queue	*/
		&typtr->tyobuff[0];		/*    as empty		*/
	typtr->tyosem = semcreate(TY_OBUFLEN);	/* output semaphore	*/
	typtr->tyehead = typtr->tyetail = 	/* set up echo queue	*/
		&typtr->tyebuff[0];		/*    as empty		*/
	typtr->tyimode = TY_IMCOOKED;		/* start in cooked mode	*/
	typtr->tyiecho = TRUE;			/* echo console input	*/
	typtr->tyieback = TRUE;			/* honor erasing bksp	*/
	typtr->tyevis = TRUE;			/* visual control chars	*/
	typtr->tyecrlf = TRUE;			/* echo CRLF for NEWLINE*/
	typtr->tyicrlf = TRUE;			/* map CR to NEWLINE	*/
	typtr->tyierase = TRUE;			/* do erasing backspace	*/
	typtr->tyierasec = TY_BACKSP;		/* erase char is ^H	*/
	typtr->tyeof = TRUE;			/* honor eof on input	*/
	typtr->tyeofch = TY_EOFCH;		/* end-of-file character*/
	typtr->tyikill = TRUE;			/* allow line kill	*/
	typtr->tyikillc = TY_KILLCH;		/* set line kill to ^U	*/
	typtr->tyicursor = 0;			/* start of input line	*/
	typtr->tyoflow = TRUE;			/* handle flow control	*/
	typtr->tyoheld = FALSE;			/* output not held	*/
	typtr->tyostop = TY_STOPCH;		/* stop char is ^S	*/
	typtr->tyostart = TY_STRTCH;		/* start char is ^Q	*/
	typtr->tyocrlf = TRUE;			/* send CRLF for NEWLINE*/
	typtr->tyifullc = TY_FULLCH;		/* send ^G when buffer	*/
						/*   is full		*/

	if(devptr->dvminor == 1) {
		volatile uint32 *clkctrl = (uint32 *)UART1_CLKCTRL_ADDR;
		*clkctrl = UART1_CLKCTRL_EN;
		while((*clkctrl) != 0x2);
		*((uint32 *)UART1_PADRX_ADDR) &= UART1_PADRX_MODE;
		*((uint32 *)UART1_PADTX_ADDR) &= UART1_PADTX_MODE;
		kprintf("radio tty init csr %x\n", devptr->dvcsr);
		typtr->tyimode = TY_IMRAW;
		//return OK;
	}
	/* Initialize the UART */

	uptr = (struct uart_csreg *)devptr->dvcsr;

	/* Reset the UART module */
	uptr->sysc |= UART_SYSC_SOFTRESET;
	while((uptr->syss & UART_SYSS_RESETDONE) == 0);
	if(devptr->dvminor == 1) {
		kprintf("reset done uart1\n");
	}

	/* Set baud rate */
	//outb((int)&uptr->lcr, UART_LCR_DLAB);
	//outb((int)&uptr->dlm, 0x00);
	//outb((int)&uptr->dll, 0x0c);
	uptr->lcr = UART_LCR_DLAB;
	if(devptr->dvminor == 0) {
		uptr->dlm = 0;
		uptr->dll = 26;
	}
	else if(devptr->dvminor == 1) {
		uptr->dlm = 0x1;
		uptr->dll = 0x38;
		//return OK;
	}

	//outb((int)&uptr->lcr, UART_LCR_8N1);	/* 8 bit char, No Parity, 1 Stop*/
	//outb((int)&uptr->fcr, 0x00);		/* Disable FIFO for now		*/
	uptr->lcr = UART_LCR_8N1;
	uptr->fcr = 0;

	/* OUT2 value is used to control the onboard interrupt tri-state*/
	/* buffer. It should be set high to generate interrupts		*/
	//outb((int)&uptr->mcr, UART_MCR_DTR | 
	//		      UART_MCR_RTS | 
	//		      UART_MCR_OUT2);	/* Turn on user-defined OUT2	*/

	uptr->mcr = (UART_MCR_DTR | UART_MCR_RTS);
	/* Register the interrupt dispatcher for the tty device */

	set_evec( devptr->dvirq, (uint32)devptr->dvintr );

	/* Enable interrupts on the device */

	/* Enable UART FIFOs, clear and set interrupt trigger level	*/
	//outb((int)&uptr->fcr, UART_FCR_EFIFO | UART_FCR_RRESET |
	//		      UART_FCR_TRESET | UART_FCR_TRIG2);
	uptr->fcr = (UART_FCR_EFIFO | UART_FCR_TRESET | UART_FCR_RRESET);
	/* Start the UART module in 16x mode */
	uptr->mdr1 = UART_MDR1_16X;

	ttyKickOut(typtr, uptr);
	//(void)inb((int)&uptr->iir);
	//(void)inb((int)&uptr->lsr);
	//(void)inb((int)&uptr->msr);
	//(void)inb((int)&uptr->buffer);

	return OK;
}
