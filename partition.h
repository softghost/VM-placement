/*
 * partition.h
 *
 *  Created on: Jun 14, 2010
 *      Author: rodri145
 */

#ifndef PARTITION_H_
#define PARTITION_H_

/******************************************
 * 		The Partition data structures     *
 ******************************************/

typedef struct partition_t {
	int graph;
    int id;
    int wgt;
    int size;
    list constraints;
    struct partition_node_t* next;
}partition;

typedef struct partition_node_t {
	int id;
	struct partition_node_t* next;
}partition_node;

/******************************************
 * 		The Partition functions           *
 ******************************************/

/**
 * Initialize a partition data structure
 */
void partition_init(partition* this, int graph, int id);

/**
 * Add a new id to a partition
 */
void partition_add(partition* this, int id, int wgt);

/**
 * Allocate a new partition_node and place it at the front of the linked list
 */
void partition_node_add(partition_node** this, int id);

/**
 * Add a list of constraints to the partitions list
 */
void partition_constraint_add(partition* this, list* l);
/**
 * Print the partition data structure.
 *
 * To edit the format refer to macros.h
 */
void partition_print(partition* this);

#endif /* PARTITION_H_ */
