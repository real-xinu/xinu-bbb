/* ptcreate.c - ptcreate */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  ptcreate  --  create a port that allows "count" outstanding messages
 *------------------------------------------------------------------------
 */
syscall	ptcreate(
	  int32		count
	)
{
	intmask	mask;			/* saved interrupt mask		*/
	int32	i;			/* counts all possible ports	*/
	int32	ptnum;			/* candidate port number to try	*/
	struct	ptentry	*ptptr;		/* pointer to port table entry	*/

	mask = disable();
	if (count < 0) {
		restore(mask);
		return(SYSERR);
	}

	for (i=0 ; i<NPORTS ; i++) {	/* count all table entries	*/
		ptnum = ptnextid;	/* get an entry to check	*/
		if (++ptnextid >= NPORTS) {
			ptnextid = 0;	/* reset for next iteration	*/
		}

		/* Check table entry that corresponds to ID ptnum */

		ptptr= &porttab[ptnum];
		if (ptptr->ptstate == PT_FREE) {
			ptptr->ptstate = PT_ALLOC;
			ptptr->ptssem = semcreate(count);
			ptptr->ptrsem = semcreate(0);
			ptptr->pthead = ptptr->pttail = NULL;
			ptptr->ptseq++;
			ptptr->ptmaxcnt = count;
			restore(mask);
			return(ptnum);
		}
	}
	restore(mask);
	return(SYSERR);
}
