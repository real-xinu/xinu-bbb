/* net.c - net_init, netin, rawin, eth_hton */

#include <xinu.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * net_init - initialize network data structures and processes
 *------------------------------------------------------------------------
 */

void	net_init (void)
{
	int32	iface;			/* index into interface table	*/
	char	str[16];		/* string to hold process name	*/

	/* Initialize interface data structures */

	netiface_init();

	/* Initialize ARP cache for each interface */

	for (iface=0; iface<NIFACES; iface++) {
		arp_init(iface);

	}

	/* Initialize UDP */

	udp_init();

	/* Initialize ICMP */

	icmp_init();

	/* Create a network input process for each interface */

	for (iface=0; iface<NIFACES; iface++) {
		sprintf(str, "net%d_input", iface);
		resume(create(netin, 4196, 5000, str, 1, iface));
	}

	/* Initialize the IP output queue */

	ipoqueue.iqtail = ipoqueue.iqhead = 0;
	ipoqueue.iqsem = semcreate(0);

	/* Create an IP output process  */

	resume(create(ipout, 2048, 6000, "ip_output", 0));

	/* Create a low-level input process that reads raw frames and	*/
	/*	demultiplexes them to the correct interface		*/

	resume(create(rawin, 2048, 8000, "raw_input", 0));
}


/*------------------------------------------------------------------------
 * netin - repeatedly read and process the next incoming packet
 *------------------------------------------------------------------------
 */

process	netin (
	  int32	iface			/* Interface for this process	*/
	)
{
	struct	ifentry	*ifptr;		/* ptr to interface table entry	*/
	struct	netpacket *pkt;		/* ptr to current packet	*/

	/* Do forever: read packets from the network and process */

	ifptr = &if_tab[iface];
	while(1) {

		/* Obtain next packet arriving on an interface */

		wait(ifptr->if_sem);
		pkt = ifptr->if_queue[ifptr->if_head++];
		if (ifptr->if_head >= IF_QUEUESIZE) {
			ifptr->if_head = 0;
		}

		/* Store interface number in packet buffer */

		pkt->net_iface = iface;

		/* Convert Ethernet Type to host order */

		eth_ntoh(pkt);

		/* Demultiplex on Ethernet type */

		switch (pkt->net_ethtype) {

		    case ETH_ARP:			/* Handle ARP	*/
			arp_in(iface,(struct arppacket *)pkt);
			continue;

		    case ETH_IP:			/* Handle IP	*/
			ip_in(pkt);
			continue;
	
		    case ETH_IPv6:			/* Handle IPv6	*/
			freebuf((char *)pkt);
			continue;

		    default:	/* Ignore all other incoming packets	*/
			freebuf((char *)pkt);
			continue;
		}
	}
}

/*------------------------------------------------------------------------
 * rawin - continuously read the next incoming frame, examine the MAC
 *		address, and enqueue on the appropriate interface queue
 *------------------------------------------------------------------------
 */

process	rawin (void) {

	status	retval;			/* return value from function	*/
	struct	netpacket *pkt;		/* packet buffer being used now	*/
	int32	nbufs;			/* total number of buffers	*/
	struct	ifentry	*ifptr;		/* ptr to interface table entry	*/
	int32	iface;			/* index into interface table	*/

	/* Global buffer pool shared by all interfaces */

	nbufs = UDP_SLOTS * UDP_QSIZ + ICMP_SLOTS * ICMP_QSIZ
				+ NIFACES * IF_QUEUESIZE + 1;
	netbufpool = mkbufpool(PACKLEN, nbufs);

	if (netbufpool == SYSERR) {
		kprintf("Cannot allocate network buffer pool\n");
		kill(getpid());
	}

	open(ETHER0, NULL, NULL);

	pkt = (struct netpacket *)getbuf(netbufpool);
	while(1) {
	    	retval = read(ETHER0, (char *)pkt, PACKLEN);
	    	if (retval == SYSERR) {
			panic("Ethernet read error");
	    	}

		/* Demultiplex on MAC address */

		for (iface=0; iface<=NIFACES; iface++) {
		    ifptr = &if_tab[iface];
		    if (ifptr->if_state != IF_UP) {
			continue;
		    }
		    if ( (memcmp(ifptr->if_macucast, pkt->net_ethdst,
				ETH_ADDR_LEN) == 0) ||
			 (memcmp(ifptr->if_macbcast, pkt->net_ethdst,
				ETH_ADDR_LEN) == 0)) {

			/* packet goes to this interface */

			/* Check interface queue; drop packet if full	*/
									
			if (semcount(ifptr->if_sem) >= IF_QUEUESIZE) {
				kprintf("rawin: queue overflow on %d\n", iface);
				break;
			}

			/* Enqueue packet and signal semaphore */

			ifptr->if_queue[ifptr->if_tail++] = pkt;
			if (ifptr->if_tail >= IF_QUEUESIZE) {
				ifptr->if_tail = 0;
			}
			signal(ifptr->if_sem);

			/* Obtain a buffer for the next packet */

			pkt = (struct netpacket *)getbuf(netbufpool);
			break;
		    }
		}
	}
}


/*------------------------------------------------------------------------
 * eth_hton - convert Ethernet type field to network byte order
 *------------------------------------------------------------------------
 */
void 	eth_hton(
	  struct netpacket *pktptr
	)
{
	pktptr->net_ethtype = htons(pktptr->net_ethtype);
}


/*------------------------------------------------------------------------
 * eth_ntoh - convert Ethernet type field to host byte order
 *------------------------------------------------------------------------
 */
void 	eth_ntoh(
	  struct netpacket *pktptr
	)
{
	pktptr->net_ethtype = ntohs(pktptr->net_ethtype);
}
