/* xdone.c - xdone */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  xdone  -  Print system completion message as last thread exits
 *------------------------------------------------------------------------
 */
void	xdone(void)
{
	kprintf("\r\n\r\nAll user processes have completed.\r\n\r\n");
	gpioLEDOff(GPIO_LED_CISCOWHT);	/* turn off LED "run" light	*/
	halt();				/* halt the processor		*/
}
