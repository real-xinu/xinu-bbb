/* ip.h  -  Constants related to Internet Protocol version 4 (IPv4) */

#define	IP_BCAST	0xffffffff	/* IP local broadcast address	*/
#define	IP_THIS		0xffffffff	/* "this host" src IP address	*/
#define	IP_ALLZEROS	0x00000000	/* The all-zeros IP address     */

#define	IP_ICMP		1		/* ICMP protocol type for IP 	*/
#define	IP_UDP		17		/* UDP protocol type for IP 	*/

#define	IP_ASIZE	16		/* Bytes in an IP address	*/
#define	IP_HDR_LEN	40		/* Bytes in an IP header	*/

#define	IP_HBH		0
#define	IP_RT		43
#define	IP_FRAG		44
#define	IP_DSTOP	60
#define	IP_IPV6		41

#define	isipll(x)	(!memcmp((x), ip_llprefix, 8))
#define	isipmc(x)	(0)
#pragma pack(1)
struct	iphdr {
	byte	ipvtch;
	byte	iptclflh;
	uint16	ipfll;
	uint16	iplen;
	byte	ipnh;
	byte	iphl;
	byte	ipsrc[16];
	byte	ipdst[16];
	byte	ipdata[];
};
#pragma pack()

#define	IP_OQSIZ	8		/* Size of IP output queue	*/

/* Queue of outgoing IP packets waiting for ipout process */

struct	iqentry	{
	int32	iqhead;			/* Index of next packet to send	*/
	int32	iqtail;			/* Index of next free slot	*/
	sid32	iqsem;			/* Semaphore that counts pkts	*/
	struct	netpacket *iqbuf[IP_OQSIZ];/* Circular packet queue	*/
};

extern	struct	iqentry	ipoqueue;	/* Network output queue		*/
