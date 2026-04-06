/* tcp.h */
#define TCP_HTTP_PORT 80
#define TCP_HTTPS_PORT 443
#define TCP_SSH_PORT 22
#define TCP_DNS_PORT 53
#define TCP_LIFETIME 10000
#define TCP_QSIZ 100
#define TCP_HEADER_LEN 20

/* state of the connection */

#define TCP_SLOTS 10
#define TCP_FREE 0 /* no connection */
#define TCP_SYN_SENT 1 << 3 /* SYN has been sent */
#define TCP_SYN_RECEIVED 1 << 4 /* ACK for SYN received */
#define TCP_ESTABLISHED 1 << 5 /* ACK for SYN_ACK sent */
#define TCP_FIN_WAIT_1 1 << 6
#define TCP_FIN_WAIT_2 1 << 7
#define TCP_CLOSE_WAIT 1 << 8
#define TCP_CLOSING 1 << 9
#define TCP_LAST_ACK 1 << 10
#define TCP_TIME_WAIT 1 << 11
#define TCP_OPEN 1 << 12
#define TCP_HANDSHAKE 1 << 13
#define TCP_SYN_ACK_RECEIVED 1 << 14
#define TCP_SYN_ACK_SENT 1 << 15
#define TCP_ACK_RECEIVED 1 << 16
#define TCP_SEQ_SENT 1 << 17 /* this might be unnecessary since it is SEQ, ACK */

#define REMOVE_TCP_STATE(x,y) (x & ~y) /* remove state y from tcp_state x */
#define ADD_TCP_STATE(x,y) (x | y) /* add state y to tcp_state x */

/* Bitshifts for flags */

#define TCP_RECVFROM 1
#define TCP_SENDTO 2

#define TCP_FIN 1
#define TCP_SYN 1 << 1
#define TCP_RST 1 << 2
#define TCP_ACK 1 << 4

#define TCP_MTU 4096
#define TCP_RETRANSMISSION 500

// struct tcppacket {
//     uint32 srcip;
//     uint32 dstip;
//     char tcpbuf[MAX_BUFFER_LEN];
//     byte ipvh;
//     uint16 tcpid;
//     uint16 tcpsport;
//     uint16 tcpdport;
//     uint32 seqnum;
//     uint32 acknum;
//     uint16 tcplen;
//     uint16 tcpcksum;
//     byte tcpflags;
// };

struct tcpentry {
    uint32 tcp_state;
    uint32 tcpremip;
    uint32 tcpremport;
    uint32 tcplocport;

    /* the following fields are offsets in the window */
    uint32 tcp_lar; // last ACK received (sender)
    uint32 tcp_lfs; // last frame sent (sender)
    uint32 tcp_nfe; // next frame expected (receiver)
    uint32 tcp_lfa; // last frame ACK (receiver)
    uint32 retrans_time; /* time to wait before retransmitting packet (ms) */
    /* window offsets end */

    uint32 seq;
    uint32 ack;
    uint32 seq_init = 0;
    uint32 ack_init = 0;

    // sid32 tcpisem;
    // sid32 tcposem;
    pid32 tcppid;
    uint16 tcp_rwnd; // effective receiving window size
    uint16 tcp_swnd; // effective sending window size
    char tcpsbuf[MAX_BUFFER_LEN];
    char tcprbuf[MAX_BUFFER_LEN];
    struct netpacket *tcpsqueue[TCP_QSIZ]; // sliding window sending
    struct netpacket *tcprqueue[TCP_QSIZ]; // sliding window receiving
};

extern struct tcpentry tcptab[TCP_SLOTS]; /* to be initialized in system */
