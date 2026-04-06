/* tcp.c tcp_init, tcp_open*/


/*
 * -------------------------------------------------------------------------------------------------
 * High Level: each connection request opens up a pseudo device, which is opened from the
 * TCP main device. Initially each entry in tcptab is free. Once a device is opened, we
 * undergo the regular 3-way handshake in order to establish a connection.
 *
 * Handling incoming packets: packets are received from ip_in and forwarded to tcp_in. We then
 * identify which entry in the table is associated with our packet, and then call tcp_receive_packet
 * accordingly, filling up our SWS and updating LFA and NFE (we are receiver). Then we send a packet
 * using tcp_send_packet with just the ack
 *
 * Sneding packets: simply allocate space for a netpacket struct, fill in fields such as source and
 * destination address, flags, seq/ack. Sends the packet over using ip_send to maintain encapsulation
 * practices.
 * --------------------------------------------------------------------------------------------------
 */

#include <complex.h>
#include <netinet/in.h>
#include <xinu.h>
#include <stdlib.h>

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

/*------------------------------------------------------------------------
 * tcp_open  -  open a TCP pseudo device that creates one half of a TCP connection
 * if expecting recvfrom version, set i/o to 2 and destip:destport to 0:0
 *------------------------------------------------------------------------
 */

devcall tcp_open(struct dentry *devptr, char *name, char *addr_port) {

    // addr_port should be a string in the form "locport:destip:destport:i/o"

    intmask mask;
    uint16 locport; // how do you ensure the ports are not already in use?
    uint32 remip;
    uint16 remport;
    uint16 mode;
    int32 slot;
    struct tcpentry *tcptr;

    mask = disable();

    // Parse the addr_port string to extract remote IP and port
    if (sscanf(addr_port, "%hu:%u:%hu:%hu", &locport, &remip, &remport, &mode) != 2) {
        restore(mask);
        kprintf("Invalid format for TCP open");
        return; // Invalid format
    }

    // Find a free slot in the TCP table
    for (slot = 0; slot < TCP_SLOTS; slot++) {
        tcptr = &tcptab[slot];

        if (tcptr->tcp_state == TCP_FREE) { // TODO: set SEQ and send SYN for 3-way handshake
            tcptr->tcp_state = TCP_HANDSHAKE;
            // we handle the entire handshake here so don't need other intermediary states
            tcptr->tcpremip = remip;
            tcptr->tcpremport = remport;
            tcptr->tcplocport = locport;
            tcptr->tcplocip = NetData.ipucast;
            tcptr->retrans_time = TCP_RETRANSMISSION; // default retransmission timeout -> will have to
            tcptr->tcp_lar = 0; // implement as pointers
            tcptr->tcp_lfs = 0;
            tcptr->tcp_nfe = 0;
            tcptr->tcp_lfa = 0;
            tcptr->tcp_wnd = TCP_MTU;
            tcptr->tcphead = 0;
            tcptr->tcptail = 0;
            if (mode == TCP_RECVFROM) {
                // tcptr->tcpisem = semcreate(TCP_QSIZ); // can receive
                // tcptr->tcposem = semcreate(0); // nothing to send
                tcp_send_packet(tcptr, NULL, 5); // length is in 32-bit words
                // tcptr->tcp_state = TCP_SYN_SENT;

                tcp_receive_packet(tcptr); // should be blocking
                // tcptr->tcp_state = TCP_SYN_ACK_RECEIVED

                tcp_send_packet(tcptr, NULL, 5); // finish handshake
                // tcptr->tcp_state = TCP_ACK_RECEIVED;

                restore(mask);
                return;
            } else {
                tcp_recvfrom(tcptr);
                // tcptr->tcp_state = TCP_SYN_RECEIVED;
                tcp_send_packet(tcptr, NULL, 5);
                // tcptr->tcp_state = TCP_ACK_RECEIVED;

                restore(mask);
                return;
            }
        }
    }

    kprintf("No free slots in TCP table");
    restore(mask);
    return SYSERR;
}


/*------------------------------------------------------------------------
 * tcp_send_packet - send a TCP packet through connection associated with
 * tptr device
 *------------------------------------------------------------------------
 */

