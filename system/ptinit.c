/* ptinit.c - ptinit */

#include <xinu.h>

struct	ptnode	*ptfree;		/* list of free message nodes	*/
struct	ptentry	porttab[NPORTS];	/* port table			*/
int32	ptnextid;			/* next table entry to try	*/

/*------------------------------------------------------------------------
 *  ptinit  --  initialize all ports
 *------------------------------------------------------------------------
 */
syscall	ptinit(
	  int32		maxmsgs		/* total messages in all ports	*/
	)
{
	int32	i;			/* runs through port table	*/
	struct	ptnode	*next, *prev;	/* used to build free list	*/

	ptfree = (struct ptnode *)getmem(maxmsgs*sizeof(struct ptnode));
	if (ptfree == (struct ptnode *)SYSERR) {
		panic("pinit - insufficient memory");
	}

	/* Initialize all port table entries to free */

	for (i=0 ; i<NPORTS ; i++) {
		porttab[i].ptstate = PT_FREE;
		porttab[i].ptseq = 0;
	}
	ptnextid = 0;

	/* Create free list of message pointer nodes */

	for ( prev=next=ptfree ;  --maxmsgs > 0  ; prev=next )
		prev->ptnext = ++next;
	prev->ptnext = NULL;
	return(OK);
}
