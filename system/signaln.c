/* signaln.c - signaln */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  signaln  -  Signal a semaphore n times, releasing n waiting processes
 *------------------------------------------------------------------------
 */
syscall	signaln(
	  sid32		sem,		/* ID of semaphore to signal	*/
	  int32		count		/* number of times to signal	*/
	)
{
	intmask	mask;			/* saved interrupt mask		*/
	struct	sentry	*semptr;	/* ptr to sempahore table entry */

	mask = disable();
	if (isbadsem(sem) || (count < 0)) {
		restore(mask);
		return SYSERR;
	}
	semptr = &semtab[sem];
	if (semptr->sstate == S_FREE) {
		restore(mask);
		return SYSERR;
	}

	for (; count > 0; count--) {
		if ((semptr->scount++) < 0) {
			ready(dequeue(semptr->squeue), RESCHED_NO);
		}
	}
	resched();
	restore(mask);
	return OK;
}
