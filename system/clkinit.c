/* clkinit.c - clkinit, clkint */

#include <xinu.h>

uint32	clktime;		/* seconds since boot			*/
uint32	ctr1000 = 0;		/* milliseconds since boot		*/
qid16	sleepq;			/* queue of sleeping processes		*/
int32	slnempty;		/* zero if the sleep queue is empty;	*/
				/*   non-zero otherwise			*/
int32	*sltop;			/* ptr to key in first entry of sleepq	*/
				/*   if sleepq is not empty		*/
uint32	preempt;		/* preemption counter			*/
   
/*------------------------------------------------------------------------
 * clkinit - initialize the clock and sleep queue at startup
 *------------------------------------------------------------------------
 */
void	clkinit(void)
{
	volatile struct am335x_timer1ms *csrptr = 0x44E31000;
			/* Pointer to timer CSR in BBoneBlack	*/
	volatile uint32 *clkctrl = AM335X_TIMER1MS_CLKCTRL_ADDR;

	*clkctrl = AM335X_TIMER1MS_CLKCTRL_EN;
	while((*clkctrl) != 0x2);

	/* Reset the timer module */

	csrptr->tiocp_cfg |= AM335X_TIMER1MS_TIOCP_CFG_SOFTRESET;

	/* Wait until the reset os complete */

	while((csrptr->tistat & AM335X_TIMER1MS_TISTAT_RESETDONE) == 0);

	/* Set interrupt vector for clock to invoke clkint */

	set_evec(67, (uint32)clkint);

	sleepq = newqueue();	/* allocate a queue to hold the delta	*/
				/* list of sleeping processes		*/
	preempt = QUANTUM;	/* initial time quantum			*/

	/* Specify that seepq is initially empty */

	slnonempty = FALSE;

	clktime = 0;		/* start counting seconds		*/

	/* The following values are calculated for a timer */
	/* that generates 1ms tick			   */
	csrptr->tpir = 1000000;
	csrptr->tnir = 0;
	csrptr->tldr = 0xFFFFFFFF - 26000;

	/* Set the timer to auto reload */

	csrptr->tclr = AM335X_TIMER1MS_TCLR_AR;

	/* Start the timer */

	csrptr->tclr |= AM335X_TIMER1MS_TCLR_ST;

	/* Enable overflow interrupt which will generate */
	/* an interrupt every 1 ms			 */

	csrptr->tier = AM335X_TIMER1MS_TIER_OVF_IT_ENA;

	/* Kickstart the timer */

	csrptr->ttgr = 1;

	return;
}

/*-----------------------------------------------------------------------
 * clkint - clock interrupt handler
 *-----------------------------------------------------------------------
 */
void clkint()
{

	static uint32 count1000 = 1000;	/* variable to count 1000ms */
	volatile struct am335x_timer1ms *csrptr = 0x44E31000;
					/* Pointer to timer CSR	    */

	/* If there is no interrupt, return */

	if((csrptr->tisr & AM335X_TIMER1MS_TISR_OVF_IT_FLAG) == 0) {
		return;
	}

	/* Acknowledge the interrupt */

	csrptr->tisr = AM335X_TIMER1MS_TISR_OVF_IT_FLAG;

	/* Decrement 1000ms counter */

	count1000--;

	/* After 1 sec, increment clktime */

	if(count1000 == 0) {
		clktime++;
		count1000 = 1000;
	}

	/* check if sleep queue is empty */

	if(slnonempty) {

		/* sleepq nonempty, decrement the key of */
		/* topmost process on sleepq		 */

		(*sltop)--;

		/* If key of topmost process is zero,	*/
		/* wakeup the process			*/

		if((*sltop) == 0) {
			wakeup();
		}
	}

	/* Decrement the preemption counter */

	preempt--;

	/* if counter zero, call resched */

	if(preempt == 0) {
		resched();
	}
}
