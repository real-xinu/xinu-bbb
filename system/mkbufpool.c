/* mkbufpool.c - mkbufpool */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  mkbufpool  --  allocate memory for a buffer pool and link the buffers
 *------------------------------------------------------------------------
 */
bpid32	mkbufpool(
	  int32		bufsiz,		/* size of a buffer in the pool	*/
	  int32		numbufs		/* number of buffers in the pool*/
	)
{
	intmask	mask;			/* saved interrupt mask		*/
	bpid32	poolid;			/* ID of pool that is created	*/
	struct	bpentry	*bpptr;		/* pointer to entry in buftab	*/
	char	*buf;			/* pointer to memory for buffer	*/

	mask = disable();
	if (bufsiz<BP_MINB || bufsiz>BP_MAXB
	    || numbufs<1 || numbufs>BP_MAXN
	    || nbpools >= NBPOOLS) {
		restore(mask);
		return (bpid32)SYSERR;
	}
	/* Round request to a multiple of 4 bytes */

	bufsiz = ( (bufsiz + 3) & (~3) );

	buf = (char *)getmem( numbufs * (bufsiz+sizeof(bpid32)) );
	if ((int32)buf == SYSERR) {
		restore(mask);
		return (bpid32)SYSERR;
	}
	poolid = nbpools++;
	bpptr = &buftab[poolid];
	bpptr->bpnext = (struct bpentry *)buf;
	bpptr->bpsize = bufsiz;
	if ( (bpptr->bpsem = semcreate(numbufs)) == SYSERR) {
		nbpools--;
		restore(mask);
		return (bpid32)SYSERR;
	}
	bufsiz+=sizeof(bpid32);
	for (numbufs-- ; numbufs>0 ; numbufs-- ) {
		bpptr = (struct bpentry *)buf;
		buf += bufsiz;
		bpptr->bpnext = (struct bpentry *)buf;
	}
	bpptr = (struct bpentry *)buf;
	bpptr->bpnext = (struct bpentry *)NULL;
	restore(mask);
	return poolid;
}
