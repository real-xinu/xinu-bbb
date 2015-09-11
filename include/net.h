/* net.h */

#define NETSTK		8192 		/* Stack size for network setup */
#define NETPRIO		500    		/* Network startup priority 	*/
#define NETBOOTFILE	128		/* Size of the netboot filename	*/

/* Constants used in the networking code */

#define	ETH_ARP     0x0806		/* Ethernet type for ARP	*/
#define	ETH_IP      0x0800		/* Ethernet type for IP		*/
#define	ETH_IPv6    0x86DD		/* Ethernet type for IPv6	*/

#define	RAD_BEACON  0x0			/* Radio Beacon Frame		*/
#define	RAD_DATA    0x1			/* Radio Data Frame		*/
#define	RAD_ACK	    0x2			/* Radio Acknowledgement Frame	*/
#define	RAD_MACCOM  0x3			/* Radio MAC Command Frame	*/

#define	RAD_AM0    0x0			/* PAN ID, Address not present	*/
#define	RAD_AM2    0x2			/* Short Address		*/
#define	RAD_AM3    0x3			/* Extended Address		*/

/* Format of an Ethernet / Radio packet carrying IPv6 and UDP */

#pragma pack(1)
struct	netpacket	{
	union {
	 struct {	/* Ethernet header	*/
	  byte		net_ethdst[ETH_ADDR_LEN];/* Ethernet dest. MAC address	*/
	  byte		net_ethsrc[ETH_ADDR_LEN];/* Ethernet source MAC address	*/
	  uint16	net_ethtype;		/* Ethernet type field		*/
	 };
	 struct	{	/* IEEE 802.15.4 header	*/
	  byte		net_radftype:3;		/* Frame Type		*/
	  byte		net_radsec:1;		/* Security enabled	*/
	  byte		net_radfp:1;		/* Frame pending	*/
	  byte		net_radar:1;
	  byte		net_radpidc:1;		/* PAN ID compression	*/
	  byte		net_radres1:1;		/* Reserved bit		*/
	  byte		net_radres2:2;		/* Reserved bits	*/
	  byte		net_raddam:2;		/* Dest. Addr. Mode	*/
	  byte		net_radfvers:2;		/* Frame version	*/
	  byte		net_radsam:2;		/* Src. Addr. Mode	*/
	  byte		net_radseq;		/* Sequence number	*/
	  uint16	net_raddpan;		/* Dst. PAN Id		*/
	  byte		net_raddst[8];		/* Dest. address	*/
	  uint16	net_radspan;		/* Src. PAN Id		*/
	  byte		net_radsrc[8];		/* Src. address		*/
	 };
	};
	byte	net_ipvh;		/* IP version and hdr length	*/
	byte	net_iptos;		/* IP type of service		*/
	uint16	net_iplen;		/* IP total packet length	*/
	uint16	net_ipid;		/* IP datagram ID		*/
	uint16	net_ipfrag;		/* IP flags & fragment offset	*/
	byte	net_ipttl;		/* IP time-to-live		*/
	byte	net_ipproto;		/* IP protocol (actually type)	*/
	uint16	net_ipcksum;		/* IP checksum			*/
	uint32	net_ipsrc;		/* IP source address		*/
	uint32	net_ipdst;		/* IP destination address	*/
	union {
	 struct {
	  uint16 	net_udpsport;	/* UDP source protocol port	*/
	  uint16	net_udpdport;	/* UDP destination protocol port*/
	  uint16	net_udplen;	/* UDP total length		*/
	  uint16	net_udpcksum;	/* UDP checksum			*/
	  byte		net_udpdata[1500-28];/* UDP payload (1500-above)*/
	 };
	 struct {
	  byte		net_ictype;	/* ICMP message type		*/
	  byte		net_iccode;	/* ICMP code field (0 for ping)	*/
	  uint16	net_iccksum;	/* ICMP message checksum	*/
	  uint16	net_icident; 	/* ICMP identifier		*/
	  uint16	net_icseq;	/* ICMP sequence number		*/
	  byte		net_icdata[1500-28];/* ICMP payload (1500-above)*/
	 };
	};
};
#pragma pack()

#define	PACKLEN	sizeof(struct netpacket)

extern	bpid32	netbufpool;		/* ID of net packet buffer pool	*/

struct	network	{
	uint32	ipucast;
	uint32	ipbcast;
	uint32	ipmask;
	uint32	ipprefix;
	uint32	iprouter;
	uint32	bootserver;
	bool8	ipvalid;
	byte	ethucast[ETH_ADDR_LEN];
	byte	ethbcast[ETH_ADDR_LEN];
	char	bootfile[NETBOOTFILE];
};

extern	struct	network NetData;	/* Local Network Interface	*/
