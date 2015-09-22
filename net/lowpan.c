/* lowpan.c - lp_send */

#include <xinu.h>

byte	ip_llprefix[] = {0xfe, 0x80, 0, 0, 0, 0, 0, 0,
			 0, 0, 0, 0, 0, 0, 0, 0};

/*------------------------------------------------------------------------
 * lp_send  -  Compress and send an IPv6 datagram
 *------------------------------------------------------------------------
 */
int32	lp_send (
		 struct	netpacket_r *pkt
		)
{
	struct	netpacket_r cpkt;	/* Compressed packet pointer	*/
	struct	lp_iphc *iphc;		/* IPHC header pointer		*/
	struct	lp_frag1 *f1;		/* Lowpan Frag1 header		*/
	struct	lp_fragn *fn;		/* Lowpan Fragn header		*/
	struct	iphdr *ipptr;		/* IP header			*/
	struct	udphdr *udpptr;		/* UDP header			*/
	byte	*encsrc, *encdst;	/* Encap. src and dst addresses	*/
	byte	*cstart;		/* Start of compression		*/
	byte	*ustart;		/* Start of uncompressed pkt	*/
	byte	*cptr, *uptr;		/* Current compressed and	*/
					/* uncompressed pointers	*/
	byte	*chdrs[10];		/* Compressed header addresses	*/
	byte	*uhdrs[10];		/* Uncompressed hdr addresses	*/
	byte	nhs[10];		/* Next header types		*/
	byte	nh = IP_IPV6;		/* Next header type - IPv6	*/
	bool8	ipenc = 0;		/* IP encapsulation		*/
	int32	nhdrs = 0;		/* No. of compr. headers	*/
	bool8	compr = 1;		/* Current compression state	*/
	bool8	frag = 0;		/* Fragmentation		*/
	int32	mtu = 128;		/* Max. Transmission Unit	*/
	int32	pktremain;		/* Remaining bytes in pkt	*/
	int32	mturemain;		/* Remaining bytes in MTU	*/
	int32	iplen;			/* Length of entire IP pkt	*/
	byte	*tmptr;			/* Temp. byte pointer		*/

	/* If the packet is not IPv6, return */

	if((pkt->net_ipvtch & 0xf0) != 0x60) {
		return SYSERR;
	}

	/* Copy the radio header */

	memcpy(&cpkt, pkt, 23);

	cstart = cpkt.net_raddata;
	ustart = (byte *)&pkt->net_ipvtch;
	cptr = cstart;
	uptr = (byte *)&pkt->net_ipvtch;
	encsrc = pkt->net_radsrc;
	encdst = pkt->net_raddst;

	iplen = IP_HDR_LEN + ntohs(pkt->net_iplen);

	while(compr) { /* Still compressing */

		switch(nh) {

		 case IP_IPV6: /* Compress an IPv6 header */

		  kprintf("compressing IPv6\n");

		  chdrs[nhdrs] = cptr;
		  uhdrs[nhdrs] = uptr;
		  nhs[nhdrs] = nh;
		  nhdrs++;

		  ipptr = (struct iphdr *)uptr;

		  if(ipenc) {
			  *cptr = 0xEE; cptr++;
		  }

		  iphc = (struct lp_iphc *)cptr;
		  cptr += 2;

		  iphc->iphc_disp = LP_DISP_IPHC;

		  iphc->iphc_tf = 3;

		  if(ipptr->ipnh == IP_HBH ||
		     ipptr->ipnh == IP_RT ||
		     ipptr->ipnh == IP_FRAG ||
		     ipptr->ipnh == IP_DSTOP ||
		     ipptr->ipnh == IP_UDP) {
			  iphc->iphc_nh = 1;
			  nh = ipptr->ipnh;
		  }
		  else {
			  iphc->iphc_nh = 0;
			  *cptr = ipptr->ipnh; cptr++;
			  compr = 0;
		  }

		  if(ipptr->iphl == 1) {
			  iphc->iphc_hl = 1;
		  }
		  else if(ipptr->iphl == 64) {
			  iphc->iphc_hl = 2;
		  }
		  else if(ipptr->iphl == 255) {
			  iphc->iphc_hl = 3;
		  }
		  else {
			  iphc->iphc_hl = 0;
			  *cptr = ipptr->iphl; cptr++;
		  }

		  iphc->iphc_cid = 0;
		  iphc->iphc_sac = 0;

		  if(isipll(ipptr->ipsrc)) {
			  if(!memcmp(&ipptr->ipsrc[8], encsrc, 8)) {
				  iphc->iphc_sam = 3;
			  }
			  else {
				  iphc->iphc_sam = 1;
				  memcpy(cptr, &ipptr->ipsrc[8], 8);
				  cptr += 8;
			  }
		  }
		  else {
			  iphc->iphc_sam = 0;
			  memcpy(cptr, ipptr->ipsrc, 16);
			  cptr += 16;
		  }

		  if(isipmc(ipptr->ipdst)) {
		  }
		  else {
			  if(isipll(ipptr->ipdst)) {
				  if(!memcmp(&ipptr->ipdst[8], encdst, 8)) {
					  iphc->iphc_dam = 3;
				  }
				  else {
					  iphc->iphc_dam = 1;
				  	  memcpy(cptr, &ipptr->ipdst[8], 8);
					  cptr += 8;
				  }
			  }
			  else {
				  memcpy(cptr, ipptr->ipdst, 16);
				  cptr += 16;
			  }
		  }

		  ipenc = 1;
		  encsrc = &ipptr->ipsrc[8];
		  encdst = &ipptr->ipdst[8];
		  *cptr = 0;
		  uptr += IP_HDR_LEN;
		  break;

		 case IP_DSTOP:
		  *cptr = (*cptr) + 1;
		 case IP_FRAG:
		  *cptr = (*cptr) + 1;
		 case IP_RT:
		  *cptr = (*cptr) + 1;
		 case IP_HBH:

		  *cptr = (*cptr) << 1;
		  *cptr |= 0xE0;

		  chdrs[nhdrs] = cptr;
		  uhdrs[nhdrs] = uptr;
		  nhs[nhdrs] = nh;
		  nhdrs++;

		  if((*uptr) == IP_HBH ||
		     (*uptr) == IP_RT ||
		     (*uptr) == IP_FRAG ||
		     (*uptr) == IP_DSTOP ||
		     (*uptr) == IP_IPV6 ||
		     (*uptr) == IP_UDP) {
			  nh = *uptr;
			  *cptr |= 0x01; cptr++;
		  }
		  else {
			  cptr++;
			  *cptr = *uptr; cptr++;
			  compr = 0;
		  }
		  uptr++;

		  *cptr = (*uptr)*8 + 6; cptr++;
		  memcpy(cptr, uptr+1, (*uptr)*8 + 6);
		  cptr += (*uptr)*8 + 6;
		  uptr += (*uptr)*8 + 7;
		  break;
		}

		if((cptr-cstart) > mtu) {
			while((cptr-cstart) > (mtu-4)) {
				nhdrs--;
				cptr = chdrs[nhdrs-1];
				uptr = uhdrs[nhdrs-1];
				nh = nhs[nhdrs-1];
				if(nh == IP_IPV6) {
					iphc = (struct lp_iphc *)cptr;
					iphc->iphc_nh = 0;
					tmptr = chdrs[nhdrs];
					while(tmptr >= (cptr+2)) {
						*(tmptr + 1) = *tmptr;
						tmptr--;
					}
				}
				else {
					*cptr &= 0xFE;
					tmptr = chdrs[nhdrs];
					while(tmptr >= (cptr+1)) {
						*(tmptr + 1) = *tmptr;
						tmptr--;
					}
				}
			}
			frag = 1;
			compr = 0;
		}
	}

	pktremain = iplen - (uptr-ustart);

	if(frag == 0) {
		mturemain = mtu - (cptr-cstart);
		if(pktremain <= mturemain) {
			memcpy(cptr, uptr, pktremain);
			pktlog(&cpkt, 23 + (cptr-cstart));
			return OK;
		}
	}

	tmptr = cptr;
	while(tmptr >= cstart) {
		*(tmptr + 4) = *tmptr;
		tmptr--;
	}
	f1 = (struct lp_frag1 *)cstart;
	f1->frag1_disp = LP_DISP_FRAG1;
	f1->frag1_dtag = 0;
	f1->frag1_dsize = iplen;
	f1->frag1 = htonl(f1->frag1);

	cptr += 4;
	mturemain = mtu - (cptr-cstart);
	while(mturemain >= 8) {
		memcpy(cptr, uptr, 8);
		cptr += 8;
		uptr += 8;
		mturemain -= 8;
	}

	pktlog(&cpkt, 23 + (cptr-cstart));

	while(1) {

		cstart = cptr = cpkt.net_raddata;
		fn = (struct lp_fragn *)cstart;
		fn->fragn_disp = LP_DISP_FRAGN;
		fn->fragn_dtag = 0;
		fn->fragn_dsize = iplen;
		fn->fragn_doff = (uptr-ustart)/8;
		fn->fragn = htonl(fn->fragn);

		pktremain = iplen - (uptr-ustart);
		mturemain = mtu - 5;
		cptr += 5;

		if(pktremain <= mturemain) {
			memcpy(cptr, uptr, pktremain);
			pktlog(&cpkt, 23 + (cptr-cstart));
			return OK;
		}

		while(mturemain >= 8) {
			memcpy(cptr, uptr, 8);
			cptr += 8;
			uptr += 8;
			mturemain -= 8;
		}

		pktlog(&cpkt, 23 + (cptr-cstart));
	}

	return OK;
}
