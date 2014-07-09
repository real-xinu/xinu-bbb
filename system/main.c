/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>
/*
void radputc(char c) {

	struct uart_csreg *csrptr = devtab[RADIO].dvcsr;

	while((csrptr->lsr & UART_LSR_THRE) == 0);

	csrptr->buffer = c;
}
char radgetc() {

	struct uart_csreg *csrptr = devtab[RADIO].dvcsr;
	uint32 buffer;

	while((csrptr->lsr & UART_LSR_DR) == 0);

	buffer = csrptr->buffer;
	kprintf("%x", buffer);
	return (char)buffer;
}*/
void radio_read();
void radio_write();
int32	main(void)

{
	uint32	retval;
//	byte	mac1[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
//	char	buf[1500];
//	struct	netpacket pkt;
//	int32	i;
//	bool8	done;

	kprintf("\n\n###########################################################\n\n");

//	done = FALSE;
//	while (!done) {
//		printf("Enter s for sender or r for receiver: ");
//		retval = read(CONSOLE, buf, 20);
//		if ( retval != 2 ) {
//			continue;
//		}
//		if ( (buf[0] != 'r') && (buf[0] != 's') ) {
//			continue;
//		} else {
//			done = TRUE;
//		}
//	}
//	e1000e_rar_set(&ethertab[0], mac1, 4);
//	if ( buf[0] == 'r' ) {
//		while (TRUE) {
//			retval = read(ETHER0, buf, 1500);
//			if (retval < 0) {
//				continue;
//			}
//			if (buf[0] == mac1[0]) {
//				kprintf("success!\n");
//			}
//		}
//	} else {
//		for (i=0; i<1500; i++) {
//			*(i + (char *)&pkt) = 0xff&i;
//		}
//		memcpy(pkt.net_ethdst, mac1, 6);
//		control(ETHER0, ETH_CTRL_GET_MAC,
//				(int32)&pkt.net_ethsrc, 0);
//		pkt.net_ethtype = ntohs(0x0800);
//		while (TRUE) {
//			retval = read(CONSOLE, buf, 100);
//			write(ETHER0, (char *)&pkt, 1200);
//		}
//	}
	//disable();
	/*uint32 *cmwkup = 0x44e00400;
	kprintf("cmwkup off 0 %x\n", *cmwkup);
	cmwkup = 0x44e004c4;
	*cmwkup = 0x2;
	cmwkup = 0x44e0014c;
	kprintf("32khz clk %x\n", *cmwkup);
	*cmwkup = 0x2;
	kprintf("32khz clk %x\n", *cmwkup);
	cmwkup = 0x44e00150;
	kprintf("24MHz clk %x\n", *cmwkup);
	cmwkup = 0x44e00528;
	*cmwkup = 0;
	clkinit();
	kprintf("returned from clkinit\n");
	struct am335x_timer1ms *tcsrptr = 0x44e31000;
	tcsrptr->ttgr = 1;
	while(1);*/
	/*while(1) {
		kprintf("testing sleep %d\n", clktime);
		sleep(1);
	}
	uint32 *padptr = 0x44e10980;
	kprintf("uart1_rxd %x\n", *padptr);
	uint32 padvalue;
	padvalue = *padptr;
	padvalue &= 0xffffffb8;
	//padvalue |= 0x00000060;
	*padptr = padvalue;
	kprintf("uart1_rxd %x\n", *padptr);
	padptr = 0x44e10984;
	kprintf("uart1_txd %x\n", *padptr);
	padvalue = *padptr;
	padvalue &= 0xffffff98;
	//padvalue |= 0x00000040;
	*padptr = padvalue;
	kprintf("uart1_txd %x\n", *padptr);
	kprintf("trying to communicate with radio\n");
	char buf[100] = {'\0'};
	uint32 nread;
	//write(RADIO, "+++", 3);
	//nread = read(RADIO, buf, 3);
	kprintf("nread %d msg %s\n", nread, buf);			
	kprintf("...starting the network\n");
	//netstart();
	printf("radio comm\n");
	struct uart_csreg *csrptr = devtab[RADIO].dvcsr;*/
	/*kprintf("sending a +++\n");
	radputc('+');
	radputc('+');
	radputc('+');
	kprintf("reading back from radio");
	kprintf("%c", radgetc());
	kprintf("%c", radgetc());
	kprintf("%c", radgetc());*/
	//resume(create(radio_read, 8*1024, 100, "radio_read", 0, NULL));
	//resume(create(radio_write, 8*1024, 101, "radio_write", 0, NULL));
	//while(1);
	write(RADIOTTY, "+++", 3);
	while(1) {
		char buf[100];
		int count;
		count = 0;
		while(count == 0) {
			count = read(RADIOTTY, buf, 0);
		}
		buf[count] = '\0';
		kprintf("%s\n", buf);
		count = read(CONSOLE, buf, 99);
		kprintf("read %d\n", count);
		//buf[count-1] = '\r';
		write(RADIOTTY, buf, count);
		//count = read(RADIO, buf, 3);
		//buf[count] = '\0';
		//kprintf("%s\n", buf);
	}
	//write(CONSOLE, "test\n", 5);
	//nread = read(CONSOLE, buf, 10);
	//buf[nread] = '\0';
	//kprintf("read from CONSOLE: %s\n", buf);
	//uint32 *ptr = 0x44e0006c;
	/*ptr = 0x44e00c08;
	kprintf("pmper1 %x\n", *ptr);
	ptr = 0x44e00c0c;
	kprintf("pmper2 %x\n", *ptr);
	ptr = 0x44e00490;
	kprintf("cm_clkmode_dpll_core %x\n", *ptr);
	ptr = 0x44e00000;
	kprintf("l4lsclkstctrl %x\n", *ptr);
	ptr = 0x44e00060;
	*ptr = 0x2;
	kprintf("l4ls %x\n", *ptr);
	ptr = 0x44e00000;
	kprintf("l4lsclkstctrl %x\n", *ptr);*/
	//ptr = 0x44e0006c;
	//*ptr = 0x2;
	//kprintf("Uart1 clk %x\n", *ptr);
	//struct uart_csreg *csrptr = devtab[RADIO].dvcsr;
	//kprintf("accessing uart 2 registers\n");
	//kprintf("uart2 lcr %x\n", csrptr->lcr);
	//kprintf("\n...creating a shell\n");
	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	/* Wait for shell to exit and recreate it */

	while (TRUE) {
		retval = receive();
		sleepms(200);
		kprintf("\n\n\rMain process recreating shell\n\n\r");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	return OK;
}
/*
void radio_read() {

	char c;
	kprintf("radio_read process\n");
	while(1) {
		read(RADIO, &c, 1);
		kprintf("%c", c);
		if(c == '\r')
			kprintf("\n");
	}
}

void radio_write() {

	char buf[100];
	uint32 count;
	kprintf("radio_write process\n");
	while(1) {
		count = read(CONSOLE, buf, 99);
		//kprintf("read from console %d\n", count);
		if((buf[0] == '+') && (buf[1] == '+') && (buf[2] == '+')) {
			count = 3;
		}
		write(RADIO, buf, count);
	}
}*/
