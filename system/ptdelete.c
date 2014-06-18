/* ptdelete.c - ptdelete */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  ptdelete  --  delete a port, freeing waiting processes and messages
 *------------------------------------------------------------------------
 */
syscall	ptdelete(
	  int32		portid,		/* ID of port to delete		*/
	  int32		(*dispose)(int32)/* function to call to dispose	*/
	)				/*   of waiting messages	*/
{
	intmask	mask;			/* saved interrupt mask		*/
	struct	ptentry	*ptptr;		/* pointer to port table entry	*/

	mask = disable();
	if ( isbadport(portid) ||
	     (ptptr= &porttab[portid])->ptstate != PT_ALLOC ) {
		restore(mask);
		return(SYSERR);
	}
	_ptclear(ptptr, PT_FREE, dispose);
	ptnextid = portid;
	restore(mask);
	return(OK);
}
