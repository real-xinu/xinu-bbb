/* net.h */

#define NETSTK		8192 		/* Stack size for network setup */
#define NETPRIO		500    		/* Network startup priority 	*/
#define NETBOOTFILE	128		/* Size of the netboot filename	*/

/* Constants used in the networking code */

#define	ETH_ARP     0x0806		/* Ethernet type for ARP	*/
#define	ETH_IP      0x0800		/* Ethernet type for IP		*/
#define	ETH_IPv6    0x86DD		/* Ethernet type for IPv6	*/

#define	RAD_BCN		0x0	/* Radio beacon frame		*/
#define	RAD_DATA	0x1	/* Radio data frame		*/
#define	RAD_ACK		0x2	/* Radio ack. frame		*/
#define	RAD_MAC		0x3	/* Radio MAC cmd frame		*/
#define	RAD_LLDN	0x4	/* Radio LLDN frame		*/
#define	RAD_MULT	0x5	/* Radio multipurpose frame	*/

#define	RAD_AM0		0x0
#define	RAD_AM1		0x1
#define	RAD_AM2		0x2
#define	RAD_AM3		0x3

#pragma pack(1)
struct	netpacket_r {
	uint16	net_radftype:3;		/* Radio frame type	*/
	uint16	net_radsec:1;		/* Radio security	*/
	uint16	net_radfp:1;		/* Frame pending	*/
	uint16	net_radar:1;		/* Acknowledgement req	*/
	uint16	net_radpidc:1;		/* Pan ID compression	*/
	uint16	net_radres:1;		/* Reserved		*/
	uint16	net_radseqsup:1;	/* Seq. num. suppression*/
	uint16	net_radie:1;		/* IE List present	*/
	uint16	net_raddam:2;		/* Dest. Address Mode	*/
	uint16	net_radfver:2;		/* Radio frame version	*/
	uint16	net_radsam:2;		/* Src. Address Mode	*/
	byte	net_radseq;		/* Sequence number	*/
	byte	net_radsrcpan[2];	/* Src. PAN ID		*/
	byte	net_radsrc[RAD_ADDR_LEN];/* Src. Address	*/
	byte	net_raddstpan[2];	/* Dest. PAN ID		*/
	byte	net_raddst[RAD_ADDR_LEN];/* Dst. Address	*/
	uint32	net_ipfl:20;		/* IP Flow label	*/
	uint32	net_iptc:8;		/* IP traffic class	*/
	uint32	net_ipver:4;		/* IP version		*/
	uint16	net_iplen;		/* IP payload length	*/
	byte	net_ipnh;		/* IP next header	*/
	byte	net_iphl;		/* IP hop limit		*/
	byte	net_ipsrc[16];		/* IP src. address	*/
	byte	net_ipdst[16];		/* IP dst. address	*/
	union {
	 struct {
	  uint16	net_udpsport;	/* UDP source port	*/
	  uint16	net_udpdport;	/* UDP dest port	*/
	  uint16	net_udplen;	/* UDP header+data len	*/
	  uint16	net_udpcksum;	/* UDP checksum		*/
	  uint16	net_udpdata[1280-23-40-8];
	 };
	 struct {
	  byte		net_ictype;	/* ICMP type		*/
	  byte		net_iccode;	/* ICMP code		*/
	  uint16	net_iccksum;	/* ICMP checksum	*/
	  byte		net_icdata[1280-23-40-4];
	 };
	};
};
#pragma pack()

/* Format of an Ethernet packet carrying IPv4 and UDP */

#pragma pack(1)
struct	netpacket	{
	byte	net_ethdst[ETH_ADDR_LEN];/* Ethernet dest. MAC address	*/
	byte	net_ethsrc[ETH_ADDR_LEN];/* Ethernet source MAC address	*/
	uint16	net_ethtype;		/* Ethernet type field		*/
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
