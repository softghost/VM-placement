/*
 * greedyBFS.c
 *
 *  Created on: Jun 30, 2010
 *      Author: rodri145
 */
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include "macros.h"
#include "graph.h"
#include "partition.h"
#include "list.h"
#include "greedyBFS.h"

/**
 * Entry point for the greedyBFS partition routine
 */
partition* greedyBFS_partition(list_graph* graph, int* seeds, int pnum){

	PRINTLN("----------");
	partition* p = greedyBFS_helper(graph, pnum, seeds);

	int i = 0;
	for (i=0; i < pnum; i++){
		partition_print(&p[i]);
	}

	return p;
}

/**
 * Create an array of unbalanced partitions according to a distribution vector
 */
partition* greedyBFS_helper(list_graph* graph, int pnum, int* seeds){

	//Allocate space for the data structures
	partition* parray = (partition *) malloc(sizeof(partition)*pnum);
	list* dest_list = (list *) malloc(sizeof(list)*pnum);
	list* src_list = (list *) malloc(sizeof(list)*pnum);
	int* src = (int*) malloc(sizeof(int)*pnum);
	int* dest = (int*) malloc(sizeof(int)*pnum);

	//Initialize the data structures and perform the initial discovery process
	int i, tmp;
	int maxtmp = -1;
	int maxi = -1;

	/**
	 * Initialization
	 */
	for ( i = 0; i < pnum; i++){
		//Mark seeds as partitioned
		graph->vertex[seeds[i]].color = 2;

		//Initialize and insert
		partition_init(&parray[i], graph->id, i);
		partition_add(&parray[i], seeds[i], 0);

		//Populate the lists with starting nodes and constraints
		list_init(&src_list[i]);
		list_init(&dest_list[i]);
		partition_constraint_add(&parray[i], &(graph->vertex[seeds[i]].constraints));
		greedyBFS_discovery(graph, &dest_list[i], &parray[i], seeds[i]);

		//Find the best seed to start with
		tmp = greedyBFS_peek(graph, &parray[i], seeds[i]);

		if (tmp > maxtmp){
			maxi = i;
			maxtmp = tmp;
		}

		//Set initial src and dst
		src[i] = seeds[i];
		dest[i] = list_pop(&dest_list[i]);
		list_insert(&src_list[i], dest[i]);
	}

	/**
	 * Partitioning
	 */
	while (maxtmp != -1){

		printf("[%d]->[%d]:[%d]\n", src[maxi], dest[maxi], maxi);
		//Grow the partition
		list* l = greedyBFS_grow(graph, &parray[maxi], src[maxi], dest[maxi]);

		//Insert new data into the queue
		int pop = list_pop(l);
		while (pop != -1){
			list_insert(&dest_list[maxi], pop);
			pop = list_pop(l);
		}

		//update src and dest if needed
		dest[maxi] = list_pop(&dest_list[maxi]);
		if (dest[maxi] >= 0)
			list_insert(&src_list[maxi], dest[maxi]);

		if (dest[maxi] != -1){
			while(greedyBFS_peek(graph, &parray[maxi], src[maxi]) == -1){
				src[maxi] = list_pop(&src_list[maxi]);
			if(src[maxi] == -1)
				break;
			}
		}


		//Find the new best partition and update src and dest if needed
		maxtmp = -1;
		for ( i = 0; i < pnum; i++){

			if (src[i] == -1)
				tmp = -1;
			else
				tmp = greedyBFS_peek(graph, &parray[i], src[i]);

			if (tmp > maxtmp){
				maxi = i;
				maxtmp = tmp;
			}
		}
	}



	/**
	 * Fragments
	 */
	/*int alloc = graph->wgt;
	greedyBFS_grow(graph, &parray[i], *seeds++, alloc);*/

	//greedyBFS_fragment_scan(graph, parray, pnum);

	/**
	 * Cleanup
	 */
	return parray;
}

/**
 * Expand a partition
 */
list* greedyBFS_grow(list_graph* graph, partition* p, int source, int dest){

	//Prepare the queue
	list* q = (list*) malloc ( sizeof(list));
	list_init(q);

	//Get the Adjacency list of the vertex
	if (graph->vertex[dest].color == 2){
		//Someone else already took our node
		return NULL;
	}

	//Check for constraints
	if(list_contains(&(p->constraints), dest)){
		return NULL;
	}

	//Put the node in the partition
	graph->vertex[dest].color = 2;
	partition_add(p, dest, 0);

	//Add the new constraints to the partition
	partition_constraint_add(p, &(graph->vertex[dest].constraints));


	//Check the adjacency list
	edge* head = graph->vertex[dest].edge_list;

	while(head != NULL){

		if(graph->vertex[head->id].color < 2){

			//Color Grey
			graph->vertex[head->id].color = 1;

			//Enqueue
			list_insert(q, head->id);
		}

		head = head->next;
	}
	return q;
}

/**
 * Initial discovery to populate a list form a seed
 */
void greedyBFS_discovery(list_graph* graph, list* list, partition* p, int seed){

	edge* head = graph->vertex[seed].edge_list;

	//Check the adjacency list
	while(head != NULL){

		if(graph->vertex[head->id].color < 2){
			if(!list_contains(&(p->constraints), head->id)){
				//Color Grey
				graph->vertex[head->id].color = 1;

				//Enqueue
				list_insert(list, head->id);
			}
		}

		head = head->next;
	}
}

/**
 * Return the tentative choice for growing
 */
int greedyBFS_peek(list_graph* graph, partition* p, int node){

	edge* head = graph->vertex[node].edge_list;

	while (head){
		if(graph->vertex[head->id].color < 2){
			if(!list_contains(&(p->constraints), head->id))
				return head->wgt;
		}
		head = head->next;
	}

	return -1;
}
/**
 * Search the graph for fragments and integrate them into partitions
 */
void greedyBFS_fragment_scan(list_graph* graph, partition* parray, int pnum){

	int i = 0;

	//Search for unpartitioned nodes
	for (i = 0; i < graph->size; i++){

		if (graph->vertex[i].color == 0){

			edge* head = graph->vertex[i].edge_list;
			int attached = 0;

			//Attempt to find a nearby partition to attach to
			while(head != NULL){
				if (graph->vertex[head->id].color != 0){
					partition_add(&parray[graph->vertex[head->id].partition], i, head->wgt);
					graph->vertex[i].partition = graph->vertex[head->id].partition;
					graph->vertex[i].color = 2;
					attached = 1;
				}

				head = head->next;
			}

			//Place the vertex in the partition with highest available space
			if (!attached){

				int minwgt = parray[0].wgt;
				int sp = 0;
				int j;

				for (j = 1; j < pnum; j++){
					if (parray[j].wgt < minwgt){
						minwgt = parray[i].wgt;
						sp = j;
					}
				}

				partition_add(&parray[sp], i, 0);
				graph->vertex[i].partition = sp;
				graph->vertex[i].color = 2;
			}
		}
	}
}
