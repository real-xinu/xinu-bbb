/* kprintf.c -  kputc, kgetc, kprintf */

#include <xinu.h>
#include <stdarg.h>

/*------------------------------------------------------------------------
 * kputc - use polled I/O to write a character to the console serial line
 *------------------------------------------------------------------------
 */
syscall kputc(
	  byte	c			/* character to write		*/
	)
{
	struct	dentry		*devptr;
	struct	uart_csreg	*csrptr;

	/* Get CSR address of the console */

	devptr = (struct dentry *) &devtab[CONSOLE];
	csrptr = (struct uart_csreg *) devptr->dvcsr;

	/* wait for UART transmit queue to empty */

	//while ( (inb( (int)&csrptr->lsr) & UART_LSR_THRE) == 0 ) {
	//	;	/* reepatedly poll the device */
	//}
	while ( (csrptr->lsr & UART_LSR_THRE) == 0 ) {
		;
	}

	/* write the character */

	//outb( (int) &csrptr->buffer, c);
	csrptr->buffer = c;

	/* Honor CRLF - when writing NEWLINE also send CARRIAGE RETURN	*/

	if (c == '\n') {
	        //while ( (inb( (int)&csrptr->lsr) & UART_LSR_THRE) == 0 ) {
		//	;	/* poll until transmitter queue to empty */
		//}
		while ( (csrptr->lsr & UART_LSR_THRE) == 0 ) {
			;
		}
		//outb( (int) &csrptr->buffer, '\r');
		csrptr->buffer = '\r';
	}
	return OK;
}

/**
 * perform a synchronous kernel read from a serial device
 * @param *devptr pointer to device on which to write character
 * @return character read on success, SYSERR on failure
 */
syscall kgetc(void)
{
    int irmask;
    volatile struct uart_csreg *regptr;
    byte c;
	struct	dentry	*devptr;

	devptr = (struct dentry *) &devtab[CONSOLE];
    regptr = (struct uart_csreg *)devptr->dvcsr;

    irmask = regptr->ier;       /* Save UART interrupt state.   */
    regptr->ier = 0;            /* Disable UART interrupts.     */

    while (0 == (regptr->lsr & UART_LSR_DR))
    {                           /* Do Nothing */
    }

    /* read character from Receive Holding Register */
    c = regptr->rbr;
    regptr->ier = irmask;       /* Restore UART interrupts.     */
    return c;
}

extern	void	_doprnt(char *, va_list, int (*)(int), int);

/**
 * kernel printf: formatted, unbuffered output to CONSOLE
 * @param *fmt pointer to string being printed
 * @return OK on success
 */
syscall kprintf(char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    _doprnt(fmt, ap, (int (*)(int))kputc, (int)&devtab[CONSOLE]);
    va_end(ap);
    return OK;
}
