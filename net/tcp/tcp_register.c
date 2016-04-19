/* tcpmopen.c  -  checktuple, tcpmopen */

#include <xinu.h>
#include <string.h>

#if 1
#define DEBUG(x) (x)
#else
#define DEBUG(x)
#endif

/*------------------------------------------------------------------------
 *  ckecktuple  -  Verify that a TCP connection is not already in use by
 *			checking (src IP, src port, Dst IP, Dst port)
 *------------------------------------------------------------------------
 */
static	int32	checktuple (
		  uint32	lip,	/* Local IP address		*/
		  uint16	lport,	/* Local TCP port number	*/
		  uint32	rip,	/* Remote IP address		*/
		  uint16	rport	/* Remote TCP port number	*/
		)
{
	int32	i;			/* Walks though TCBs		*/
	struct	tcb *tcbptr;		/* Ptr to TCB entry 		*/

	/* Check each TCB that is open */

	for (i = 0; i < Ntcp; i++) {
		tcbptr = &tcbtab[i];
		if (tcbptr->tcb_state != TCB_FREE
		    && tcbptr->tcb_lip == lip
		    && tcbptr->tcb_rip == rip
		    && tcbptr->tcb_lport == lport
		    && tcbptr->tcb_rport == rport) {
			return SYSERR;
		}
	}
	return OK;
}

/*------------------------------------------------------------------------
 *  tcp_register  -  Open a master the TCP device and receive the ID of a
 *			TCP slave device for the connection
 *------------------------------------------------------------------------
 */
