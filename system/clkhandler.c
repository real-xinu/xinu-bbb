/* clkhandler.c - clkhandler */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  clkhandler - handle clock interrupt and process preemption events
 *			as well as awakening sleeping processes
 *------------------------------------------------------------------------
 */
interrupt clkhandler(void)
{
	clkupdate(CLKCYCS_PER_TICK);

	/* record clock ticks */

	clkticks++;

	/* update global counter for seconds */

	if (clkticks == CLKTICKS_PER_SEC) {
		clktime++;
		clkticks = 0;
	}

	/* If sleep queue is nonempty, decrement first key; when the	*/
	/* key reaches zero, awaken a sleeping process			*/

	if (nonempty(sleepq) && (--firstkey(sleepq) <= 0)) {
		wakeup();
	}

	/* Check to see if this proc should be preempted */

	if (--preempt <= 0) {
		preempt = QUANTUM;
		resched();
	}
	return;
}
