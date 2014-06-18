.text
.globl	ctxsw
ctxsw:
	push	{r0-r11, lr}
	push	{lr}
	mrs	r2, cpsr
	push	{r2}
	str	sp, [r0]
	ldr	sp, [r1]
	pop	{r0}
	bl	restore
	pop	{lr}
	pop	{r0-r12}
	mov	pc, r12
