/* platinit.c - platinit */

#include <xinu.h>

/*------------------------------------------------------------------------
 * platinit - platform specific initialization for BeagleBone Black
 *------------------------------------------------------------------------
 */
void	platinit(void)
{

	struct	uart_csreg *uptr;	/* address of UART's CSRs	*/

	/* Initialize the Interrupt Controller */

	initintc();

	/* Pad control for CONSOLE */

	am335x_padctl(UART0_PADRX_ADDR,
			AM335X_PADCTL_RXTX | UART0_PADRX_MODE);
	am335x_padctl(UART0_PADTX_ADDR,
			AM335X_PADCTL_TX | UART0_PADTX_MODE);

	/* Reset the UART device */

	uptr = (struct uart_csreg *)devtab[CONSOLE].dvcsr;
	uptr->sysc |= UART_SYSC_SOFTRESET;
	while((uptr->syss & UART_SYSS_RESETDONE) == 0);
}
