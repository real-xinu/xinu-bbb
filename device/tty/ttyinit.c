/* ttyInit.c - ttyInit */

#include <xinu.h>

struct	ttycblk	ttytab[Ntty];

/*------------------------------------------------------------------------
 *  ttyInit - initialize buffers and modes for a tty line
 *------------------------------------------------------------------------
 */
devcall	ttyinit(
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


	if(devptr->dvminor == 0) { /* CONSOLE uart0 init */

		/* Pad control register init */

		am335x_padctl(UART0_PADRX_ADDR,
				AM335X_PADCTL_RXTX | UART0_PADRX_MODE);
		am335x_padctl(UART0_PADTX_ADDR,
				AM335X_PADCTL_TX | UART0_PADTX_MODE);
	}
	if(devptr->dvminor == 1) { /* RADIOTTY uart1 init */

		volatile uint32 *clkctrl = (uint32 *)UART1_CLKCTRL_ADDR;
				/* Clock control register pointer */

		/* Enable the UART clock */

		*clkctrl = UART1_CLKCTRL_EN;

		/* Wait until the clock is enabled */

		while((*clkctrl) != 0x2);

		/* Select the proper mode for UART Tx and Rx */

		am335x_padctl(UART1_PADRX_ADDR,
				AM335X_PADCTL_RXTX | UART1_PADRX_MODE);
		am335x_padctl(UART1_PADTX_ADDR,
				AM335X_PADCTL_TX | UART1_PADTX_MODE);

		/* RADIOTTY will be run in raw mode */

		typtr->tyimode = TY_IMRAW;
	}

	/* Initialize the UART */

	uptr = (struct uart_csreg *)devptr->dvcsr;

	/* Reset the UART module */
	uptr->sysc |= UART_SYSC_SOFTRESET;
	while((uptr->syss & UART_SYSS_RESETDONE) == 0);

	/* Set baud rate */

	uptr->lcr = UART_LCR_DLAB;
	if(devptr->dvminor == 0) {

		/* Baud rate for CONSOLE is 115200 */

		uptr->dlm = 0;
		uptr->dll = 26;
	}
	else if(devptr->dvminor == 1) {

		/* Baud rate for RADIOTTY is 9600 */

		uptr->dlm = 0x1;
		uptr->dll = 0x38;
	}

	uptr->lcr = UART_LCR_8N1;	/* 8 bit char, No Parity, 1 Stop*/
	uptr->fcr = 0;			/* Disable FIFO for now		*/

	/* Set DTR and RTS for the UART */

	uptr->mcr = (UART_MCR_DTR | UART_MCR_RTS);

	/* Register the interrupt dispatcher for the tty device */

	set_evec( devptr->dvirq, (uint32)devptr->dvintr );

	/* Enable UART FIFOs, clear and set interrupt trigger level	*/

	//uptr->fcr = (UART_FCR_EFIFO | UART_FCR_TRESET | UART_FCR_RRESET);

	/* Start the UART module in 16x mode */

	uptr->mdr1 = UART_MDR1_16X;

	/* Enable interrupts for the device */

	ttykickout(uptr);

	return OK;
}
