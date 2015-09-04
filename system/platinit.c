/* platinit.c - platinit */

#include <xinu.h>

/*------------------------------------------------------------------------
 * platinit - platform specific initialization for BeagleBone Black
 *------------------------------------------------------------------------
 */
void	platinit(void)
{

	struct	uart_csreg *uptr;	/* Address of UART's CSRs	*/
	struct	watchdog_csreg *wdtptr;	/* Watchdog registers		*/

	/* Disable the watchdog timer */

	wdtptr = (struct watchdog_csreg *)WDTADDR;
	wdtptr->wspr = 0x0000aaaa;
	while(wdtptr->wwps & 0x00000010);
	wdtptr->wspr = 0x00005555;
	while(wdtptr->wwps & 0x00000010);

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
