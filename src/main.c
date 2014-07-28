#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include "core/bitstring.h"
#include "core/memory.h"

extern unsigned int bs_dimension;
extern unsigned int sdm_sample;

int tcp_listen();

int main(void) {
	struct sdm_memory mem[3];

	printf("Sparse Distributed Memory\n");
	printf("-------------------------\n");
	printf("Dimensions: %u bits\n", bs_dimension);
	printf("Sample: %u hard-locations\n", sdm_sample); 
	printf("Lookup table: %d bits\n", BS_TABLE_SIZE);
	printf("\n");

	printf("Initializing SDM... ");
	sdm_initialize(&mem[0]);
	printf("ok\n");
	printf("\n");

	return tcp_listen(11235);
}

int handle_connection(int fd, struct sockaddr_in cli_addr, socklen_t cli_len) {
	char buf[1000];
	int n;

	while(1) {
		n = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *) &cli_addr, &cli_len);
		if (n < 0) {
			perror("ERROR recvfrom");
			return 1;

		} else if (n == 0) {
			// connection closed
			break;
		}

		n = sendto(fd, buf, n, 0, (struct sockaddr *) &cli_addr, sizeof(cli_addr));
		if (n < 0) {
			perror("ERROR sendto");
			return 1;
		}
	}

	return 0;
}

/**
 * Listen and wait for TCP connections.
 **/
int tcp_listen(int port) {
	int fd, conn, ret;
	struct sockaddr_in srv_addr, cli_addr;
	socklen_t cli_len;
	pid_t pid;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		perror("ERROR opening socket");
		exit(1);
	}

	bzero(&srv_addr, sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	srv_addr.sin_port = htons(port);
	if (bind(fd, (struct sockaddr *) &srv_addr, sizeof(srv_addr)) < 0) {
		perror("ERROR on binding");
		exit(1);
	}

	listen(fd, 1024);
	printf("Listening on port %d\n", port);

	while(1) {
		cli_len = sizeof(cli_addr);
		conn = accept(fd, (struct sockaddr *) &cli_addr, &cli_len);

		printf("[%d] New connection :)\n", conn);
		fflush(stdout);

		pid = fork();
		if (pid < 0) {
			perror("ERROR on fork");

		} else if (pid > 0) {
			// parent process
			close(conn);

		} else {
			// child process
			close(fd);
			ret = handle_connection(conn, cli_addr, cli_len);
			printf("[%d][%d] Connection closed. :(\n", fd, pid);
			fflush(stdout);
			return ret;
		}
	}

	return 0;
}

