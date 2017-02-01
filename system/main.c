/*  main.c  - main */

#include <xinu.h>

uint32	ttbr0;
uint32	cr;
process	main(void)
{

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
