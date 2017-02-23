/* paging.c - paging_init */

#include <xinu.h>

/*------------------------------------------------------------------------
 * paging_init  -  Initialize Page Tables
 *------------------------------------------------------------------------
 */
status	paging_init (void) {

	int32	i;	/* Loop counter			*/
	uint32	*pd;	/* Page directory pointer	*/
	int32	ucpage;	/* Starting page of uncached mem*/

	pd = (uint32 *)&page_dir;

	/* First, initialize PDEs for device memory	*/
	/* AP[2:0] = 0b011 for full access		*/
	/* TEX[2:0] = 0b000 and B = 1, C = 0 for	*/
	/*		shareable dev. mem.		*/
	/* S = 1 for shareable				*/
	/* NS = 1 for non-secure			*/

	for(i = 0; i < 2048; i++) {

		pd[i] = ( PDE_PTSEC	|
			  PDE_B		|
			  PDE_AP3	|
			  PDE_TEX0	|
			  PDE_S		|
			  PDE_NS	|
			  (i << 20) );
	}

	/* Now, initialize normal memory		*/
	/* AP[2:0] = 0b011 for full access		*/
	/* TEX[2:0] = 0b101 B = 1, C = 0 for write back	*/
	/*	write alloc. outer & inner caches	*/
	/* NS = 1 for non-secure mem.		*/

	for(i = 2048; i < 4096; i++) {

		pd[i] = ( PDE_PTSEC	|
			  PDE_B		|
			  PDE_AP3	|
			  PDE_TEX5	|
			  PDE_NS	|
			  (i << 20) );
	}

	/* Initialize uncached memory block		*/
	/* TEX[2:0] = 0b100, B = 0, C = 0		*/

	if(ucmemlist.mlength > 0) {

		ucpage = ((uint32)ucmemlist.mnext) >> 20;

		for(i = 0; i < UCMEM_NPAGES; i++) {

			pd[ucpage+i] &= ~(PDE_TEX | PDE_B | PDE_C);
			pd[ucpage+i] |= PDE_TEX4 | PDE_S;
		}
	}

	/* Set the Translation Table Base Address Register */

	mmu_set_ttbr(&page_dir);

	return OK;
}
