/* semdelete.c - semdelete */

#include <xinu.h>

/*------------------------------------------------------------------------
 * semdelete  --  Delete a semaphore by releasing its table entry
 *------------------------------------------------------------------------
 */
syscall	semdelete(
	  sid32		sem		/* ID of semaphore to delete	*/
	)
{
	intmask mask;			/* saved interrupt mask		*/
	struct	sentry *semptr;		/* ptr to semaphore table entry	*/

	mask = disable();
	if (isbadsem(sem)) {
		restore(mask);
		return SYSERR;
	}
	
	semptr = &semtab[sem];
	if (semptr->sstate == S_FREE) {
		restore(mask);
		return SYSERR;
	}
	semptr->sstate = S_FREE;

	while (semptr->scount++ < 0) {	/* free all waiting processes	*/
		ready(getfirst(semptr->squeue), RESCHED_NO);
	}
	resched();
	restore(mask);
	return OK;
}
