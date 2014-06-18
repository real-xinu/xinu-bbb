/* xsh_memstat.c - xsh_memstat */

#include <xinu.h>
#include <stdio.h>
#include <string.h>

static	void	printMemUse(void);
static	void	printFreeList(void);

/*------------------------------------------------------------------------
 * xsh_memstat - print statistics about memory use and dump the free list
 *------------------------------------------------------------------------
 */
shellcmd xsh_memstat(int nargs, char *args[])
{

	/* For argument '--help', emit help about the 'memstat' command	*/

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("use: %s \n\n", args[0]);
		printf("Description:\n");
		printf("\tDisplays the current memory use and prints the\n");
		printf("\tfree list.\n");
		printf("Options:\n");
		printf("\t--help\t\tdisplay this help and exit\n");
		return 0;
	}

	/* Check for valid number of arguments */

	if (nargs > 1) {
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
				args[0]);
		return 1;
	}

	printMemUse();
	printFreeList();

	return 0;
}


/*------------------------------------------------------------------------
 * printFreeList - walk the list of free memory blocks and print the
 *			location and size of each
 *------------------------------------------------------------------------
 */
static void printFreeList(void)
{
	struct memblk *block;

	/* Output a heading for the free list */

	printf("Free List:\n\n");
	printf("Block address  Length (dec)  Length (hex)\n");
	printf("-------------  ------------  ------------\n");
	
	for (block = memlist.mnext; block != NULL; block = block->mnext) {
		printf("  0x%08x    %9d     0x%08x\n", block,
			block->mlength, block->mlength);
	}
	printf("\n");
}

extern void start(void);
extern void *_end;

/*------------------------------------------------------------------------
 * printMemUse - print statistics about memory use
 *------------------------------------------------------------------------
 */
static void printMemUse(void)
{
	int i;				/* index into tpcoess table	*/
	uint32 phys = 0;		/* total physical memory	*/
	uint32 resrv = 0;		/* total reserved system memory	*/
	uint32 code = 0;		/* total Xinu code memory	*/
	uint32 stack = 0;		/* total stack memory		*/
	uint32 kheap = 0;		/* total kernel heap memory	*/
	uint32 kused = 0;		/* total used kernel heap memory*/
	uint32 kfree = 0;		/* total free memory		*/
	struct memblk *block;	 	/* ptr to memory block		*/

	/* Calculate the amount of physical memory */

	phys = (uint32)maxheap;

	/* Calculate amount of reserved system memory */

	resrv = (uint32)start;

	/* Calculate amount of text memory */

	code = (uint32)&_end - (uint32)start;

	/* Caculate amount of allocated stack memory */

	for (i = 0; i < NPROC; i++) {
		if (proctab[i].prstate != PR_FREE) {
			stack += (uint32)proctab[i].prstklen;
		}
	}

	/* Calculate the amount of memory on the free list */

	for (block = memlist.mnext; block != NULL; block = block->mnext) {
		kfree += block->mlength;
	}

	/* Caculate the amount of kernel heap memory */

	kheap = phys - resrv - code - stack;
	kused = kheap - kfree;

	/* Ouput statistics on current memory use */

	printf("Current system memory statistics:\n");
	printf("---------------------------------\n");
	printf("%10d bytes (0x%08x) in the system area\n",
							resrv, resrv);
	printf("%10d bytes (0x%08x) of Xinu code\n",
							code, code);
	printf("%10d bytes (0x%08x) of allocated stack space\n",
							stack, stack);
	printf("%10d bytes (0x%08x) of kernel heap space (%d used)\n",
						 kheap, kheap, kused);
	printf("%10d bytes (0x%08x) of physical memory\n\n",
							phys, phys);
}
