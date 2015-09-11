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

	//ethinit(&devtab[ETHER0]);
/*
	uint32	*macaddr = 0x44e10630;
	kprintf("mac addr: %x %x\n", *macaddr, *(macaddr+1));
	int32 i;
*/
/*	for(i = 0; i < 6; i++) {
		NetData.ethucast[i] = i;
		NetData.ethbcast[i] = 0xff;
	}*/

	//kprintf("reading packet from ETHER0\n");
	//struct netpacket pkt;
	//read(ETHER0, &pkt, 1518);

	//resume(create(counterproc, 8192, 19, "counter proc", 0, NULL));
	//sleep(10);
	//kprintf("Counter value: %d\n", gcounter);

	//DELAY(5000000);
	//netstart();

	kprintf("SPI1 %d, NDEVS %d\n", SPI1, NDEVS);
	kprintf("SPI device CSR: %x\n", devtab[SPI0].dvcsr);

	struct	spi_transfer msg;
	byte	tx[10];
	byte	rx[10];

	msg.txbuf = tx;
	msg.rxbuf = rx;
	msg.length = 5;
	tx[0] = 0xaf;
	tx[1] = 0x8f;
	memset(&tx[2], 0, 8);
	memset(rx, 0, 10);

	control(SPI1, SPI_CTRL_TRANSFER, (int32)&msg, 0);

	int	i;
	for(i = 0; i < 5; i++) {
		kprintf("%x\n", rx[i]);
	}

	kprintf("RADIO0 %d SPI: %d, %d\n", RADIO0, radtab[0].spidev, SPI0);

	struct	netpacket pkt;
	memset(&pkt, 0, sizeof pkt);

	pkt.net_radftype = RAD_DATA;
	pkt.net_radsec = 0;
	pkt.net_radfp = 0;
	pkt.net_radar = 0;
	pkt.net_radpidc = 0;
	pkt.net_raddam = RAD_AM3;
	pkt.net_radsam = RAD_AM3;
	pkt.net_radfvers = 1;
	pkt.net_radseq = 1;
	pkt.net_raddpan = htons(0x1234);
	memset(pkt.net_raddst, 0xff, 8);
	pkt.net_radspan = htons(0x5678);
	memset(pkt.net_radsrc, 0x01, 8);

	byte	*bptr = (byte *)&pkt;
	for(i = 0; i <  23; i++) {
		kprintf("%02x ", *bptr);
		bptr++;
	}
	kprintf("\n");

	byte	partnum, txlast;

	ticc1200_read_ext(&radtab[0], 0x8F, &partnum);
	kprintf("part number %2x\n", partnum);

	ticc1200_write(&radtab[0], 0x35, 0);
	ticc1200_write(&radtab[0], 0x34, 0);

	ticc1200_read_ext(&radtab[0], TICC1200_ADDR_TXLAST, &txlast);
	kprintf("txlast: %02x\n", txlast);

	byte	pkttxbuf[25];
	byte	pktrxbuf[25];
	struct	spi_transfer pktmsg;

	pktmsg.txbuf = pkttxbuf;
	pktmsg.rxbuf = pktrxbuf;
	pktmsg.length = 24;

	pkttxbuf[0] = TICC1200_COM_SFIFO | TICC1200_COM_BURST;
	pkttxbuf[1] = txlast;
	memcpy(&pkttxbuf[1], &pkt, 23);

	control(radtab[0].spidev, SPI_CTRL_TRANSFER, &msg, 0);

	ticc1200_read_ext(&radtab[0], TICC1200_ADDR_TXLAST, &txlast);
	kprintf("txlast after writing: %2x\n", txlast);

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
