/* xsh_tcpclient.c - xsh_tcpclient */

#include <xinu.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define RECV_MAX 1024
#define SEND_MAX 1024
#define MSG_SIZE 1024

/*------------------------------------------------------------------------
 * xsh_tcpclient - simple client for testing TCP in Xinu
 *------------------------------------------------------------------------
 */
shellcmd xsh_tcpclient_big(int nargs, char *args[])
{
	uint32	ipaddr;
	int32	slot;
	int32	port = 12345;
	char	* rcvbuf = NULL;
	char	* msg = NULL;
	msg = (char *)getmem(MSG_SIZE * sizeof(char));
	int32 i = 0;
	for(i = 0; i < MSG_SIZE; i++)
		msg[i] = 'a';
	msg[i] = '\0';

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

	char * sndbuf = NULL;

	while(1)
	{
		sndbuf = (char *)getmem(SEND_MAX * sizeof(char));
		/* int32 _bread = read(stdin, sndbuf, SEND_MAX);
		fprintf(stderr, "BYTES READ: %d\n", _bread);
		sndbuf[_bread] = '\0'; */
		sprintf(sndbuf, "%s", msg);
		if(!strcmp(sndbuf, "exit"))
		{
			fprintf(stderr, "*******************HERE*******************\n");
			break;
		}
		printf("Sending packet...\n");
		tcp_send(slot, sndbuf, strlen(sndbuf));

		rcvbuf = (char *)getmem(RECV_MAX * sizeof(char));
		printf("Receiving reply: ");
		int32 _bserv = tcp_recv(slot, rcvbuf, RECV_MAX);
		rcvbuf[_bserv] = '\0';
		printf("%s\n", rcvbuf);

		freemem(sndbuf, SEND_MAX * sizeof(char));
		sndbuf = NULL;
		freemem(rcvbuf, RECV_MAX * sizeof(char));
		rcvbuf = NULL;
	}

	printf("Exiting\nClosing slot... ");
	kprintf("%d\n", tcp_close(slot));

	return 0;
}
