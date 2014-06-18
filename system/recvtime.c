/* recvtime.c - recvtime */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  recvtime  -  wait specified time to receive a message and return
 *------------------------------------------------------------------------
 */
umsg32	recvtime(
	  int32		maxwait		/* ticks to wait before timeout */
        )
{
	intmask	mask;			/* saved interrupt mask		*/
	struct	procent	*prptr;		/* tbl entry of current process	*/
	umsg32	msg;			/* message to return		*/

	if (maxwait < 0) {
		return SYSERR;
	}
	mask = disable();

	/* Schedule wakeup and place process in timed-receive state */

	prptr = &proctab[currpid];
	if (prptr->prhasmsg == FALSE) {	/* if message waiting, no delay	*/
		if (insertd(currpid,sleepq,maxwait) == SYSERR) {
			restore(mask);
			return SYSERR;
		}
		sltop = &queuetab[firstid(sleepq)].qkey;
		slnonempty = TRUE;
		prptr->prstate = PR_RECTIM;
		resched();
	}

	/* Either message arrived or timer expired */

	if (prptr->prhasmsg) {
		msg = prptr->prmsg;	/* retrieve message		*/
		prptr->prhasmsg = FALSE;/* reset message indicator	*/
	} else {
		msg = TIMEOUT;
	}
	restore(mask);
	return msg;
}