devcall tcp_send(devptr *dentry , char *buff, uint32 len) {
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

    if (tcptr->tcp_state != TCP_FREE && tcptr->tcp_state != TCP_OPEN) {
        kprintf("TCP entry not allocated!\n");
        restore(mask);
        return SYSERR;
    }

    locip = NetData.ipucast;

    pkt = (struct netpacket *) getbuf(netbufpool);

    if ((int32) pkt == SYSERR) {
        kprintf("Invalid buffer pool!\n");
        restore(mask);
        return SYSERR;
    }

    pktlen = ((char *)&pkt->tcpdata - (char *) pkt) + len;

    /* TODO: add logic for acocounting for the additional options */

    if (tcptr->tcp_state == TCP_HANDSHAKE) {
        seq = rand() % UINT32_MAX;
    } else {
        seq = tcptr->seq
    }

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
    pkt->net_ipsrc = htonl(locip);
    pkt->net_ipdst = htonl(tptr->tcpremip);
    pkt->net_tcpsport = htons(tcptr->tcplocport);
    pkt->net_tcpdport = htons(tcptr->tcpremport);
    pkt->net_tcpseqnum = htonl(seq); // TODO: implement seq-ack with lfs and lar, etc. -> rand.c is in lib directory
    pkt->net_tcpacknum = htonl(seq + 1);
    pkt->net_tcplen = 5; // one byte field so no endianness reordering necessary
    pkt->net_tcp_wnd = htons(tcptr->tcp_wnd);
    if (tcptr->tcp_state == TCP_HANDSHAKE) { // we assume TCP_HANDSHAKE with no other flags means we are handshaking
        pkt->net_tcpflags |= TCP_SYN;
    } else if (tcptr->tcp_state == TCP_ESTABLISHED) {
        pkt->net_tcpflags = htonl(TCP_ACK);
    }
    pkt->net_tcpcksum = 0x000; // TODO: implement checksum calculation
    pkt->net_tcp_urgptr = 0x0000;
    pkt->net_tcp_opts = 0x0000;
    if (buff) {
        memcpy(pkt->tcpdata, buff, len); // optional payload
    }
    tcptr->seq = tcptr->seq + sizeof(struct netpacket); // revisit this later
    tcptr->tcp_state |= TCP_LISTEN;

    tcptr->ack = tcptr->seq + 1;
    tcptr->seq = tcptr->seq + pktlen;


    ip_send(pkt);
    restore(mask);
    return OK;
}

/*
 * tcp_in - handle incoming TCP packet by associating with a device.
 * If no device matches, packet gets dropped and SYSERR returns (maybe panic).
 */

void tcp_in(struct netpacket *pktptr) {
    intmask mask; /* interrupt mask */
    int32 i; /* index for tcptab */
    struct tcpentr *tcptr; /* pointer to a TCP entry associated with a connection */
    byte pktlen;
    uint32 seq;
    uint32 ack;
    uint16 pktlen;
    char *tcp_dataptr;

    mask = disable();

    pktlen = pktptr->net_iplen;

    // tcp_len is the TCP header length in 32-bit words

    char *tcp_dataptr = pktptr->net_iplen + pktptr->net_tcplen * 4 + IP_HDR_LEN;

    // sanity check so we dont run into memory corruption

    if (pktlen < TCP_HEADER_LEN + IP_HDR_LEN) {
        kprintf("invalid packet length! Less than minimum size for headers!\n");
        freebuf((char *) pktptr);
        restore(mask);
        return;
    }

    // checksum is one's complement sum of 16-bit words

    uint16 num_words = 3 * sizeof(int32) + ((pktlen - IP_HDR_LEN - pktptr->net_tcplen)/16);
    uint16 checksum = 0;

    for (i = 0; i < num_words; i++) {
        checksum += *((int *) tcp_dataptr);
    }

    if (checksum != 0xFFFF) {
        kprintf("checksum calculation failed!\n");
        freebut((char *) pktptr);
        restore(mask);
        return;
    }

    /* only allow one process to access TCP table at a time */

    for (i = 0; i < TCP_SLOTS; i++) {
        tcptr = &tcptab[i];
        if (tcptr->tcp_state == TCP_FREE) {
            continue;
        }

        // ensure metadata is correct OR check that we might be accepting a SYN
        // packet, in which case we would have to populate the remote ip, etc.
        // should not be populating if receiving a syn_ack, though

        if ((pktptr->net_tcpdport == tcptr->tcplocport &&
            pktptr->net_ipdst == tcptr->tcplocip &&
            (pktptr->net_tcpflags & TCP_SYN) &&
            !(pktptr->net_tcpflags & TCP_ACK))) {

            tcptr->tcpremport = pktptr->net_tcpsport;
            tcptr->tcpremip = pktptr->net_ipsrc;
        }

        if (pktptr->net_tcpdport == pktptr->tcplocport &&
                pktptr->tcp_tcpsport == tcptr->tcplocport &&
                pktptr->net_ipsrc == tcptr->tcp_remip &&
                pktptr->net_ipdst == tcptr->tcp_destip) {

            // check packet size

            if (pktlen > (tcptr->tcp_lfa - tcptr->nfe) || pktlen > TCP_MTU) {
                kprintf("TCP RSW not enough space!\n");
                restore(mask);
                return(SYSERR);
            } else {
                // add to receiving packet queue (not sliding window)
                tcptr->tcpcount++;
                tcptr->tcrpqueue[udptr->tcptail++] = pktptr;
                if (tcptr->tcptail >= TCP_QSIZ) {
                    tcptr->tcptail = 0; // circular buff
                }

                // set state
                if () {
                    tcptr->tcp_state =
                        REMOVE_TCP_STATE(tcptr->tcp_state, TCP_SYN_SENT);
                    tcptr->tcp_state |= TCP_SYN_ACK_RECEIVED;
                }
                // recvfrom case
                if (tcptr->tcp_state == TCP_HANDSHAKE) {
                    tcptr->tcp_state =
                        REMOVE_TCP_STATE(tcptr->tcp_state, TCP_HANDSHAKE);
                    tcptr->tcp_state |= TCP_SYN_RECEIVED;
                }
                if (tcptr->tcp_state == TCP_SYN_ACK_SENT) {
                    tcptr->tcp_state =
                        REMOVE_TCP_STATE(tcptr->tcp_state, TCP_SYN_ACK_SENT);
                    tcptr->tcp_state |= TCP_ACK_RECEIVED;
                }
                send(tcptr->tcppid, OK);
            }

            restore(mask);
            return;

        }
        freebuf((char *) pktptr);
        restore(mask);
        return;
    }
}

