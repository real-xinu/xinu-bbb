/* pktlog.c - pktlog */

#include <xinu.h>

/* Structure of Ethernet packet */

#pragma pack(1)
struct	pkt {
	byte	ethdst[6];
	byte	ethsrc[6];
	uint16	ethtype;
	byte	ipvh;
	byte	iptos;
	uint16	iplen;
	uint16	ipid;
	uint16	ipfrag;
	byte	ipttl;
	byte	ipproto;
	uint16	ipcksum;
	uint32	ipsrc;
	uint32	ipdst;
	uint16	udpsport;
	uint16	udpdport;
	uint16	udplen;
	uint16	udpcksum;
	byte	data[1500-20-8];
};
#pragma pack()

byte	SERVETH[] = 	{0x90, 0xE2, 0xBA, 0x64, 0xF7, 0x55};
#define	SERVIP		0x800a871f
#define	SERVPORT	9000

/*------------------------------------------------------------------------
 * pktlog  -  Log a packet with the packet log server
 *------------------------------------------------------------------------
 */
void	pktlog (
		void	*pktbuf,
		int32	pktlen
	       )
{
	struct	pkt p;
	uint16	*ptr16, cksum;
	uint32	sum;
	int	i;

	memcpy(p.ethdst, SERVETH, 6);
	memcpy(p.ethsrc, ethertab[0].devAddress, 6);
	p.ethtype = htons(0x0800);
	p.ipvh = 0x45;
	p.iptos = 0;
	p.iplen = 20 + 8 + pktlen;
	p.iplen = htons(p.iplen);
	p.ipid = 0;
	p.ipfrag = 0;
	p.ipttl = 255;
	p.ipproto = 17;
	p.ipcksum = 0;
	p.ipsrc = htonl(0x800a8965);
	p.ipdst = htonl(SERVIP);
	p.udpsport = htons(9001);
	p.udpdport = htons(SERVPORT);
	p.udplen = 8 + pktlen;
	p.udplen = htons(p.udplen);
	p.udpcksum = 0;
	memcpy(p.data, pktbuf, pktlen);

	sum = 0;
	ptr16 = (uint16 *)&p.ipvh;
	for(i = 0; i < 10; i++) {
		sum += htons(*ptr16);
		ptr16++;
	}

	cksum = (uint16)(sum & 0xffff) + (uint16)(sum >> 16);
	cksum = ~cksum;
	kprintf("ipcksum %4x %4x\n", cksum, ipcksum(&p));
	p.ipcksum = htons(cksum);

	pdump((struct netpacket *)&p);

	write(ETHER0, (char *)&p, 14 + 20 + 8 + pktlen);
}
