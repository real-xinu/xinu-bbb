.text
.globl	enable
enable:
	cpsie	i
	mov	pc, lr

.globl	disable
disable:
	mrs	r0, cpsr
	cpsid	i
	mov	pc, lr

.globl	restore
restore:
	push	{r1, r2}
	mrs	r1, cpsr
	ldr	r2, =0x01F00220
	and	r1, r1, r2
	bic	r0, r0, r2
	orr	r1, r1, r0
	msr	cpsr_cfsx, r1
	pop	{r1, r2}
	mov	pc, lr

.globl	halt
.globl	pause
halt:
pause:
	cpsid	i
dloop:	b	dloop

.globl	inb
.globl	outb
inb:
outb:
	mov	pc, lr

.globl	irq_handler
irq_handler:
	sub	lr, lr, #4
	srsdb	sp!, #19
	cps	#19
	push	{r0-r12, lr}
	bl	irq_dispatch
	pop	{r0-r12, lr}
	rfeia	sp!

//irq_dispatch:
//	mov	pc, lr
