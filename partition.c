/*
 * Partition.c
 *
 *  Created on: Jun 14, 2010
 *      Author: rodri145
 */

#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include "macros.h"
#include "graph.h"
#include "partition.h"


/**
 * Initialize a partition data structure
 */
void partition_init(partition* this, int graph, int id){
	this->graph = graph;
	this->id = id;
	this->wgt = 0;
	this->size = 0;
	this->next = NULL;
}

/**
 * Add a new id to a partition
 */
void partition_add(partition* this, int id, int wgt){
	partition_node_add(&(this->next), id);
	this->wgt += wgt;
	this->size++;
}

/**
 * Allocate a new partition_node and place it at the front of the linked list
 */
void partition_node_add(partition_node** head, int id){

    //Allocate
	partition_node* new_node = (partition_node*) malloc(sizeof(partition_node));

    //Populate
    new_node->id = id;

    if(head)
    	new_node->next = (*head);

    //Insert
    *head = new_node;

}

/**
 * Add a list of constraints to the partition's blacklist
 */
void partition_constraint_add(partition* this, list* l){
	int pop = list_pop(l);
	while (pop != -1){
		list_push(&(this->constraints), pop);
		pop = list_pop(l);
	}
}

/**
 * Print the partition data structure.
 *
 * To edit the format refer to macros.h
 */
void partition_print(partition* this){

	PRINT("\n");

	printf(FPARTITIONHEADER, this->graph, this->id);

	partition_node* tmp = this->next;

	if (tmp != NULL){
		printf(FPARTITION1, tmp->id);
		tmp = tmp->next;
	}

	while(tmp != NULL){
		printf(FPARTITION2, tmp->id);
		tmp = tmp->next;
	}

	PRINT("\n");
}

/**
 * Print the partition data structure.
 *
 * To edit the format refer to macros.h
 */
void partition_to_array(partition* this, int* array){


	partition_node* tmp = this->next;

	array[0] = this->size;

	int i = 1;
	for(i = 0; i < this->size; i++){
		array[i+1] = tmp->id;
		tmp = tmp->next;
	}

}


