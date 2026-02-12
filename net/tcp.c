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

void tcp_open(char *name, char *addr_port) { // addr_port should be a string in the form "destination address:port"
    intmask mask;
    mask = disable();
    uint16 locport;
    uint32 remip;
    uint16 remport;
    int32 slot;
    struct tcpentry *tcptr;

    mask = disable();

    // Parse the addr_port string to extract remote IP and port
    if (sscanf(addr_port, "%u:%hu", &remip, &remport) != 2) {
        restore(mask);
        return; // Invalid format
    }

    // Find a free slot in the TCP table
    for (slot = 0; slot < TCP_SLOTS; slot++) {
        tcptr = &tcptab[slot];
        if (tcptr->tcp_state == TCP_FREE) {
            tcptr->tcp_state = TCP_USED;
            tcptr->tcpremip = remip;
            tcptr->tcpremport = remport;
            tcptr->tcplocport = 0; // Assign a local port as needed
            restore(mask);
            return;
        }
    }
    enable(mask);
}