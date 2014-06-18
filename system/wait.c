/* wait.c - wait */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  wait  -  Cause current process to wait on a semaphore
 *------------------------------------------------------------------------
 */
syscall	wait(
	  sid32		sem		/* semaphore on which to wait  */
	)
{
	intmask mask;			/* saved interrupt mask		*/
	struct	procent *prptr;		/* ptr to process' table entry	*/
	struct	sentry *semptr;		/* ptr to sempahore table entry	*/

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

	if (--(semptr->scount) < 0) {		/* if caller must block	*/
		prptr = &proctab[currpid];
		prptr->prstate = PR_WAIT;	/* set state to waiting	*/
		prptr->prsem = sem;		/* record semaphore ID	*/
		enqueue(currpid,semptr->squeue);/* enqueue on semaphore	*/
		resched();			/*   and reschedule	*/
	}

	restore(mask);
	return OK;
}
