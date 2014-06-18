/* ptsend.c - ptsend */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  ptsend  --  send a message to a port by adding it to the queue
 *------------------------------------------------------------------------
 */
syscall	ptsend(
	  int32		portid,		/* ID of port to use		*/
	  umsg32	msg		/* message to send		*/
	)
{
	intmask	mask;			/* saved interrupt mask		*/
	struct	ptentry	*ptptr;		/* pointer to table entry	*/
	int32	seq;			/* local copy of sequence num.	*/
	struct	ptnode	*msgnode;	/* allocated message node 	*/
	struct	ptnode	*tailnode;	/* last node in port or NULL	*/

	mask = disable();
	if ( isbadport(portid) ||
	     (ptptr= &porttab[portid])->ptstate != PT_ALLOC ) {
		restore(mask);
		return SYSERR;
	}

	/* Wait for space and verify port has not been reset */

	seq = ptptr->ptseq;		/* record orignal sequence	*/
	if (wait(ptptr->ptssem) == SYSERR
	    || ptptr->ptstate != PT_ALLOC
	    || ptptr->ptseq != seq) {
		restore(mask);
		return SYSERR;
	}
	if (ptfree == NULL) {
		panic("Port system ran out of message nodes");
	}

	/* Obtain node from free list by unlinking */

	msgnode = ptfree;		/* point to first free node */
	ptfree  = msgnode->ptnext;	/* unlink from the free list*/
	msgnode->ptnext = NULL;		/* set fields in the node   */
	msgnode->ptmsg  = msg;

	/* Link into queue for the specified port */

	tailnode = ptptr->pttail;
	if (tailnode == NULL) {		/* queue for port was empty */
		ptptr->pttail = ptptr->pthead = msgnode;
	} else {			/* insert new node at tail  */
		tailnode->ptnext = msgnode;
		ptptr->pttail = msgnode;
	}
	signal(ptptr->ptrsem);
	restore(mask);
	return OK;
}
