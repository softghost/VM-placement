/*
 * main.c
 *
 *  Created on: Jul 19, 2010
 *      Author: rodri145
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>

#define LISTENPORT 5454

void error(char *msg)
{
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[])
{
	int sockfd, n;
	struct hostent *server;

	if (argc < 3) {
       fprintf(stderr,"usage tester hostname filename\n");
       exit(0);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");

	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(LISTENPORT);
	memset(&serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));
	memmove((void*)&serv_addr.sin_addr.s_addr, (void*)server->h_addr, server->h_length);


	if (connect(sockfd, &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR connecting");

	n = send(sockfd,argv[2],strlen(argv[2]), 0);
	if (n < 0)
		 error("ERROR writing to socket");

	int buffer[256] = {0};
	n = recv(sockfd,buffer, sizeof(int), 0);
	if (n < 0)
		error("ERROR reading from socket");
	int pnum = buffer[0];

	printf("%d partitions created\n\n", pnum);

	int i, j;
	for (i = 0; i < pnum; i++){
		n = recv(sockfd,buffer,255, 0);
		if (n < 0)
			error("ERROR reading from socket");
		printf("Partition %d:\n", i);
		for (j = 0; j < buffer[0]; j++){
			printf("%d->", buffer[j + 1]);
		}
		printf("\n");

		if (send(sockfd, buffer, sizeof(int), 0) < 0){
			printf("errno \n");
		}
	}

	return 0;
}
