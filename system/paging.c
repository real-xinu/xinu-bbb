/* paging.c - init_paging */

#include <xinu.h>

/*------------------------------------------------------------------------
 * init_paging  -  Initialize Paging
 *------------------------------------------------------------------------
 */
status	init_paging (void) {

	int32	i;	/* Loop counter			*/
	uint32	*pd;	/* Page directory pointer	*/

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
	/* TEX[2:0] = 0b101 B = 1, C = 1 for write back	*/
	/*	write alloc. outer & inner caches	*/
	/* NS = 1 for non-shareable mem.		*/

	for(i = 2048; i < 4096; i++) {

		pd[i] = ( PDE_PTSEC	|
			  PDE_B		|
			  PDE_AP3	|
			  PDE_TEX5	|
			  PDE_NS	|
			  (i << 20) );
	}

	asm volatile (

		/* Set PD0=1 and N=0 in TTBCR	*/
		"ldr	r0, =0x00000020\n"
		"mcr	p15, 0, r0, c2, c0, 2\n"
		"isb\n"

		/* Set the PD base address in TTBR0	*/
		"orr	%0, #2\n"
		"mcr	p15, 0, %0, c2, c0, 0\n"
		"isb\n"

		: /* No output	*/
		: "r" (&page_dir)
		: "r0"
		);

	return OK;
}
