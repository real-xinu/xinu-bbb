/* mmu.c - mmu_enable, mmu_disable, mmu_set_ttbr */

#include <xinu.h>

/*------------------------------------------------------------------------
 * mmu_enable  -  Enable the MMU
 *------------------------------------------------------------------------
 */
void	mmu_enable (void) {

	asm volatile (
			/* Read the Control Register */

			"mrc	p15, 0, r0, c1, c0, 0\n"

			/* Set the MMU Enable bit */

			"orr	r0, #0x00000001\n"

			/* Write the new Control Register */

			"mcr	p15, 0, r0, c1, c0, 0\n"
			"isb\n"

			:	/* Output	*/
			:	/* Input	*/
			: "r0"	/* Clobber	*/
		     );
}

/*------------------------------------------------------------------------
 * mmu_disable  -  Disable the MMU
 *------------------------------------------------------------------------
 */
void	mmu_disable (void) {

	asm volatile (
			/* Read the Control Register */

			"mrc	p15, 0, r0, c1, c0, 0\n"

			/* Reset the MMU Enable bit */

			"bic	r0, #0x00000001\n"

			/* Write the new Control Register */

			"mcr	p15, 0, r0, c1, c0, 0\n"

			:	/* Output	*/
			:	/* Input	*/
			: "r0"	/* Clobber	*/
		     );
}

/*------------------------------------------------------------------------
 * mmu_set_ttbr  -  Set the Translation Table Base Register
 *------------------------------------------------------------------------
 */
void	mmu_set_ttbr (
		void	*ttbaddr	/* Translation Tab. Base Address*/
		)
{
	asm volatile (
			/* We want to use TTBR0 only */

			"ldr	r0, =0x00000020\n"

			/* Write the value into TTBCR */

			"mcr	p15, 0, r0, c2, c0, 2\n"

			/* Load the base address in r0 */

			"mov	r0, %0\n"

			/* Make the translation table walk cacheable */

			"orr	r0, #0x00000002\n"

			/* Write the new TTBR0 */

			"mcr	p15, 0, r0, c2, c0, 0\n"

			/* Perform memory synchronization */

			"dsb\n"
			"dmb\n"

			:		/* Output	*/
			: "r" (ttbaddr)	/* Input	*/
			: "r0"		/* Clobber	*/
		);
}
