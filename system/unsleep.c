/* unsleep.c - unsleep */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  unsleep  -  Internal function to remove a process from the sleep
 *			queue prematurely by adjusting the delay of
 *			successive processes.  The caller must move the
 *			process a new state and call resched, if needed.
 *------------------------------------------------------------------------
 */
status	unsleep(
	  pid32		pid		/* ID of process to remove	*/
        )
{
	intmask	mask;			/* saved interrupt mask		*/
        struct	procent	*prptr;		/* ptr to process' table entry	*/

        pid32	pidnext;		/* ID of process on sleep queue	*/
					/* that follows the process that*/
					/* is being removed		*/

	mask = disable();

	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	/* Verify that candidate process is on the sleep queue */

	prptr = &proctab[pid];
	if ((prptr->prstate!=PR_SLEEP) && (prptr->prstate!=PR_RECTIM)) {
		restore(mask);
		return SYSERR;
	}

	/* Increment delay of next process if such a process exists */

	pidnext = queuetab[pid].qnext;
	if (pidnext < NPROC) {
		queuetab[pidnext].qkey += queuetab[pid].qkey;
	}

	if ( nonempty(sleepq) ) {
		sltop = &queuetab[firstid(sleepq)].qkey;
		slnonempty = TRUE;
	} else {
		slnonempty = FALSE;
	}
	getitem(pid);			/* unlink process from queue */
	restore(mask);
	return OK;
}
