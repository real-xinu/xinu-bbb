/* wakeup.c - wakeup */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  wakeup  -  Called by clock interrupt handler to awaken processes
 *------------------------------------------------------------------------
 */
void	wakeup(void)
{
	/* Awaken all processes that have no more time to sleep */

	while (nonempty(sleepq) && (firstkey(sleepq) <= 0)) {
		ready(dequeue(sleepq), RESCHED_NO);
	}
	
	if ( (slnonempty = nonempty(sleepq)) == TRUE ) {
		sltop = &firstkey(sleepq);
	}
	resched();
	return;
}

