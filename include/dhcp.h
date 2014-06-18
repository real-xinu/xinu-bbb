/* dhcp.h - Definitions related to DHCP */

#define DHCP

#define DHCP_RETRY 	5

#pragma pack(2)
struct	dhcpmsg	{
	byte	dc_bop;			/* DHCP bootp op 1=req 2=reply	*/
	byte	dc_htype;		/* DHCP hardware type		*/
	byte	dc_hlen;		/* DHCP hardware address length	*/
	byte	dc_hops;		/* DHCP hop count		*/
	uint32	dc_xid;			/* DHCP xid			*/
	uint16	dc_secs;		/* DHCP seconds			*/
	uint16	dc_flags;		/* DHCP flags			*/
	uint32	dc_cip;			/* DHCP client IP address	*/
	uint32	dc_yip;			/* DHCP your IP address		*/
	uint32	dc_sip;			/* DHCP server IP address	*/
	uint32	dc_gip;			/* DHCP gateway IP address	*/
	byte	dc_chaddr[16];		/* DHCP client hardware address	*/
	byte	dc_bootp[192];		/* DHCP bootp area (zero)	*/
	uint32	dc_cookie;		/* DHCP cookie			*/
	byte	dc_opt[1024];		/* DHCP options area (large	*/
					/*  enough to hold more than	*/
					/*  reasonable options		*/
};
#pragma pack()

extern	bpid32	netbufpool;		/* ID of net packet buffer pool	*/