/*------------------------------------------------------------------------
 * tcp_recv  -  Receive a TCP packet
 *------------------------------------------------------------------------
 */

int32 tcp_recv(struct tcpentry *tcptr, char *buff, int32 len) {
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

    if (tcptr->tcp_state != TCP_SYN_SENT ||
        tcptr->tcp_state != TCP_SYN_ACK_SENT ||
        tcptr->tcp_state != TCP_SEQ_SENT) {
        kprintf("Invalid TCP entry state!\n");
        restore(mask);
        return SYSERR;
    }

    // TODO: implement checksum

    pkt = tcptr->tcptab

}

void tcp_ntoh(struct netpacket *pkt) {
    pkt->net_ipdst = ntohl(pkt->net_ipdst);
    pkt->net_tcpsport = ntohs(pkt->net_tcpsport);
    pkt->net_tcpdport = ntohs(pkt->net_tcpdport);
    pkt->net_tcpseqnum = ntohl(pkt->net_tcpseqnum);
    pkt->net_tcpacknum = ntohl(pkt->net_tcpacknum);
    pkt->net_tcplen = ntohs(pkt->net_tcplen);
    pkt->net_tcp_wnd = ntohs(pkt->net_tcp_wnd);
    pkt->net_tcpflags = ntohl(pkt->net_tcpflags);
    pkt->net_tcpcksum = ntohs(pkt->net_tcpcksum);
    pkt->net_tcp_urgptr = ntohs(pkt->net_tcp_urgptr);
    pkt->net_tcp_opts = ntohs(pkt->net_tcp_opts);
    return;
}

void tcp_hton(struct netpacket *pkt) {
    pkt->net_ipdst = htonl(pkt->net_ipdst);
    pkt->net_tcpsport = htons(pkt->net_tcpsport);
    pkt->net_tcpdport = htons(pkt->net_tcpdport);
    pkt->net_tcpseqnum = htonl(pkt->net_tcpseqnum);
    pkt->net_tcpacknum = htonl(pkt->net_tcpacknum);
    pkt->net_tcplen = htons(pkt->net_tcplen);
    pkt->net_tcp_wnd = htons(pkt->net_tcp_wnd);
    pkt->net_tcpflags = htonl(pkt->net_tcpflags);
    pkt->net_tcpcksum = htons(pkt->net_tcpcksum);
    pkt->net_tcp_urgptr = htons(pkt->net_tcp_urgptr);
    pkt->net_tcp_opts = htons(pkt->net_tcp_opts);
}
