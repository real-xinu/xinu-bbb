/* semaphore.h - isbadsem */

#ifndef	NSEM
#define	NSEM		120	/* number of semaphores, if not defined	*/
#endif

/* Semaphore state definitions */

#define	S_FREE	0		/* semaphore table entry is available	*/
#define	S_USED	1		/* semaphore table entry is in use	*/

/* Semaphore table entry */
struct	sentry	{
	byte	sstate;		/* whether entry is S_FREE or S_USED	*/
	int32	scount;		/* count for the semaphore		*/
	qid16	squeue;		/* queue of processes that are waiting	*/
				/*     on the semaphore			*/
};

extern	struct	sentry semtab[];

#define	isbadsem(s)	((int32)(s) < 0 || (s) >= NSEM)