int32	tcp_register (
	uint32	ip,
	uint16	port,
	int32	active
	)
{
	// char		*spec;		/* IP:port as a string		*/
	struct tcb	*tcbptr;	/* Ptr to TCB			*/
	uint32		lip;		/* Local IP address		*/
	int32		i;		/* Walks through TCBs		*/
	int32		state;		/* Connection state		*/
	int32		slot;		/* Slot in TCB table		*/

	/* Parse "X:machine:port" string and set variables, where	*/
	/*	X	- either 'a' or 'p' for "active" or "passive"	*/
	/*	machine	- an IP address in dotted decimal		*/
	/*	port	- a protocol port number			*/
/*
	spec = (char *)arg1;
	if (tcpparse (spec, &ip, &port, &active) == SYSERR) {
		return SYSERR;
	}
*/
	/* Obtain exclusive use, find free TCB, and clear it */

	wait (Tcp.tcpmutex);
	for (i = 0; i < Ntcp; i++) {
		if (tcbtab[i].tcb_state == TCB_FREE)
			break;
	}
	if (i >= Ntcp) {
		signal (Tcp.tcpmutex);
		return SYSERR;
	}
	tcbptr = &tcbtab[i];
	tcbclear (tcbptr);
	slot = i;
	DEBUG(kprintf("\t[TCP: Got free TCB slot %d]\n", slot));

	/* Either set up active or passive endpoint */

	if (active) {
		DEBUG(kprintf("\t[TCP: Creating active connection]\n"));

		/* Obtain local IP address from interface */

		lip = NetData.ipucast;

		/* Allocate receive buffer and initialize ptrs */

		DEBUG(kprintf("\t[TCP: Allocating receive buffer]\n"));
		tcbptr->tcb_rbuf = (char *)getmem (65535);
		if (tcbptr->tcb_rbuf == (char *)SYSERR) {
			DEBUG(kprintf("\t[TCP: Error allocating!]\n"));
			signal (Tcp.tcpmutex);
			return SYSERR;
		}
		tcbptr->tcb_rbsize = 65535;
		tcbptr->tcb_rbdata = tcbptr->tcb_rbuf;
		tcbptr->tcb_rbend = tcbptr->tcb_rbuf + tcbptr->tcb_rbsize;


		DEBUG(kprintf("\t[TCP: Allocating sender buffer]\n"));
		tcbptr->tcb_sbuf = (char *)getmem (65535);
		if (tcbptr->tcb_sbuf == (char *)SYSERR) {
			DEBUG(kprintf("\t[TCP: Error allocating!]\n"));
			/* Need to free the *receive* buffer */
			freemem ((char *)tcbptr->tcb_rbuf, 65535);
			signal (Tcp.tcpmutex);
			return SYSERR;
		}
		tcbptr->tcb_sbsize = 65535;

		/* The following will always succeed because	*/
		/*   the iteration covers at least Ntcp+1 port	*/
		/*   numbers and there are only Ntcp slots	*/

		DEBUG(kprintf("\t[TCP: finding usable local port]\n"));
		for (i = 0; i < Ntcp + 1; i++) {
			if (checktuple (lip, Tcp.tcpnextport,
					      ip, port) == OK) {
				break;
			}
			Tcp.tcpnextport++;
			if (Tcp.tcpnextport > 63000)
				Tcp.tcpnextport = 33000;
		}
		DEBUG(kprintf("\t[TCP: calculation complete]\n"));

		tcbptr->tcb_lip = lip;

		/* Assign next local port */
		tcbptr->tcb_lport = Tcp.tcpnextport++;
		DEBUG(kprintf("\t[TCP: local port is %d]\n", tcbptr->tcb_lport));

		if (Tcp.tcpnextport > 63000) {
			Tcp.tcpnextport = 33000;
		}

		DEBUG(kprintf("\t[TCP: finishing TCB setup]\n"));
		tcbptr->tcb_rip = ip;
		tcbptr->tcb_rport = port;
		tcbptr->tcb_snext = tcbptr->tcb_suna = tcbptr->tcb_ssyn = 1;
		tcbptr->tcb_state = TCB_SYNSENT;
		wait (tcbptr->tcb_mutex);
		DEBUG_TCBREF(tcbptr, "tcp_register active before signal");
		tcbref (tcbptr);
		signal (Tcp.tcpmutex);

		DEBUG_TCBREF(tcbptr, "tcp_register active after signal");
		tcbref (tcbptr);
		DEBUG(kprintf("\t[TCP: Sending SYN]\n"));
		mqsend (Tcp.tcpcmdq, TCBCMD(tcbptr, TCBC_SEND));
		DEBUG(kprintf("\t[TCP: mqsend completed]\n"));
		while (tcbptr->tcb_state != TCB_CLOSED
		       && tcbptr->tcb_state != TCB_ESTD) {
			tcbptr->tcb_readers++;
			signal (tcbptr->tcb_mutex);
			wait (tcbptr->tcb_rblock);
			wait (tcbptr->tcb_mutex);
		}
		if ((state = tcbptr->tcb_state) == TCB_CLOSED) {
			DEBUG_TCBUNREF(tcbptr, "tcp_register active closed after wait");
			tcbunref (tcbptr);
		}
		signal (tcbptr->tcb_mutex);
		DEBUG(kprintf("\t[TCP: State is %d]\n", state));
		return (state == TCB_ESTD ? slot : SYSERR);

	} else {  /* Passive connection */
		DEBUG(kprintf("\t[TCP: Creating passive slot]\n"));

		for (i = 0; i < Ntcp; i++) {
			if (tcbtab[i].tcb_state == TCB_LISTEN
			    && tcbtab[i].tcb_lip == ip
			    && tcbtab[i].tcb_lport == port) {

				/* Duplicates prior connection */

				DEBUG(kprintf("\t[TCP: Prior connection duplicated]\n"));
				signal (Tcp.tcpmutex);
				return SYSERR;
			}
		}

		/* New passive endpoint - fill in TCB */

		DEBUG(kprintf("\t[TCP: New endpoint]\n"));
		tcbptr->tcb_lip = ip;
		tcbptr->tcb_lport = port;
		tcbptr->tcb_state = TCB_LISTEN;
		DEBUG_TCBREF(tcbptr, "tcp_register passive");
		tcbref (tcbptr);
		signal (Tcp.tcpmutex);

		/* Return slave device to caller */

		DEBUG(kprintf("\t[TCP: Returning slot %d]\n", slot));
		return slot;
	}
}
