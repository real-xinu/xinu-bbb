/* ptrecv.c - ptrecv */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  ptrecv  --  receive a message from a port, blocking if port empty
 *------------------------------------------------------------------------
 */
uint32	ptrecv(
	  int32		portid		/* ID of port to use		*/
	)
{
	intmask	mask;			/* saved interrupt mask		*/
	struct	ptentry	*ptptr;		/* pointer to table entry	*/
	int32	seq;			/* local copy of sequence num.	*/
	umsg32	msg;			/* message to return		*/
	struct	ptnode	*msgnode;	/* first node on message list	*/

	mask = disable();
	if ( isbadport(portid) ||
	     (ptptr= &porttab[portid])->ptstate != PT_ALLOC ) {
		restore(mask);
		return (uint32)SYSERR;
	}

	/* Wait for message and verify that the port is still allocated */

	seq = ptptr->ptseq;		/* record orignal sequence	*/
	if (wait(ptptr->ptrsem) == SYSERR || ptptr->ptstate != PT_ALLOC
	    || ptptr->ptseq != seq) {
		restore(mask);
		return (uint32)SYSERR;
	}

	/* Dequeue first message that is waiting in the port */

	msgnode = ptptr->pthead;
	msg = msgnode->ptmsg;
	if (ptptr->pthead == ptptr->pttail)	/* delete last item	*/
		ptptr->pthead = ptptr->pttail = NULL;
	else
		ptptr->pthead = msgnode->ptnext;
	msgnode->ptnext = ptfree;		/* return to free list	*/
	ptfree = msgnode;
	signal(ptptr->ptssem);
	restore(mask);
	return msg;
}
