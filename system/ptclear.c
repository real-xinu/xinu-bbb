/* ptclear.c - _ptclear */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  _ptclear  --  used by ptdelete and ptreset to clear or reset a port
 *			(internal function assumes interrupts disabled
 *			 and arguments have been checked for validity)
 *------------------------------------------------------------------------
 */
void	_ptclear(
	  struct ptentry *ptptr,	/* table entry to clear		*/
	  uint16       newstate,	/* new state for port		*/
	  int32        (*dispose)(int32)/* disposal function to call	*/
	 )
{
	struct	ptnode	*walk;		/* pointer to walk message list	*/

	/* Place port in limbo state while waiting processes are freed */

	ptptr->ptstate = PT_LIMBO;

	ptptr->ptseq++;			/* reset accession number	*/
	walk = ptptr->pthead;		/* first item on msg list	*/

	if ( walk != NULL ) {		/* if message list nonempty	*/

		/* Walk message list and dispose of each message */

		for( ; walk!=NULL ; walk=walk->ptnext) {
                        (*dispose)( walk->ptmsg );
		}

		/* Link entire message list into the free list */

                (ptptr->pttail)->ptnext = ptfree;
                ptfree = ptptr->pthead;
        }

	if (newstate == PT_ALLOC) {
		ptptr->pttail = ptptr->pthead = NULL;
		semreset(ptptr->ptssem, ptptr->ptmaxcnt);
		semreset(ptptr->ptrsem, 0);
	} else {
		semdelete(ptptr->ptssem);
		semdelete(ptptr->ptrsem);
	}
	ptptr->ptstate = newstate;
	return;
}
