/*  main.c  - main */

#include <xinu.h>

uint32	ttbr0;
uint32	cr;
volatile uint32	clktimems;
volatile int a;
extern	struct memblk ucmemlist;
process	main(void)
{

	kprintf("Normal Memory List:\n");
	struct	memblk *memptr;

	memptr = memlist.mnext;
	while(memptr) {
		kprintf("\tAddress: %08x, Length: %d bytes\n", (uint32)memptr, memptr->mlength);
		memptr = memptr->mnext;
	}

	kprintf("\nUncache Memory List:\n");
	memptr = ucmemlist.mnext;
	while(memptr) {
		kprintf("\tAddress: %08x, Length: %d bytes\n", (uint32)memptr, memptr->mlength);
		memptr = memptr->mnext;
	}

	volatile byte	*ptr1, *ptr2;
	volatile uint32	time1, time2;

	ptr1 = (byte *)getucmem(100);
	ptr2 = (byte *)getmem(100);

	kprintf("getucmem. ptr1 %08x, ptr2 %08x\n", ptr1, ptr2);

	init_paging();

	uint32	*pd;

	pd = (uint32 *)&page_dir;

	kprintf("pd[0x811] %08x\n", pd[0x811]);
	kprintf("pd[0x800] %08x\n", pd[0x800]);

	asm(
		"push	{r0, r1}\n"
		"mcr	p15, 0, r0, c8, c7, 0\n"
		"isb\n"
		"mrc	p15, 0, r0, c1, c0, 0\n"
		"orr	r0, #4\n"
		"mcr	p15, 0, r0, c1, c0, 0\n"
		"isb\n"
		"dsb\n"
		"mrc	p15, 0, r0, c1, c0, 0\n"
		"orr	r0, #1\n"
		"mcr	p15, 0, r0, c1, c0, 0\n"
		"isb\n"
		"mov	r0, #0\n"
		"mcr	p15, 0, r0, c8, c7, 0\n"
		"isb\n"
		"ldr	r0, =0x81100000\n"
		"mcr	p15, 0, r0, c7, c6, 1\n"
		"isb\n"
		"dsb\n"
		"pop	{r0, r1}\n"
	);

	int32	i;
	//time1 = clktimems;

	i = 10000000;
	while(i > 0) {
		a = *ptr2;
		i--;
	}

	//time2 = clktimems;

	kprintf("Writing to cached mem: %d\n", (time2-time1));

	time1 = time2 = 0;

	//time1 = clktimems;

	i = 10000000;
	while(i > 0) {
		a = *ptr1;
		i--;
	}

	//time2 = clktimems;

	kprintf("Writing to uncached mem: %d\n", (time2-time1));

	freeucmem((char *)ptr1, 100);
/*
	while(1);

	asm("push {r0,r1}");
	asm("mrc p15, 0, r0, c1, c0, 0");
	asm("ldr r1,=0x1002");
	asm("bic r0,r1");
	asm("mcr p15, 0, r0, c1, c0, 0");
	asm("isb");
	asm("pop {r0,r1}");

	init_paging();

	asm("push {r0,r1}");
	asm("mrc p15, 0, r0, c2, c0, 0");
	asm("ldr r1,=ttbr0");
	asm("str r0,[r1]");
	asm("mov r0, #0");
	asm("mcr p15, 0, r0, c8, c7, 0");
	asm("pop {r0,r1}");

	kprintf("TTBR0: %08x\n", ttbr0);

	kprintf("Turning on MMU..");
	asm("push {r0,r1}");
	asm("mrc p15, 0, r0, c1, c0, 0");
	asm("orr r0, #1");
	asm("mcr p15, 0, r0, c1, c0, 0");
	asm("pop {r0,r1}");
	kprintf("Done\n");
*/
	/* Run the Xinu shell */

	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
    
}
