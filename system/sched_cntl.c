/* sched_cntl.c - sched_cntl */

#include <xinu.h>

struct	defer	Defer;

/*------------------------------------------------------------------------
 *  sched_cntl  -  control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	sched_cntl(		/* assumes interrupts are disabled	*/
	  int32	def		/* either DEFER_START or DEFER_STOP	*/
	)
{

	switch (def) {

	    /* Process request to defer:				*/
	    /*	1) Increment count of outstanding deferral requests	*/
	    /*	2) If this is the start of deferral, initialize Boolean	*/
	    /*		to indicate whether resched has been called	*/

	    case DEFER_START:
		if (Defer.ndefers++ == 0) {	/* increment deferrals	*/
			Defer.attempt = FALSE;	/* no attempts so far	*/
		}
		return OK;

	    /* Process request to stop deferring:			*/
	    /*	1) Decrement count of outstanding deferral requests	*/
	    /*	2) If last deferral ends, make up for any calls to	*/
	    /*		resched that were missed during the deferral	*/

	    case DEFER_STOP:
		if (Defer.ndefers <= 0) {	/* none outstanding */
			return SYSERR;
		}
		if (--Defer.ndefers == 0) {	/* end deferral period	*/
			if (Defer.attempt) {	/* resched was called	*/
				resched();	/*   during deferral	*/
			}
		}
		return OK;

	    default:
		return SYSERR;
	}
}
