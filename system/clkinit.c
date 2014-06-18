/* clkinit.c - clkinit */

#include <xinu.h>
//#include <interrupt.h>
//#include <clock.h>

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
	uint16	intv;			/* clock rate in KHz		*/
	volatile struct am335x_timer1ms *csrptr = 0x44E31000;
	volatile uint32 *clkctrl = AM335X_TIMER1MS_CLKCTRL_ADDR;

	*clkctrl = AM335X_TIMER1MS_CLKCTRL_EN;
	while((*clkctrl) != 0x2);

	//kprintf("timer1ms reset...\n");
	csrptr->tiocp_cfg |= AM335X_TIMER1MS_TIOCP_CFG_SOFTRESET;
	while((csrptr->tistat & AM335X_TIMER1MS_TISTAT_RESETDONE) == 0);
	//kprintf("timer1ms reset done\n");
	/* Set interrupt vector for clock to invoke clkint */

	set_evec(67, (uint32)clkint);

	/* clock rate is 1.190 Mhz; this is 1 ms interrupt rate */

	//intv = 1193;	/* using 1193 instead of 1190 to fix clock skew	*/

	sleepq = newqueue();	/* allocate a queue to hold the delta	*/
				/* list of sleeping processes		*/
	preempt = QUANTUM;	/* initial time quantum			*/

	/* Specify that seepq is initially empty */

	slnonempty = FALSE;

	clktime = 0;		/* start counting seconds		*/

	csrptr->tpir = 1000000;
	csrptr->tnir = 0;
	csrptr->tldr = 0xFFFFFFFF - 26000;

	csrptr->tclr = AM335X_TIMER1MS_TCLR_AR;
	csrptr->tclr |= AM335X_TIMER1MS_TCLR_ST;
	csrptr->tier = AM335X_TIMER1MS_TIER_OVF_IT_ENA;

	csrptr->ttgr = 1;
	/*  set to: timer 0, 16-bit counter, rate generator mode,
		counter is binary */
	//outb(CLKCNTL, 0x34);

	/* must write LSB first, then MSB */
	//outb(CLOCK0, (char) (0xff & intv) );
	//outb(CLOCK0, (char) (0xff & (intv>>8)));
	return;
}

void clkint() {

	static uint32 count1000 = 1000;
	volatile struct am335x_timer1ms *csrptr = 0x44E31000;

	//kprintf("clkint\n");
	if((csrptr->tisr & AM335X_TIMER1MS_TISR_OVF_IT_FLAG) == 0) {
		return;
	}
	csrptr->tisr = AM335X_TIMER1MS_TISR_OVF_IT_FLAG;

	count1000--;
	if(count1000 == 0) {
		clktime++;
		//kprintf("clktime %d\n", clktime);
		count1000 = 1000;
	}

	if(slnonempty) {

		(*sltop)--;
		if((*sltop) == 0) {
			wakeup();
		}
	}

	preempt--;
	if(preempt == 0) {
		resched();
	}
}
