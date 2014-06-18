/* netstart.c - netstart */

#include <xinu.h>
#include <stdio.h>

bool8	host;				/* TRUE if running a host	*/
int32	bingid;				/* User's bing ID		*/

#define	ROUTER_SUFFIX	0x00006464	/* router IP address on othernet*/
					/*   ENDS IN   .100.100		*/

struct ether ethertab[1];
void e1000e_rar_set(struct ether *ethptr, byte *bptr, uint32 value) {
}
/*------------------------------------------------------------------------
 * netstart - ask user for Bing ID and whether to run a host or router
 *		and set configuration variables accordingly
 *------------------------------------------------------------------------
 */

void	netstart (void)
{
	char	buffer[20];		/* input buffer			*/
	int32	nchars;			/* number Of characters read	*/
	int32	i;			/* counter from 0 to nchars	*/
	bool8	found;			/* was a valid answer found?	*/
	char	ch;			/* one character from the buff.	*/
	struct	ifentry	*ifptr;		/* ptr to interface		*/
	uint32	ipaddr;			/* IP address on interface 0	*/
	uint32	suffix;			/* host suffix to use on both	*/
					/*   othernet interfaces	*/
	static	uint32	rarindex = 1;	/* next RAR index to use	*/
	struct	ether	*ethptr;	/* ptr to Ethernet control blk.	*/
	char	*ifnames[] = {"Lab Ethernet","Othernet 1","Othernet 2"};

	/* Initialize network interfaces */

	kprintf("...initializing network stack\n");
	net_init();

	/* Initialize NAT tables */

	nat_init();

	/* Delay because Ethernet driver doesn't work without it */

	sleepms(800);
	
	/* Force system to use DHCP to obtain an address */

	kprintf("...using dhcp to obtain an IP address\n");
	ipaddr = getlocalip();
	if (ipaddr == SYSERR) {
		panic("Error: could not obtain an IP address\n\r");
	}
	kprintf("\nIP address is %d.%d.%d.%d   (0x%08x)\n\r",
		(ipaddr>>24)&0xff, (ipaddr>>16)&0xff, (ipaddr>>8)&0xff,
		ipaddr&0xff,ipaddr);
	ifptr = &if_tab[0];

	kprintf("Subnet mask is %d.%d.%d.%d and router is %d.%d.%d.%d\n\r",
		(ifptr->if_ipmask>>24)&0xff, (ifptr->if_ipmask>>16)&0xff,
		(ifptr->if_ipmask>> 8)&0xff,  ifptr->if_ipmask&0xff,
		(ifptr->if_iprouter>>24)&0xff, (ifptr->if_iprouter>>16)&0xff,
		(ifptr->if_iprouter>> 8)&0xff, ifptr->if_iprouter&0xff);

	/* Ask the user for a Bing ID */

	found = FALSE;
	while (!found) {
		printf("\n\rEnter a bing ID between 0 and 255: ");
		nchars = read(CONSOLE, buffer, 20);
		if ((nchars == SYSERR) || (nchars == EOF) ||
						(nchars < 2)) {
			continue;
		}
		nchars--;	/* Eliminate NEWLINE */
		bingid = 0;
		for (i=0; i<nchars; i++) {
			ch = buffer[i];
			if ( (ch<'0') || (ch>'9')) {
				break;
			}
			bingid = 10*bingid + (ch-'0');
		}
		if ((i<nchars) || (bingid > 254)) {
			continue;
		}
		found = TRUE;
	}
	printf("Bing ID is set to %d.\n", bingid);

	/* Generate Othernet emulation addresses */

	suffix = ipaddr & 0x0000ffff;	/* suffix is last two bytes of	*/
					/*   the IP address		*/

	ethptr = &ethertab[0];

	/* Othernet 1 */

	ifptr = &if_tab[1];
	ifptr->if_ipmask  = 0xffff0000;	/* 255.255.0.0. */
	ifptr->if_ipprefix= 0x0a010000;	/* 10.1.0.0/16	*/
	ifptr->if_ipucast = ifptr->if_ipprefix | suffix;
	ifptr->if_ipbcast = ifptr->if_ipprefix | ~ifptr->if_ipmask;

	ifptr->if_macucast[0] = 0x11;
	ifptr->if_macucast[1] = 0xff & (ipaddr >> 24);
	ifptr->if_macucast[2] = 0xff & (ipaddr >> 16);
	ifptr->if_macucast[3] = 0xff & (ipaddr >>  8);
	ifptr->if_macucast[4] = 0xff & (ipaddr >>  0);
	ifptr->if_macucast[5] = 0xff & bingid;
	e1000e_rar_set(ethptr, ifptr->if_macucast, rarindex++);

	ifptr->if_macbcast[0] = 0x11;
	ifptr->if_macbcast[1] = 0xff;
	ifptr->if_macbcast[2] = 0xff;
	ifptr->if_macbcast[3] = 0xff;
	ifptr->if_macbcast[4] = 0xff;
	ifptr->if_macbcast[5] = 0xff & bingid;
	e1000e_rar_set(ethptr, ifptr->if_macbcast, rarindex++);
	ifptr->if_ipvalid = TRUE;


	/* Othernet 2 */

	ifptr = &if_tab[2];
	ifptr->if_ipmask  = 0xffff0000;	/* 255.255.0.0. */
	ifptr->if_ipprefix= 0x0a020000;	/* 10.2.0.0/16	*/
	ifptr->if_ipucast = ifptr->if_ipprefix | suffix;
	ifptr->if_ipbcast = ifptr->if_ipprefix | ~ifptr->if_ipmask;

	ifptr->if_macucast[0] = 0x21;
	ifptr->if_macucast[1] = 0xff & (ipaddr >> 24);
	ifptr->if_macucast[2] = 0xff & (ipaddr >> 16);
	ifptr->if_macucast[3] = 0xff & (ipaddr >>  8);
	ifptr->if_macucast[4] = 0xff & (ipaddr >>  0);
	ifptr->if_macucast[5] = 0xff & bingid;
	e1000e_rar_set(ethptr, ifptr->if_macucast, rarindex++);

	ifptr->if_macbcast[0] = 0x21;
	ifptr->if_macbcast[1] = 0xff;
	ifptr->if_macbcast[2] = 0xff;
	ifptr->if_macbcast[3] = 0xff;
	ifptr->if_macbcast[4] = 0xff;
	ifptr->if_macbcast[5] = 0xff & bingid;
	e1000e_rar_set(ethptr, ifptr->if_macbcast, rarindex++);
	ifptr->if_ipvalid = TRUE;

	/* Ask the user what to run */

	found = FALSE;
	while (!found) {
		printf("\nEnter r for router or hX for host on interface X: ");
		nchars = read(CONSOLE, buffer, 20);
		switch (nchars) {
		    case 2:
			ch = buffer[0];
			if ( (ch!='r') && (ch!='R')) {
				continue;
			}
			host = FALSE;
			ifprime = 0;
			found = TRUE;
			break;

		    case 3:
			ch = buffer[0];
			if ((ch!='h') && (ch!='H')) {
				continue;
			}
			ch = buffer[1];
			if ( (ch<'0') || (ch>'2')) {
				continue;
			}
			ifprime = ch - '0';
			host = TRUE;
			found = TRUE;
			break;

		    default:
			continue;
		}

	}
	if (host) {
		printf("\nRunning host on %s\n\r", ifnames[ifprime]);
		if (ifprime != 0) {
			/* Shut off Ethernet and bring up other net */
			if_tab[0].if_state = IF_DOWN;
			ifptr = &if_tab[ifprime];
			ifptr->if_state = IF_UP;
			/* Set default route to router address */
			if (ifptr->if_iprouter == 0) {
				ifptr->if_iprouter = ifptr->if_ipprefix |
							ROUTER_SUFFIX;
			}
		}
	} else {
		kprintf("Running a router\n\r");
		for (i=0; i<NIFACES; i++) {
			ifptr = &if_tab[i];
			ifptr->if_state = IF_UP;
			if (i != 0) {
				/* Set router address */
				ifptr->if_ipucast = ifptr->if_ipprefix |
							ROUTER_SUFFIX;
			}
		}
	}
	return;
}
