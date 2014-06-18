/* evec.c -- initevec, doevec */

#include <xinu.h>
#include <stdio.h>

/*#define STKTRACE*/
/*#define REGDUMP*/

/*
 * The girmask is used as a mask for interrupts that don't have a
 * handler set. disable() & restore() are OR-ed with it to get the
 * mask actually used.
 */
uint16	girmask;

extern	void	setirmask(void);
extern	struct	idt idt[NID];
extern	struct	segtr idtr;
extern	long	defevec[];
extern	void	userret(void);
extern	void	init8259();
extern	int	lidt();
uint32	intc_vector[128];
/*------------------------------------------------------------------------
 * initevec - initialize exception vectors to a default handler
 *------------------------------------------------------------------------
 */
int32	initevec()
{
	struct	intc_csreg *csrptr = (struct intc_csreg *)0x48200000;

	csrptr->sysconfig |= (INTC_SYSCONFIG_SOFTRESET);
	while((csrptr->sysstatus & INTC_SYSSTATUS_RESETDONE) == 0);

	return OK;
}

/*------------------------------------------------------------------------
 * set_evec - set exception vector to point to an exception handler
 *------------------------------------------------------------------------
 */
int32	set_evec(uint32 xnum, uint32 handler)
{
	struct	intc_csreg *csrptr = (struct intc_csreg *)0x48200000;
	uint32	bank;
	uint32	mask;

	if(xnum > 127) {
		return SYSERR;
	}

	intc_vector[xnum] = handler;

	bank = (xnum/32);
	mask = (0x00000001 << (xnum%32));

	csrptr->banks[bank].mir &= (~mask);

	return OK;
}

void	irq_dispatch()
{
	//kprintf("irq_dispatch\n");
	struct	intc_csreg *csrptr = (struct intc_csreg *)0x48200000;
	uint32	xnum;
	interrupt (*handler)();

	xnum = csrptr->sir_irq & 0x7F;
	//kprintf("irq_dispatch xnum %d\n", xnum);
	if(intc_vector[xnum]) {
		handler = intc_vector[xnum];
		handler(xnum);
	}

	csrptr->control |= (INTC_CONTROL_NEWIRQAGR);
}

char *inames[] = {
	"divided by zero",
	"debug exception",
	"NMI interrupt",
	"breakpoint",
	"overflow",
	"bounds check failed",
	"invalid opcode",
	"coprocessor not available",
	"double fault",
	"coprocessor segment overrun",
	"invalid TSS",
	"segment not present",
	"stack fault",
	"general protection violation",
	"page fault",
	"coprocessor error"
};

static long *fp;
/*------------------------------------------------------------------------
 * trap -- print some debugging info when a trap occurred 
 * Note: Need some more work.
 *------------------------------------------------------------------------
*/
#define REGDUMP

void trap(int inum)
{
	long		*sp;
	intmask 	mask;

	mask = disable();
	kprintf("TRAP\n");
	//asm("movl	%ebp,fp");
	sp = fp + 15;	/* eflags/CS/eip/ebp/regs/trap#/Xtrap/ebp */
	kprintf("Xinu trap!\n");
	if (inum < 16)
		kprintf("exception %d (%s) currpid %d (%s)\n", inum,
			inames[inum], currpid, proctab[currpid].prname);
	else
		kprintf("exception %d currpid %d (%s)\n", inum, currpid, 
			proctab[currpid].prname);
#ifdef REGDUMP
	kprintf("eflags %X ", *sp--);
	sp--;	/* skip OLD CS */
	kprintf("eip %X ", *sp);
	sp--;
	if (inum == 8 || (inum >= 10 && inum <= 14)) {
		kprintf("error code %08x (%u)\n", *sp, *sp);
		sp--;
	}
	sp--;	/* skip FP for Xint call */
	kprintf("register dump:\n");
	kprintf("eax %08X (%u)\n", *sp, *sp);
	sp--;
	kprintf("ecx %08X (%u)\n", *sp, *sp);
	sp--;
	kprintf("edx %08X (%u)\n", *sp, *sp);
	sp--;
	kprintf("ebx %08X (%u)\n", *sp, *sp);
	sp--;
	kprintf("esp %08X (%u)\n", *sp, *sp);
	sp--;
	kprintf("ebp %08X (%u)\n", *sp, *sp);
	fp = sp;
	sp--;
	kprintf("esi %08X (%u)\n", *sp, *sp);
	sp--;
	kprintf("edi %08X (%u)\n", *sp, *sp);
	sp--;
#endif // REGDUMP
#ifdef STKTRACE
	stacktrace(currpid);
#endif // STKTRACE

	panic("Trap processing complete...\n");
}
