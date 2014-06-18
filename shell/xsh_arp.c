/* xsh_arp.c - xsh_arp */

#include <xinu.h>
#include <stdio.h>
#include <string.h>

static	void	arp_dmp(int32);
/*------------------------------------------------------------------------
 * xsh_arp - display the current ARP cache for an interface
 *------------------------------------------------------------------------
 */
shellcmd xsh_arp(int nargs, char *args[])
{
	char	ch;			/* character from arg string	*/
	int32	iface;			/* interface number in binary	*/

	/* For argument '--help', emit help about the 'arp' command	*/

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Use: %s\n\n", args[0]);
		printf("Description:\n");
		printf("\tDisplays information from the ARP cache\n");
		printf("Options:\n");
		printf("\t--help\t display this help and exit\n");
		printf("\tiface\t interface number between 0 and 3\n");
		return 0;
	}

	/* If no args, see if at least one interface is alive */

	if (nargs == 1) {

		/* Use prime interface */

		if (ifprime < 0) {
		   fprintf(stderr, "%s: no network is available\n",
								args[0]);
		   return 1;
		}
		iface = -1;

	} else if (nargs == 2) {

		/* Convert interface number to binary */

		ch = *args[1]++;
		if ( (*args[1] != NULLCH) || (ch < '0') || (ch > '9')) {
		   fprintf(stderr,
			    "%s: argument must be an interface number\n",
								args[0]);
		   fprintf(stderr, "Try '%s --help' for more information\n",
								args[0]);
		}
		iface = ch - '0';
	} else {
		fprintf(stderr, "%s: incorrect number of arguments\n",
								args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",
								args[0]);
		return 1;
	}

	/* Either dump the arp table for the specified interface or	*/
	/*	for all interfaces that are up				*/

	printf("\n");
	if (iface >= 0) {
		arp_dmp(iface);
	} else {
		for (iface = 0; iface<NIFACES; iface++) {
			if (if_tab[iface].if_state != IF_UP) {
				continue;
			}
			arp_dmp(iface);
		}
	}
	return 0;
}


/*------------------------------------------------------------------------
 * arp_dmp - an arp table given an inteface number
 *------------------------------------------------------------------------
 */
static	void arp_dmp (
	  int32	iface			/* interface to use		*/
	)
{
	int32	i, j;			/* index into the ARP table	*/
	struct	ifentry  *ifptr;	/* ptr to interface		*/
	struct	arpentry *arptr;	/* pointer to entry in cache	*/


	/*  ARP table for specified interface */

	ifptr = &if_tab[iface];
	
	/* Print entries from the ARP table */

	printf("ARP cache for interface %d:\n", iface);
	printf("   State Pid    IP Address    Hardware Address\n");
	printf("   ----- --- --------------- -----------------\n");
	for (i = 0; i < ARP_SIZ; i++) {
		arptr = &ifptr->if_arptab[i];
		if (arptr->arstate == AR_FREE) {
			continue;
		}
		switch(arptr->arstate) {
		    case AR_PENDING:	printf("   PEND "); break;
		    case AR_RESOLVED:	printf("   RESLV"); break;
		    default:		printf("   ?????"); break;
		}
		if (arptr->arstate == AR_PENDING) {
			printf("%4d ", arptr->arpid);
		} else {
			printf("     ");
		}
		printf("%3d.", (arptr->arpaddr & 0xFF000000) >> 24);
		printf("%3d.", (arptr->arpaddr & 0x00FF0000) >> 16);
		printf("%3d.", (arptr->arpaddr & 0x0000FF00) >> 8);
		printf("%3d",  (arptr->arpaddr & 0x000000FF));

		printf(" %02X", arptr->arhaddr[0]);
		for (j = 1; j < ARP_HALEN; j++) {
			printf(":%02X", arptr->arhaddr[j]);
		}
		printf("\n");
	}
	printf("\n");
	return;
}
