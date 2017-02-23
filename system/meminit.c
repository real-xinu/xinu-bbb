/* meminit.c - meminit */

#include <xinu.h>

void	*minheap;	/* Start address of heap	*/
void	*maxheap;	/* End address of heap		*/

/*------------------------------------------------------------------------
 * meminit - Initialize the free memory list for BeagleBone Black
 *------------------------------------------------------------------------
 */
void	meminit(void)
{
	struct	memblk *memptr;	/* Memory block pointer	*/
	byte	*ucstart;	/* Start of uncached mem*/
	byte	*ucend;		/* End of uncached mem	*/

	/* Initialize the minheap and maxheap variables */

	minheap = (void *)&end;
	maxheap = (void *)MAXADDR;

	/* Initialize ucstart to first byte after minheap which is a	*/
	/* mutiple of page size: 1 M					*/

	ucstart = (byte *)minheap;

	ucstart = (byte *)((uint32)ucstart & 0xFFF00000);
	ucstart = (byte *)((uint32)ucstart + 0x00100000);

	/* Initialize ucend acording to no. of pages for uncached mem	*/

	ucend   = (byte *)ucstart + (UCMEM_NPAGES * 0x100000);

	if( (ucstart > (byte *)maxheap) || 
	    (ucend > (byte *)maxheap) ||
	    ( (ucstart == (byte *)minheap) && (ucend == (byte *)maxheap) )
	  ) { /* We cannot accomodate uncached memory		*/

		/* Free list will be a single block of memory */

		memlist.mnext = (struct memblk *)minheap;
		memptr = memlist.mnext;

		memptr->mnext = (struct memblk *)NULL;

		memlist.mlength = memptr->mlength =
			(uint32)maxheap - (uint32)minheap;

		/* Uncached memory list will be empty */

		ucmemlist.mnext = (struct memblk *)NULL;
		ucmemlist.mlength = 0;
	}
	else { /* We can accomodate uncached memory */

		if(ucstart == (byte *)minheap) { /* Uncached mem is at 	*/
						 /* the start of heap	*/

			/* Normal memory freelist begins*/
			/* where uncached memory ends	*/

			memlist.mnext = (struct memblk *)ucend;
			memptr = memlist.mnext;

			memptr->mnext = (struct memblk *)NULL;

			memlist.mlength = memptr->mlength =
				(uint32)maxheap - (uint32)ucend;
		}
		else { /* Uncached mem is not at the start of heap */

			/* Divide the normal memory into two blocks */

			memlist.mnext = (struct memblk *)minheap;
			memptr = memlist.mnext;

			memptr->mnext = (struct memblk *)ucend;

			memptr->mlength = memlist.mlength =
				(uint32)ucstart - (uint32)minheap;

			if(ucend != (byte *)maxheap) {
				/* Uncached mem is not at end of heap */

				memptr = memptr->mnext;
				memptr->mnext = (struct memblk *)NULL;

				memptr->mlength = 
					(uint32)maxheap - (uint32)ucend;
				memlist.mlength += memptr->mlength;
			}
		}

		/* Now, initialize the uncached memory free list */

		ucmemlist.mnext = (struct memblk *)ucstart;

		memptr = ucmemlist.mnext;

		memptr->mnext = (struct memblk *)NULL;
		memptr->mlength = ucmemlist.mlength =
			(uint32)ucend - (uint32)ucstart;
	}

	/* Disable all caches */

	cache_disable_all();

	/* Invalidate all caches */

	cache_inv_all();

	/* Invalidate the TLB */

	tlb_inv_all();

	/* Initialize page tables */

	paging_init();

	/* Make sure all memory operations are completed */

	asm volatile (
		"dsb\n"
		"dmb\n"
		);

	/* Turn on the MMU */

	mmu_enable();

	/* Enable caches  */

	cache_enable_all();
}
