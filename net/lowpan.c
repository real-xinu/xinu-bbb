/* lowpan.c - lp_send */

#include <xinu.h>

byte	ip_llprefix[] = {0xfe, 0x80, 0, 0, 0, 0, 0, 0,
			 0, 0, 0, 0, 0, 0, 0, 0};

/*------------------------------------------------------------------------
 * lp_send  -  Compress and send an IPv6 datagram (RFC 6282 and 4494)
 *------------------------------------------------------------------------
 */
int32	lp_send (
		 struct	netpacket_r *pkt
		)
{
	struct	netpacket_r cpkt;	/* Compressed packet to be sent	*/
	struct	lp_iphc *iphc;		/* Lowpan IPHC header		*/
	struct	lp_frag1 *f1;		/* Lowpan Frag1 header		*/
	struct	lp_fragn *fn;		/* Lowpan FragN header		*/
	struct	iphdr *ipptr;		/* IP header			*/
	struct	udphdr *udpptr;		/* UDP header			*/
	byte	*encsrc, *encdst;	/* Encap. src and dst		*/
	bool8	ipenc;			/* IP encapsulation boolean	*/
	byte	*cstart;		/* Start of compr. packet	*/
	byte	*ustart;		/* Start of uncompr. packet	*/
	byte	*cptr, *uptr;		/* Current compr, uncompr ptrs	*/
	byte	*tmptr;			/* Temporary pointer		*/
	byte	*chdrs[10];		/* Compressed header addresses	*/
	byte	*uhdrs[10];		/* Uncompr. header addresses	*/
	byte	nhs[10];		/* Next header types		*/
	byte	nh;			/* Current header type		*/
	int32	nhdrs;			/* No. of headers compressed	*/
	int32	mtu = 40;		/* Max. Transmission Unit	*/
	int32	pktremain;		/* Remaining bytes in packet	*/
	int32	mturemain;		/* Remaining bytes in MTU	*/
	int32	pktlen;			/* Length of entire IP packet	*/
	bool8	compr;			/* Compression boolean		*/
	bool8	frag;			/* Fragmentation boolean	*/

	/* Verify that the packet is IPv6 */

	if((pkt->net_ipvtch & 0xf0) != 0x60) {
		return SYSERR;
	}

	/* Initialize all the pointers and variables */

	cstart = cptr = cpkt.net_raddata;
	ustart = uptr = &pkt->net_ipvtch;
	pktlen = IP_HDR_LEN + ntohs(pkt->net_iplen);
	encsrc = pkt->net_radsrc;
	encdst = pkt->net_raddst;
	nh = IP_IPV6;
	ipenc = 0;
	compr = 1;
	frag = 0;
	nhdrs = 0;

	/* Copy the radio header */

	memcpy(&cpkt, pkt, 23);

	/* Start compressing */

	while(compr) {

		switch(nh) {

		 case IP_IPV6: /* Compress an IPv6 header */

		  /* Store the start of compressed and uncompressed headers */

		  chdrs[nhdrs] = cptr;
		  uhdrs[nhdrs] = uptr;
		  nhs[nhdrs] = nh;
		  nhdrs++;

		  /* If this header is encapsulate, add a IPNHC byte */

		  if(ipenc == 1) {
			  *cptr = 0xEE;
			  cptr++;
		  }

		  ipptr = (struct iphdr *)uptr;
		  iphc = (struct lp_iphc *)cptr;
		  cptr += 2;

		  /* Set the IPHC dispatch */

		  iphc->iphc_disp = LP_DISP_IPHC;

		  /* Traffic class is elided for now */

		  iphc->iphc_tf = 0x3;

		  /* Encode the next header */

		  if(ipptr->ipnh == IP_HBH ||
		     ipptr->ipnh == IP_RT ||
		     ipptr->ipnh == IP_FRAG ||
		     ipptr->ipnh == IP_DSTOP ||
		     ipptr->ipnh == IP_IPV6 ||
		     ipptr->ipnh == IP_UDP) {
			  nh = ipptr->ipnh;
			  iphc->iphc_nh = 1;
		  }
		  else {
			  compr = 0;
			  *cptr = ipptr->ipnh;
			  cptr++;
		  }

		  /* Encode the hop limit */

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
			  *cptr = ipptr->iphl;
			  cptr++;
		  }

		  /* We do not use contextx for now */

		  iphc->iphc_cid = 0;
		  iphc->iphc_sac = 0;
		  iphc->iphc_dac = 0;

		  /* Encode the IP source address */

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
			  memcpy(cptr, ipptr->ipsrc, 16);
			  cptr += 16;
		  }

		  /* Encode the IP destination address */

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

		  *cptr = 0;
		  ipenc = 1;
		  encsrc = ipptr->ipsrc;
		  encdst = ipptr->ipdst;
		  uptr += IP_HDR_LEN;
		  break;

		 case IP_DSTOP:
		  *cptr += 1;
		 case IP_FRAG:
		  *cptr += 1;
		 case IP_RT:
		  *cptr += 1;
		 case IP_HBH:

		  /* Record the start of compr. and uncompr. headers */

		  chdrs[nhdrs] = cptr;
		  uhdrs[nhdrs] = uptr;
		  nhs[nhdrs] = nh;
		  nhdrs++;

		  /* Set the IPNHC byte */

		  *cptr *= 2;
		  *cptr |= 0xE0;

		  /* Encode the next header */

		  if((*uptr) == IP_HBH ||
		     (*uptr) == IP_RT ||
		     (*uptr) == IP_FRAG ||
		     (*uptr) == IP_DSTOP ||
		     (*uptr) == IP_IPV6 ||
		     (*uptr) == IP_UDP) {
			  nh = *uptr;
			  *cptr |= 0x01;
			  cptr++;
		  }
		  else {
			  compr = 0;
			  cptr++;
			  *cptr = *uptr;
			  cptr++;
		  }
		  uptr++;

		  /* Copy the extension header data */

		  *cptr = (*uptr)*8 + 6;
		  cptr++;
		  memcpy(cptr, (uptr+1), (*uptr)*8 + 6);
		  cptr += (*uptr)*8 + 6;
		  uptr += (*uptr)*8 + 7;
		  break;
		}

		/* If we have exceeded the MTUm start backtracking */

		if((cptr-cstart) > mtu) {
			/* Leave 4 bytes for frag1 header */
			while((cptr-cstart) > (mtu-4)) {
				nhdrs--;
				cptr = chdrs[nhdrs-1];
				uptr = uhdrs[nhdrs-1];
				nh = nhs[nhdrs-1];
				if(nh == IP_IPV6) {
					*cptr = *(cptr+1);
					*(cptr+1) = *(cptr+2);
					*(cptr+2) = nhs[nhdrs];
					((struct lp_iphc *)cptr)->iphc_nh = 0;
					cptr = chdrs[nhdrs];
					uptr = uhdrs[nhdrs];
				}
				else {
					tmptr = chdrs[nhdrs]-1;
					while(tmptr >= (cptr+1)) {
						*(tmptr + 1) = *tmptr;
						tmptr--;
					}
					*cptr &= 0xE0;
					*(cptr + 1) = nhs[nhdrs];
					cptr = chdrs[nhdrs]+1;
					uptr = uhdrs[nhdrs];
				}
			}
			frag = 1;
			compr = 0;
		}
	}

	pktremain = pktlen - (uptr-ustart);

	if(frag == 0) {
		mturemain = mtu - (cptr-cstart);
		if(pktremain <= mturemain) {
			memcpy(cptr, uptr, pktremain);
			cptr += pktremain;
			uptr += pktremain;
			pktlog(&cpkt, 23 + (cptr-cstart));
			return OK;
		}
	}

	/* Make space for the Frag1 header */

	tmptr = cptr;
	while(tmptr >= cstart) {
		*(tmptr + 4) = *tmptr;
		tmptr--;
	}
	cptr += 4;

	f1 = (struct lp_frag1 *)cstart;
	f1->frag1_disp = LP_DISP_FRAG1;
	f1->frag1_dtag = 0;
	f1->frag1_dsize = pktlen;
	f1->frag1 = htonl(f1->frag1);

	/* Copy as much data as possible in the fragment */

	mturemain = mtu - (cptr-cstart);
	while(mturemain >= 8) {
		memcpy(cptr, uptr, 8);
		cptr += 8;
		uptr += 8;
		mturemain -= 8;
	}

	pktlog(&cpkt, 23 + (cptr-cstart));

	while(1) {

		fn = (struct lp_fragn *)cstart;
		fn->fragn_disp = LP_DISP_FRAGN;
		fn->fragn_dtag = 0;
		fn->fragn_dsize = pktlen;
		fn->fragn_doff = (uptr-ustart)/8;
		fn->fragn = htonl(fn->fragn);

		cptr = cstart + 5;

		pktremain = pktlen - (uptr-ustart);
		mturemain = mtu - (cptr-cstart);

		if(pktremain <= mturemain) {
			memcpy(cptr, uptr, pktremain);
			cptr += pktremain;
			uptr += pktremain;
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
