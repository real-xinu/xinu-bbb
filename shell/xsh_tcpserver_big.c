/* xsh_tcpserver.c - xsh_tcpserver */

#include <xinu.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define RECV_MAX 1024
#define SEND_MAX 1024

/*------------------------------------------------------------------------
 * xsh_tcpserver - simple server for testing TCP in Xinu
 *------------------------------------------------------------------------
 */
shellcmd xsh_tcpserver_big(int nargs, char *args[])
{
	int32	slot;
	int32	port = 12345;
	// char	rcvbuf[RECV_MAX] = {0};

	/* Print help information */
	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Use: ");
		printf("%s\n\n", args[0]);
		printf("Description:\n");
		printf("\tStart a TCP server to receive and reply to messages.\n");
		printf("Options:\n");
		printf("\t--help\tdisplay this help and exit\n");

		return 0;
	}

	/* Open passive slot */
	printf("Opening slot... ");
	slot = tcp_register(NetData.ipucast, port, 0);	/* 0 = passive */
	printf("%d\n");

	while(TRUE) 
	{
		/* Get connection from client */
		int32 client;
		if(tcp_recv(slot, (char *) &client, 4) == SYSERR) {
			continue;
		}

		printf("New connection from %x\n", tcbtab[client].tcb_rip);

		process_server(client);

		/* Close client slot */
		printf("\tClosing connection...\n");
		tcp_close(client);
	}

	return 0;
}

void process_server(int32 client)
{

	char * sndbuf = NULL;
	char * rcvbuf = NULL;

	while(1)
	{
		/* Receive message from client */
		rcvbuf = (char *)getmem(RECV_MAX * sizeof(char));
		int32 size = tcp_recv(client, rcvbuf, RECV_MAX);
		rcvbuf[size] = '\0';
		printf("\tGot message: %s SIZE: %d\n", rcvbuf, size);

		/* Send reply to client */
		sndbuf = (char *)getmem(SEND_MAX * sizeof(char));

		if(!strcmp(rcvbuf, "term"))
			sprintf(sndbuf, "****************** OK ******************\n");
		else
			sprintf(sndbuf, "%s", rcvbuf);
		sndbuf[size] = '\0';

		printf("\tReplying with: %s\n", sndbuf);
		tcp_send(client, sndbuf, strlen(sndbuf));

		freemem(sndbuf, SEND_MAX * sizeof(char));
		sndbuf = NULL;

		if(!strcmp(rcvbuf, "term"))
		{
			freemem(rcvbuf, RECV_MAX * sizeof(char));
			rcvbuf = NULL;
			break;
		}

		freemem(rcvbuf, RECV_MAX * sizeof(char));
		rcvbuf = NULL;
	}
}