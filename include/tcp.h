/* tcp.h */
#define TCP_HTTP_PORT 80
#define TCP_HTTPS_PORT 443
#define TCP_SSH_PORT 22
#define TCP_DNS_PORT 53
#define TCP_LIFETIME 10000
#define TCP_QSIZ 100

#define TCP_SLOTS 10
#define TCP_FREE 0
#define TCP_USED 1
#define TCP_LISTEN 2
#define TCP_SYN_SENT 3
#define TCP_SYN_RECEIVED 4
#define TCP_ESTABLISHED 5
#define TCP_FIN_WAIT_1 6
#define TCP_FIN_WAIT_2 7
#define TCP_CLOSE_WAIT 8
#define TCP_CLOSING 9
#define TCP_LAST_ACK 10
#define TCP_TIME_WAIT 11

#define MAX_BUFFER_LEN 65536
#define MAX_HDR_SIZ 60

struct tcppacket {
    uint32 srcip;
    uint32 dstip;
    char tcpbuf[MAX_BUFFER_LEN];
    byte ipvh;
    uint16 tcpid;
    uint16 tcpsport;
    uint16 tcpdport;
    uint32 seqnum;
    uint32 acknum;
    uint16 tcplen;
    uint16 tcpcksum;
    byte tcpflags;
};

struct tcpentry {
    uint32 ipmask;
    uint32 tcp_state;
    uint32 tcpremip;
    uint32 tcpremport;
    uint32 tcplocport;
    uint32 tcp_lar; // last ACK received (sender)
    uint32 tcp_lfs; // last frame sent (sender)
    uint32 tcp_nfe; // next frame expected (receiver)
    uint32 tcp_lfa; // last frame ACK (receiver)
    uint32 tcpcount;
    uint32 retrans_time; /* time to wait before retransmitting packet */
    uint32 time_to_live; /* max time before giving up */
    pid32 tcppid;
    struct netpacket *tcpsqueue[TCP_QSIZ]; // sliding window
    struct netpacket *tcprqueue[TCP_QSIZ];
    struct netpacket last_pkt; /* used to store the last packet sent for retransmission */
};

extern struct tcpentry tcptab[]; /* to be initialized in system */