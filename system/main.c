/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
int32 cpudelay;
volatile uint32	gcounter = 400000000;
process	counterproc() {

	while(gcounter > 0) {
		gcounter--;
	}
	return OK;
}

process	main(void)
{
	/* Start the network */

	netstart();
	tcp_init();

#if 0
	byte	radsrc[] = {0, 1, 2, 3, 4, 5, 6, 7};
	byte	raddst[] = {8, 9, 10, 11, 12, 13, 14, 15};

	struct	netpacket_r pkt;
	memset(&pkt, 0, sizeof pkt);
	pkt.net_radftype = RAD_DATA;
	pkt.net_radsam = 3;
	memcpy(pkt.net_radsrc, radsrc, 8);
	pkt.net_raddam = 3;
	memcpy(pkt.net_raddst, raddst, 8);

	pkt.net_lpdisp = 0x41;
	pkt.net_ipvtch = 0x60;
	pkt.net_ipnh = 0;
	pkt.net_iphl = 254;
	pkt.net_iplen = htons(248);
	memcpy(pkt.net_ipsrc, ip_llprefix, 8);
	pkt.net_ipsrc[15] = 1;
	//memcpy(pkt.net_ipsrc+8, radsrc, 8);
	memcpy(pkt.net_ipdst, ip_llprefix, 8);
	pkt.net_ipdst[15] = 2;
	//memcpy(pkt.net_ipdst+8, raddst, 8);
	byte *bptr = pkt.net_ipdata;
	*bptr = IP_IPV6; bptr++;
	*bptr = 0;
	bptr += 7;
	struct iphdr *ipptr = (struct iphdr *)bptr;
	ipptr->ipvtch = 0x60;
	ipptr->ipnh = 1;
	ipptr->iphl = 255;
	ipptr->iplen = htons(200);
	memcpy(ipptr->ipsrc, ip_llprefix, 8);
	memcpy(ipptr->ipsrc+8, &pkt.net_ipsrc[8], 8);
	memcpy(ipptr->ipdst, ip_llprefix, 8);
	memcpy(ipptr->ipdst+8, pkt.net_ipdst+8, 8);
	ipptr->ipsrc[15] = 3;
	ipptr->ipdst[15] = 4;

	lp_send(&pkt);
#endif

	uint32	ipaddr;
	dot2ip("128.10.137.101", &ipaddr);
	kprintf("Server IP %x\n", ipaddr);

	int32	slot = tcp_register(ipaddr, 12345, 1);
	kprintf("slot = %d\n", slot);

	char	name[] = "AlexHamlin";
	kprintf("sending packet now");
	tcp_send(slot, name, 5);
	char	msg[50] = {0};
	tcp_recv(slot, msg, 50);
	kprintf("%s\n", msg);

	tcp_close(slot);

#if 0
	int32	lslot = tcp_register(NetData.ipucast, 12345, 0);
	int32	nslot, retval;
	char	name[50];
	char	msg[50];

	while(TRUE) {
		retval = tcp_recv(lslot, &nslot, 4);
		if(retval == SYSERR) {
			continue;
		}

		kprintf("New connection from: %x\n", tcbtab[nslot].tcb_rip);
		retval = tcp_recv(nslot, name, 10);
		name[retval] = 0;
		memset(msg, 0, 50);
		sprintf(msg, "Hello %s\n", name);
		tcp_send(nslot, msg, strlen(msg));
		//tcp_send(nslot, name, strlen(name));
		kprintf("calling tcp_close\n");
		tcp_close(nslot);
	}
#endif

#if 0
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
#endif
	return OK;
}
