/* pcap.c */

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netdb.h>

#pragma pack(1)
struct	pcaphdr {
	uint32_t	magic_number;
	uint16_t	version_major;
	uint16_t	version_minor;
	int32_t		thiszone;
	uint32_t	sigfigs;
	uint32_t	snaplen;
	uint32_t	network;
};

struct	pkthdr {
	uint32_t	ts_sec;
	uint32_t	ts_usec;
	uint32_t	incl_len;
	uint32_t	orig_len;
};
#pragma pack()

int	main() {

	int	filefd, sockfd;
	struct	sockaddr_in caddr;
	struct	addrinfo hints, *res;
	socklen_t caddrlen;
	char	pktbuf[1280];
	int	recvlen;

	filefd = open("xinucap.pcap", O_RDWR);
	if(filefd == -1) {
		printf("Cannot open pcap file\n");
		return 1;
	}

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd == -1) {
		perror("socket");
		close(filefd);
		return 1;
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, "9000", &hints, &res);

	if(bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
		perror("bind");
		close(filefd);
		close(sockfd);
		return 1;
	}

	struct	pcaphdr pcap;
	struct	pkthdr pkt;

	pcap.magic_number = 0xa1b2c3d4;
	pcap.version_major = 2;
	pcap.version_minor = 4;
	pcap.thiszone = 0;
	pcap.sigfigs = 0;
	pcap.snaplen = 1280;
	pcap.network = 230;

	write(filefd, &pcap, sizeof pcap);

	while(1) {

		caddrlen = sizeof caddr;
		recvlen = recvfrom(sockfd, pktbuf, 1280, 0, (struct sockaddr *)&caddr, &caddrlen);
		if(recvlen == -1) {
			perror("recvfrom");
			close(filefd);
			close(sockfd);
			return 1;
		}

		printf("recvd. %d bytes\n", recvlen);

		pkt.ts_sec = 0;
		pkt.ts_usec = 0;
		pkt.incl_len = recvlen;
		pkt.orig_len = recvlen;

		write(filefd, &pkt, sizeof pkt);
		write(filefd, pktbuf, recvlen);
	}

	close(filefd);
	close(sockfd);

	return 0;
}
