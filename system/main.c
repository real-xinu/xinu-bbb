/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
int32 cpudelay;
process	main(void)
{
	/* Start the network */

	ethinit(&devtab[ETHER0]);

	int32 i;
	for(i = 0; i < 6; i++) {
		NetData.ethucast[i] = i;
		NetData.ethbcast[i] = 0xff;
	}

	//kprintf("reading packet from ETHER0\n");
	//struct netpacket pkt;
	//read(ETHER0, &pkt, 1518);

	netstart();

	kprintf("\n...creating a shell\n");
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
