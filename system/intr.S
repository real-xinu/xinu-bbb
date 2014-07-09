/* intr.S - enable, disable, restore, halt, pause, irq_handler */

	.text
	.globl	enable
enable:
	cpsie	i	/* Enable interrupts	*/
	mov	pc, lr	/* Return		*/

	.globl	disable
disable:
	mrs	r0, cpsr/* Copy the CPSR into r0*/
	cpsid	i	/* Disable interrupts	*/
	mov	pc, lr	/* Return the CPSR	*/

	.globl	restore
restore:
	push	{r1, r2}/* Save r1, r2 on stack	*/
	mrs	r1, cpsr/* Copy CPSR in r1	*/
	ldr	r2, =0x01F00220
	and	r1, r1, r2/* Extract flags and	*/
	bic	r0, r0, r2/* other imp bits	*/
	orr	r1, r1, r0/* from the mask	*/
	msr	cpsr_cfsx, r1/* restore CPSR	*/
	pop	{r1, r2}/* Restore r1, r2	*/
	mov	pc, lr	/* Return		*/

	.globl	halt
	.globl	pause
halt:
pause:
	cpsid	i	/* Disable interrupts	*/
dloop:	b	dloop	/* Dead loop		*/

	.globl	irq_handler
irq_handler:
	sub	lr, lr, #4/* Correct return addr*/
	srsdb	sp!, #19/* Save return state on	*/
			/* the stack of		*/
			/* superviser mode	*/
	cps	#19	/* Change to superviser	*/
	push	{r0-r12, lr}/* Save all regs	*/
	bl	irq_dispatch/* Call dispatch	*/
	pop	{r0-r12, lr}/* Restore all regs	*/
	rfeia	sp!	/* Return from exception*/
			/* using the info stored*/
			/* on the stack		*/
