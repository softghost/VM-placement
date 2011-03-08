/*
 * cmd.c
 *
 *  Created on: Jun 9, 2010
 *      Author: rodri145
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "macros.h"
#include "graph.h"
#include "cmd_host.h"
#include "partition.h"
#include <errno.h>

#define LISTENPORT 5454
#define BACKLOG 10

#define MAXGRAPHS 10
list_graph graphs[MAXGRAPHS];
int graph_counter = 0;
pthread_mutex_t m_maxgraph;
pthread_mutex_t m_graphs[MAXGRAPHS];

/**
 * Entry point for Server Listen Thread
 */
void listen_entry_point(){

	//Create the socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0){
		perror("ERROR opening socket");
		exit(1);
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(LISTENPORT);
	memset(&server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));

	//Bind
	if(bind(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0){
		perror("ERROR binding socket");
		exit(1);
	}

	//Listen
	if(listen(sockfd, BACKLOG) < 0){
		perror("ERROR on listen");
		exit(1);
	}

	//Accept and spawn handler thread
	int acceptfd, listen = 1;

	while(listen){

		//block until a new connection is established
		acceptfd = accept(sockfd, NULL, NULL);
		if (acceptfd < 0){
			printf("failed to connect\n");
			continue;
		}

		//spawn a worker thread
		pthread_t* newthread = (pthread_t*) malloc(sizeof(pthread_t));
		pthread_create(newthread, 0, cmd_worker_thread, acceptfd);
		pthread_detach(*newthread);
	}

}

/**
 * Entry point for a graph worker thread
 */
void* cmd_worker_thread(void* acceptfd){

	int fd = (int) acceptfd;
	char buf[256] = {0};

	int n = recv(acceptfd,buf,255, 0);
	if (n < 0){
		printf("ERROR reading from socket");
		return 0;
	}

    int num_partitions;
    list_graph traffic;
    list_graph constraints;

    cli_load_file(buf, &traffic, &constraints);
    int* seeds = coloring_get_seeds(&constraints, &traffic, &num_partitions);

    partition* p;
    p = greedyBFS_partition(&traffic, seeds, num_partitions);

    printf("sending reply");

	if (send(acceptfd, &num_partitions, sizeof(int), 0) == -1){
		printf("errno %d\n", errno);
	}

    int i = 0;
    for( i = 0; i < num_partitions; i++){
    	int * array = (int*) malloc(sizeof(int)*(p[i].size + 1));
    	partition_to_array(&p[i], array);

    	if (send(acceptfd, array, sizeof(int)*(p[i].size + 1), 0) == -1){
    		printf("errno %d\n", errno);
    	}

    	free(array);

    	//Wait to send the next one
    	int ACK;
    	if (recv(acceptfd,&ACK,sizeof(int), 0) < 0)
    		printf("ERROR reading from socket");

    	printf(".");
    }

    printf("done\n");

	return 0;
}


