/* xsh_tcpclient.c - xsh_tcpclient */

#include <xinu.h>
#include <stdio.h>

#define RECV_MAX 64

/*------------------------------------------------------------------------
 * xsh_tcpclient - simple client for testing TCP in Xinu
 *------------------------------------------------------------------------
 */
shellcmd xsh_tcpclient(int nargs, char *args[])
{
	uint32	ipaddr;
	int32	slot;
	int32	port = 12345;
	char	rcvbuf[RECV_MAX] = {0};

	/* Print help information */
	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Use: ");
		printf("%s SERVERIP\n\n", args[0]);
		printf("Description:\n");
		printf("\tConnects to a TCP server and sends a message.\n");
		printf("Options:\n");
		printf("\tSERVERIP\tthe IP of the server in dotted decimal\n");
		printf("\t--help\tdisplay this help and exit\n");

		return 0;
	}

	/* Make sure we have correct arguments */
	if (nargs != 2) {
		printf("invalid arguments (see %s --help)\n", args[0]);
		return 1;
	}

	/* Parse IP address */
	if(dot2ip(args[1], &ipaddr) == SYSERR) {
		printf("invalid IP address\n");
		return 1;
	}
	printf("Connecting to %s (0x%x)\n", args[1], ipaddr);

	/* Connect to server */
	printf("Registering TCP slot... ");
	slot = tcp_register(ipaddr, port, 1);	/* 1 = active connection */
	printf("got slot %d\n", slot);

	char sndbuf[] = "User";
	printf("Sending packet...\n");
	tcp_send(slot, sndbuf, 4);

	printf("Receiving reply: ");
	tcp_recv(slot, rcvbuf, RECV_MAX);
	printf("%s\n", rcvbuf);

	printf("Closing slot...\n");
	tcp_close(slot);

	return 0;
}
