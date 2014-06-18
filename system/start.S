.text
.globl	start

start:
	ldr	sp, =0x84000000

	ldr	r0, =edata
	ldr	r1, =end
	bl	bzero

	mrc	p15, 0, r0, c1, c0, 0
	and	r0, r0, #0xFFFFDFFF
	mcr	p15, 0, r0, c1, c0, 0

	ldr	r0, =0x20000
	mcr	p15, 0, r0, c12, c0, 0

	ldr	r0, =0x4030CE18
	ldr	r1, =instr
	ldr	r1, [r1]
	str	r1, [r0]

	ldr	r0, =0x4030CE38
	ldr	r1, =irq_handler
	str	r1, [r0]

	bl	nulluser

instr:	ldr	pc, [pc, #24]

bzero:
	mov	r2, #0
	add	r0, r0, #3
	and	r0, r0, #0xFFFFFFFC
bloop:	cmp	r0, r1
	bhs	bexit
	str	r2, [r0]
	add	r0, r0, #4
	b	bloop
bexit:	mov	pc, lr
