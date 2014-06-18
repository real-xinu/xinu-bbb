/* initialize.c - nulluser, sysinit, sizmem */

/* Handle system initialization and become the null process */

#include <xinu.h>
#include <string.h>

extern	void	start(void);	/* start of Xinu code */
extern	void	*_end;		/* end of Xinu code */

/* Function prototypes */

extern	void main(void);	/* main is the first process created	*/
extern	void xdone(void);	/* system "shutdown" procedure		*/
static	void sysinit(void);	/* initializes system structures	*/

/* Declarations of major kernel variables */

struct	procent	proctab[NPROC];	/* Process table			*/
struct	sentry	semtab[NSEM];	/* Semaphore table			*/
struct	memblk	memlist;	/* List of free memory blocks		*/

/* Active system status */

int	prcount;		/* Total number of live processes	*/
pid32	currpid;		/* ID of currently executing process	*/

/* Memory bounds set by start.S */

void	*minheap;		/* start of heap			*/
void	*maxheap;		/* highest valid memory address		*/

/*------------------------------------------------------------------------
 * nulluser - initialize the system and become the null process
 *
 * Note: execution begins here after the C run-time environment has been
 * established.  Interrupts are initially DISABLED, and must eventually
 * be enabled explicitly.  The code turns itself into the null process
 * after initialization.  Because it must always remain ready to execute,
 * the null process cannot execute code that might cause it to be
 * suspended, wait for a semaphore, put to sleep, or exit.  In
 * particular, the code must not perform I/O except for polled versions
 * such as kprintf.
 *------------------------------------------------------------------------
 */

void	nulluser(void)
{
	sysinit();

	kprintf("\n\r%s\n\n\r", VERSION);

	/* Output Xinu memory layout */

	kprintf("%10d bytes physical memory.\n",
		(uint32)maxheap - (uint32)0);
	kprintf("           [0x%08X to 0x%08X]\r\n",
		(uint32)0, (uint32)maxheap - 1);
	kprintf("%10d bytes of Xinu code.\n",
		(uint32)&etext - (uint32)0);
	kprintf("           [0x%08X to 0x%08X]\n",
		(uint32)0, (uint32)&etext - 1);
	kprintf("%10d bytes of data.\n",
		(uint32)&end - (uint32)&etext);
	kprintf("           [0x%08X to 0x%08X]\n",
		(uint32)&etext, (uint32)&end - 1);
	if ( (char *)minheap < HOLESTART) {
	    kprintf("%10d bytes of heap space below 640K.\n",
		(uint32)HOLESTART - (uint32)roundmb(minheap));
	}
	if ( (char *)maxheap > HOLEEND ) {
	    kprintf("%10d bytes of heap space above 1M.\n",
		(uint32)maxheap - (uint32)HOLEEND);
	    kprintf("           [0x%08X to 0x%08X]\n",
		(uint32)HOLEEND, (uint32)truncmb(maxheap) - 1);
	}

	/* Enable interrupts */
	
	enable();

	/* Start the network */
	
/*DEBUG*/ // netstart();

	/* Create a process to execute function main() */

	resume (
	   create((void *)main, INITSTK, INITPRIO, "Main process", 20, 0,
           NULL));

	/* Become the Null process (i.e., guarantee that the CPU has	*/
	/*  something to run when no other process is ready to execute)	*/

	while (TRUE) {
		;		/* do nothing */
	}
}

/*------------------------------------------------------------------------
 *
 * sysinit - intialize all Xinu data structures and devices
 *
 *------------------------------------------------------------------------
 */

static	void	sysinit(void)
{
	int32	i;
	struct	procent	*prptr;		/* ptr to process table entry	*/
	struct	sentry	*semptr;	/* prr to semaphore table entry	*/
	struct	memblk	*memptr;	/* ptr to memory block		*/

	/* Initialize the interrupt vectors */

	initevec();

	/* Initialize system variables */

	/* Count the Null process as the first process in the system */

	prcount = 1;

	/* Scheduling is not currently blocked */

	Defer.ndefers = 0;

	/* Initialize the free memory list */

	/* Note: PC version has to pre-allocate 640K-1024K "hole" */

	//maxheap = (void *)MAXADDR;
	maxheap = (void *)0x84000000;
	minheap = &end;

	memptr = memlist.mnext = (struct memblk *)roundmb(minheap);
	//if ((char *)(maxheap+1) > HOLESTART) {
		/* create two blocks that straddle the hole */
	//	memptr->mnext = (struct memblk *)HOLEEND;
	//	memptr->mlength = (int) truncmb((unsigned) HOLESTART -
	//     		 (unsigned)&end - 4);
	//	memptr = (struct memblk *) HOLEEND;
	//	memptr->mnext = (struct memblk *) NULL;
	//	memptr->mlength = (int) truncmb( (uint32)maxheap - 
	//			(uint32)HOLEEND - NULLSTK);
	//} else {
		/* initialize free memory list to one block */
		memlist.mnext = memptr = (struct memblk *) roundmb(&end);
		memptr->mnext = (struct memblk *) NULL;
		memptr->mlength = (uint32) truncmb((uint32)maxheap -
				(uint32)&end - NULLSTK);
	//}

	/* Initialize process table entries free */

	for (i = 0; i < NPROC; i++) {
		prptr = &proctab[i];
		prptr->prstate = PR_FREE;
		prptr->prname[0] = NULLCH;
		prptr->prstkbase = NULL;
		prptr->prprio = 0;
	}

	/* Initialize the Null process entry */

	prptr = &proctab[NULLPROC];
	prptr->prstate = PR_CURR;
	prptr->prprio = 0;
	strncpy(prptr->prname, "prnull", 7);
	prptr->prstkbase = getstk(NULLSTK);
	prptr->prstklen = NULLSTK;
	prptr->prstkptr = 0;
	currpid = NULLPROC;

	/* Initialize semaphores */

	for (i = 0; i < NSEM; i++) {
		semptr = &semtab[i];
		semptr->sstate = S_FREE;
		semptr->scount = 0;
		semptr->squeue = newqueue();
	}

	/* Initialize buffer pools */

	bufinit();
	/* Create a ready list for processes */

	readylist = newqueue();

	/* Initialize the PCI bus */

	//pci_init();

	/* Initialize the real time clock */

	clkinit();

	for (i = 0; i < NDEVS; i++) {
		init(i);
	}
	return;
}

#define	NBPG		4096		/* number of bytes per page	*/

/*------------------------------------------------------------------------
 * sizmem - return memory size (in pages)
 *------------------------------------------------------------------------
 */
int32	sizmem(void) {

	unsigned char	*ptr, *start, stmp, tmp;
	int32		npages;

        return 4096;	/* 16M for now */
	start = ptr = 0;
	npages = 0;
	stmp = *start;
	while (1) {
		tmp = *ptr;
		*ptr = 0xA5;
		if (*ptr != 0xA5)
			break;
		*ptr = tmp;
		++npages;
		ptr += NBPG;
		if ((uint32)ptr == (uint32)HOLESTART) {	/* skip I/O pages */
			npages += (1024-640)/4;
			ptr = (unsigned char *)HOLEEND;
		}
	}
	return npages;
}

int32	stop(char *s)
{
	kprintf("%s\n", s);
	kprintf("looping... press reset\n");
	while(1)
		/* empty */;
}

/*int32	delay(int n)
{
	DELAY(n);
	return OK;
}*/
