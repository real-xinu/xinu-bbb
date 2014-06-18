/* insertd.c - insertd */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  insertd  -  Insert a process in delta list using delay as the key
 *------------------------------------------------------------------------
 */
status	insertd(			/* assumes interrupts disabled	*/
	  pid32		pid,		/* ID of process to insert	*/
	  qid16		q,		/* ID of queue to use		*/
	  int32		key		/* delay from "now" (in ms.)	*/
	)
{
	int	next;			/* runs through the delta list	*/
	int	prev;			/* follows next through the list*/

	if (isbadqid(q) || isbadpid(pid)) {
		return SYSERR;
	}

	prev = queuehead(q);
	next = queuetab[queuehead(q)].qnext;
	while ((next != queuetail(q)) && (queuetab[next].qkey <= key)) {
		key -= queuetab[next].qkey;
		prev = next;
		next = queuetab[next].qnext;
	}

	/* Insert new node between prev and next nodes */

	queuetab[pid].qnext = next;
	queuetab[pid].qprev = prev;
	queuetab[pid].qkey = key;
	queuetab[prev].qnext = pid;
	queuetab[next].qprev = pid;
	if (next != queuetail(q)) {
		queuetab[next].qkey -= key;
	}

	return OK;
}
