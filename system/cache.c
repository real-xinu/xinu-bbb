/* cache.c - cache_enable_all, cache_disable_all, cache_inv_all	 */
/*         - tlb_inv_all, cache_get_info			 */

#include <xinu.h>

/*------------------------------------------------------------------------
 * cache_enable_all  -  Enable caches at all levels
 *------------------------------------------------------------------------
 */
void	cache_enable_all (void) {

	asm volatile (
			/* Read the Control Register */

			"mrc	p15, 0, r0, c1, c0, 0\n"

			/* Set the bits for L1 D and I caches */

			"ldr	r1, =0x1004\n"
			"orr	r0, r1\n"

			/* Write the new Control register */

			"mcr	p15, 0, r0, c1, c0, 0\n"

			/* Read the Auxialiary Control Register */

			"mrc	p15, 0, r0, c1, c0, 1\n"

			/* Set the bit for L2 cache */

			"orr	r0, #0x2\n"

			/* Write the new Auxialiary Control Register */

			//"mcr	p15, 0, r0, c1, c0, 0\n"
			
			:		/* Output	*/
			:		/* Input	*/
			: "r0", "r1"	/* Clobber	*/
			);
}

/*------------------------------------------------------------------------
 * cache_disable_all  -  Disable caches at all levels
 *------------------------------------------------------------------------
 */
void	cache_disable_all (void) {

	asm volatile (
			/* Read the Auxialiary Control Register */

			"mrc	p15, 0, r0, c1, c0, 1\n"

			/* Reset the bit for L2 cache */

			"bic	r0, #0x2\n"

			/* Write the new Auxiliary Control Register */

			"mcr	p15, 0, r0, c1, c0, 1\n"

			/* Read the Control Register */

			"mrc	p15, 0, r0, c1, c0, 0\n"

			/* Reset the bits for D and I caches */

			"ldr	r1, =0x1004\n"
			"bic	r0, r1\n"

			/* Write the new Control Register */

			"mcr	p15, 0, r0, c1, c0, 0\n"

			:		/* Output	*/
			:		/* Input	*/
			: "r0", "r1"	/* Clobber	*/
			);
}

/*------------------------------------------------------------------------
 * tlb_inv_all  -  Invalidate all TLB entries (data and instr.)
 *------------------------------------------------------------------------
 */
void	tlb_inv_all (void) {

	asm volatile (
			"mov	r0, #0\n"
			"mcr	p15, 0, r0, c8, c7, 0\n"

			: 	/* No output 	*/
			: 	/* No input	*/
			: "r0"	/* Clobber 	*/
		     );
}

/*------------------------------------------------------------------------
 * cache_inv_all  -  Invalidate all data and intr. caches
 *------------------------------------------------------------------------
 */
void	cache_inv_all (void) {

	struct	cache_info cinfo;	/* Cache information	*/
	uint32	nsets;			/* No. of sets		*/
	uint32	nways;			/* No. of ways		*/
	uint32	level;			/* Level of cache	*/
	uint32	csid;			/* Cache Size ID	*/
	int32	i, j, k;		/* Loop index variables	*/

	/* Get the cache information */

	cache_get_info(&cinfo);

	/* Invalidate all Instruction cache entries */

	asm volatile (
			"mov	r0, #0\n"
			"mcr	p15, 0, r0, c7, c5, 0\n"
			:
			:
			: "r0"
		     );

	/* Now invalidate all data cache entries */

	level = 0;
	for(i = 0; i < cinfo.ncaches; i++) {

		/* Read the Cache Size ID register for this cache level */

		asm volatile (
				"mcr	p15, 2, %1, c0, c0, 0\n"
				"mrc	p15, 1, %0, c0, c0, 0\n"
				: "=r" (csid)		/* Output	*/
				: "r" (level << 1)	/* Input	*/
			     	:			/* Clobber	*/
			     );

		/* Get no. of sets and ways at this level */

		nways = ((csid >> 3) & 0x3FF) + 1;
		nsets = ((csid >> 13) & 0x7FFF) + 1;

		for(j = 0; j < nsets; j++) {
			for(k = 0; k < nways; k++) {

				/* Generate the data item for	*/
				/* cache invalidation		*/

				data = (i << 1) | (j << 6);
				if(i == 0) {
					data |= (k << 30);
				}
				else {
					data |= (k << 29);
				}

				//kprintf("Invalidating cache level %d, set %d, way %d\n", i, j, k);
				//kprintf("data for cache inv: %08x\n", data);

				asm volatile (
					"mcr	p15, 0, %0, c7, c6, 2\n"
					:		/* Ouptut	*/
					: "r" (data)	/* Input	*/
					:		/* Clobber	*/
					);
			}
		}

		/* Go to the next cache level */

		level++;
	}
}

/*------------------------------------------------------------------------
 * cache_get_info  -  Get Cache information
 *------------------------------------------------------------------------
 */
void	cache_get_info (
		struct	cache_info *cinfo	/* Cache information	*/
		)
{
	uint32	clid;	/* Cache Level ID	*/

	/* Read the Cache Level ID Register */

	asm volatile (
			"mrc	p15, 1, %0, c0, c0, 1\n"
			: "=r" (clid)	/* Output	*/
			:		/* Input	*/
			:		/* Clobber	*/
		     );

	/* Extract the levels of unification and coherence */

	cinfo->lou = (clid >> 27) & 0x7;
	cinfo->loc = (clid >> 24) & 0x7;

	/* Compute the number of cache levels */

	cinfo->ncaches = 0;
	while((clid & 0x7) != 0) {
		clid >>= 3;
	}

	//kprintf("No. of caches: %d\n", cinfo->ncaches);
	//kprintf("Level of Unification: %d\n", cinfo->lou);
	//kprintf("Level of Coherence: %d\n", cinfo->loc);
}
