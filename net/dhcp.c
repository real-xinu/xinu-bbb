/* dhcp.c - getlocalip */

#include <xinu.h>

/*------------------------------------------------------------------------
 * getlocalip - use DHCP to obtain an IP address
 *------------------------------------------------------------------------
 */
uint32	getlocalip(void)
{
	int32	slot;			/* UDP slot to use		*/
	struct	dhcpmsg dmsg;		/* holds outgoing DHCP discover	*/
					/*	message			*/
	struct	dhcpmsg dmsg2;		/* holds incoming DHCP offer	*/
					/* and outgoing request message	*/
	uint32	xid;			/* xid used for the exchange	*/
	int32	i, j;			/* retry counters		*/
	int32	len;			/* length of data sent		*/
	int32	inlen;			/* length of data received	*/
	char	*optptr;		/* pointer to options area	*/
	char	*eop;			/* address of end of packet	*/
	int32	msgtype;		/* type of DCHP message		*/
	uint32	addrmask;		/* address mask for network	*/
	uint32	routeraddr;		/* default router address	*/
	uint32	tmp;			/* used for byte conversion	*/
	struct	ifentry	*ifptr;		/* ptr to interface		*/

	/* Select interface and obtain a pointer to the table entry */

	ifptr = &if_tab[0];		/* Use interface 0 for DHCP	*/
	if (ifptr->if_ipvalid) {	/* already have an IP address	*/
		return ifptr->if_ipucast;
	}
	slot = udp_register(0, 0, UDP_DHCP_SPORT, UDP_DHCP_CPORT);
	if (slot == SYSERR) {
		kprintf("getlocalip: cannot register with UDP\r\n");
		return SYSERR;
	}
	memcpy(&xid, ifptr->if_macucast, 4);
					/* use 4 bytes from MAC as XID	*/

	/* handcraft a DHCP Discover message in dmsg */

	dmsg.dc_bop = 0x01;		/* Outgoing request		*/
	dmsg.dc_htype = 0x01;		/* hardware type is Ethernet	*/
	dmsg.dc_hlen = 0x06;		/* hardware address length	*/
	dmsg.dc_hops = 0x00;		/* Hop count			*/
	dmsg.dc_xid = htonl(xid);	/* xid (unique ID)		*/
	dmsg.dc_secs = 0x0000;		/* seconds			*/
	dmsg.dc_flags = 0x0000;		/* flags			*/
	dmsg.dc_cip = 0x00000000;	/* Client IP address		*/
	dmsg.dc_yip = 0x00000000;	/* Your IP address		*/
	dmsg.dc_sip = 0x00000000;	/* Server IP address		*/
	dmsg.dc_gip = 0x00000000;	/* Gateway IP address		*/
	memset(&dmsg.dc_chaddr,'\0',16);/* Client hardware address	*/
	memcpy(&dmsg.dc_chaddr, ifptr->if_macucast, ETH_ADDR_LEN);
	memset(&dmsg.dc_bootp,'\0',192);/* zero the bootp area		*/
	dmsg.dc_cookie = htonl(0x63825363); /* Magic cookie for DHCP	*/

	dmsg.dc_opt[0] = 0xff & 53;	/* DHCP message type option	*/
	dmsg.dc_opt[1] = 0xff &  1;	/* option length		*/
	dmsg.dc_opt[2] = 0xff &  1;	/* DHCP Dicover message		*/
	dmsg.dc_opt[3] = 0xff &  0;	/* Options padding		*/

	dmsg.dc_opt[4] = 0xff & 55;	/* DHCP parameter request list	*/
	dmsg.dc_opt[5] = 0xff &  2;	/* option length		*/
	dmsg.dc_opt[6] = 0xff &  1;	/* request subnet mask 		*/
	dmsg.dc_opt[7] = 0xff &  3;	/* request default router addr.	*/

	dmsg.dc_opt[8] = 0xff &  0;	/* options padding		*/
	dmsg.dc_opt[9] = 0xff &  0;	/* options padding		*/
	dmsg.dc_opt[10]= 0xff &  0;	/* options padding		*/
	dmsg.dc_opt[11]= 0xff &  0;	/* options padding		*/

	len = (char *)&dmsg.dc_opt[11] - (char *)&dmsg + 1;

	for (i = 0; i < DHCP_RETRY; i++) {
		udp_sendto(slot, IP_BCAST, UDP_DHCP_SPORT,
						(char *)&dmsg, len);

		/* Read 3 incoming DHCP messages and check for an offer	*/
		/* 	or wait for three timeout periods if no message */
		/* 	arrives.					*/

		for (j=0; j<3; j++) {
			inlen = udp_recv(slot, (char *)&dmsg2,
					sizeof(struct dhcpmsg),2000);
			if (inlen == TIMEOUT) {
				continue;
			} else if (inlen == SYSERR) {
				return SYSERR;
			}
			/* Check that incoming message is a valid	*/
			/* response (ID	matches our request)		*/

			if ( (ntohl(dmsg2.dc_xid) != xid) ) {
				continue;
			}

			eop = (char *)&dmsg2 + inlen - 1;
			optptr = (char *)&dmsg2.dc_opt;
			msgtype = addrmask = routeraddr = 0;
			while (optptr < eop) {

		    	switch (*optptr) {
				case 53:	/* message type */
					msgtype = 0xff & *(optptr+2);
				break;

			case 1:		/* subnet mask */
				memcpy((void *)&tmp, optptr+2, 4);
				addrmask = ntohl(tmp);
				break;

			case 3:		/* router address */
				memcpy((void *)&tmp, optptr+2, 4);
				routeraddr = ntohl(tmp);
				break;
		    	}
		    	optptr++;	/* move to length octet */
		    	optptr += (0xff & *optptr) + 1;
			}

			if (msgtype == 0x02) {	/* offer - send request	*/
				dmsg2.dc_opt[0] = 0xff & 53;
				dmsg2.dc_opt[1] = 0xff &  1;
				dmsg2.dc_opt[2] = 0xff &  3;
				dmsg2.dc_bop = 0x01;
				udp_sendto(slot, IP_BCAST, UDP_DHCP_SPORT,
					(char *)&dmsg2,
					sizeof(struct dhcpmsg) - 4);
			
			} else if (dmsg2.dc_opt[2] != 0x05) {
				/* if not an ack skip it */
				continue;
			}
			if (addrmask != 0) {
				ifptr->if_ipmask = addrmask;
			}
			if (routeraddr != 0) {
				ifptr->if_iprouter = routeraddr;
			}
			ifptr->if_ipucast = ntohl(dmsg2.dc_yip);
			ifptr->if_ipprefix = ifptr->if_ipucast &
							ifptr->if_ipmask;
			ifptr->if_ipbcast = ifptr->if_ipprefix | 
							~ifptr->if_ipmask;
			ifptr->if_ipvalid = TRUE;
			udp_release(slot);
			return ifptr->if_ipucast;
		}
	}

	kprintf("DHCP failed to get response\r\n");
	udp_release(slot);
	return (uint32)SYSERR;
}
