/* ctxsw.S - ctxsw (for ARM) */

/*------------------------------------------------------------------------
 * ctxsw -  ARM context switch; the call is ctxsw(&old_sp, &new_sp)
 *------------------------------------------------------------------------
 */
.text
.globl	ctxsw
ctxsw:
	push	{r0-r11, lr}		/* Push regs 0 - 11 and lr	*/
	push	{lr}			/* Push return address		*/
	mrs	r2, cpsr		/* Push status flags		*/
	push	{r2}			/* Push r2			*/
	str	sp, [r0]		/* Save old process stack ptr	*/
	ldr	sp, [r1]		/* Pick up new process stack ptr*/
	pop	{r0}			/* Pop r0			*/
	bl	restore			/* Restore status flags		*/
	pop	{lr}			/* Pop lr			*/
	pop	{r0-r12}		/* Pop regs 0 - 12		*/
	mov	pc, r12			/* Return to new process caller	*/
