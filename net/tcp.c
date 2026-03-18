/* tcp.c tcp_init, tcp_open*/

#include <xinu.h>

struct tcpentry tcptab[TCP_SLOTS]; /* Table of TCP endpoints */

/*------------------------------------------------------------------------
 * tcp_init  -  Initialize all entries in the TCP endpoint table
 *------------------------------------------------------------------------
 */

void    tcp_init(void)
{
   int32 i = 0;

   for (i = 0; i < TCP_SLOTS; i++) {
       tcptab[i].tcp_state = TCP_FREE;
   }
}

devcall tcp_open(struct dentry *devptr, char *name, char *addr_port) {

    // addr_port should be a string in the form "locport:destip:destport"

    intmask mask;
    uint16 locport;
    uint32 remip;
    uint16 remport;
    int32 slot;
    struct tcpentry *tcptr;

    mask = disable();

    // Parse the addr_port string to extract remote IP and port
    if (sscanf(addr_port, "%hu:%u:%hu", &locport, &remip, &remport) != 2) {
        restore(mask);
        kprintf("Invalid format for TCP open");
        return; // Invalid format
    }

    // Find a free slot in the TCP table
    for (slot = 0; slot < TCP_SLOTS; slot++) {
        tcptr = &tcptab[slot];

        if (tcptr->tcp_state == TCP_HANDSHAKE) { // todo: set SEQ and send SYN for 3-way handshake
            tcptr->tcp_state = TCP_OPEN;
            tcptr->tcpremip = remip;
            tcptr->tcpremport = remport;
            tcptr->tcplocport = locport;
            tcptr->tcplocip = NetData.ipucast;
            tcptr->retrans_time = TCP_RETRANSMISSION;
            tcptr->tcp_lar = 0;
            tcptr->tcp_lfs = 0;
            tcptr->tcp_nfe = 0;
            tcptr->tcp_lfa = 0;
            tcptr->tcpisem = semcreate(TCP_QSIZ); // can receive
            tcptr->tcposem = semcreate(0); // nothing to send
            tcp_send_packet(tcptr, NULL, 5); // length is in 32-bit words

            tcp_receive_packet(tcptr); // should be blocking

            tcp_send_packet(tcptr, NULL, 5); // finish handshake

            restore(mask);
            return;
        }
    }

    kprintf("No free slots in TCP table");
    restore(mask);
    return SYSERR;
}

void tcp_send_packet(struct tcpentry *tcptr, char *buff, uint32 len) {
    intmask mask;
    struct netpacket *pkt;
    int32 pktlen
    uint32 seq;
    uint32 ack;
    uint32 remip;
    uint16 remport;
    uint16 locport;
    uint32 locip;

    disable(mask);

    if (tcptr == NULL) {
        kprintf("Invalid pointer to TCP table!\n");
        restore(mask);
        return SYSERR;
    }

    if (tcptr->tcp_state != TCP_FREE) {
        kprintf("TCP entry not allocated!\n");
        restore(mask);
        return SYSERR;
    }

    locip = NetData.ipucast;

    pkt = (struct netpacket *) getbuf(netbufpool); //

    if ((int32) pkt == SYSERR) {
        kprintf("Invalid buffer pool!\n");
        restore(mask);
        return SYSERR;
    }

    pktlen = ((char *)&pkt->tcpdata - (char *) pkt) + len;

    /* TODO: add logic for acocounting for the additional options */

    memcpy(char *pkt->net_ethsrc, NetData.ethucast, ETH_ADDR_LEN);
    pkt->net_ethtype = 0x0800;
    pkt->net_ipvh = 0x45;
    pkt->net_iptos = 0x00;
    pkt->net_iplen = pktlen - ETH_HDR_LEN; // eth encapsulates ip
    pkt->net_ipid = ident++;
    pkt->net_ipfrag = 0x0000;
    pkt->net_ipttl = 0xff;
    pkt->net_ipproto = IP_TCP;
    pkt->net_ipcksum = 0x000;
    pkt->net_ipsrc = locip;
    pkt->net_ipdst = tptr->tcpremip;
    pkt->net_tcpsport = tcptr->tcplocport;
    pkt->net_tcpdport = tcptr->tcpremport;
    pkt->net_tcpseqnum = htonl(seq); // TODO: implement seq-ack with lfs and lar, etc. -> rand.c is in lib directory
    pkt->net_tcpacknum = htonl(ack);
    pkt->net_tcplen = 5;
    pkt->net_tcp_wnd = htons(TCP_BUF_SIZE);
    if (tcptr->tcp_state == TCP_SYN) {
        pkt->net_tcpflags = TCP_SYN;
    } else if (tcptr->tcp_state == TCP_ESTABLISHED) {
        pkt->net_tcpflags = TCP_ACK;
    }
    pkt->net_tcpcksum = 0x000; // TODO: implement checksum calculation
    pkt->net_tcp_urgptr = 0x0000;
    pkt->net_tcp_opts = 0x0000;
    if (buff) {
        memcpy(pkt->tcpdata, buff, len); // optional payload
    }
    tcptr->seq = tcptr->seq + sizeof(struct netpacket); // revisit this later
    tcptr->tcp_state = TCP_LISTEN;

    ip_send(pkt);
    restore(mask);
    return OK;
}

int32 tcp_receive_packet(struct tcpentry *tcptr, char *buff, int32 len) {
    intmask mask;
    struct  netpacket *pkt;
    int32 pktlen;
    static uint16 ident  = 1;
    char *tcpdataptr

    mask = disable();

    if (tcptr == NULL) {
        kprintf("Invalid TCP table entry!\n");
        restore(mask);
        return SYSERR
    }

    if (tcptr->tcpstate != TCP_LISTEN) {
        kprintf("Invalid TCP entry state!\n");
        restore(mask);
        return SYSERR;
    }

    if (tcptr->tcp_lfr == 0) {

    }

}
