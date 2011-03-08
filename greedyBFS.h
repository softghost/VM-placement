/*
 * greedyBFS.h
 *
 *  Created on: Jun 30, 2010
 *      Author: rodri145
 */

#ifndef GREEDYBFS_H_
#define GREEDYBFS_H_

#include "graph.h"
#include "partition.h"

/**
 * Entry point for the greedyBFS partition routine
 */
partition* greedyBFS_partition(list_graph* graph, int* seeds, int pnum);

/**
 * Create an array of unbalanced partitions according to a distribution vector
 */
partition* greedyBFS_helper(list_graph* graph, int pnum, int* seeds);

/**
 * Expand a partition
 */
list* greedyBFS_grow(list_graph* graph, partition* p, int source, int dest);

/**
 * Initial discovery to populate a list form a seed
 */
void greedyBFS_discovery(list_graph* graph, list* list, partition* p, int seed);

/**
 * Return the tentative choice for growing
 */
int greedyBFS_peek(list_graph* graph, partition* p, int node);

/**
 * Search the graph for fragments and integrate them into partitions
 */
void greedyBFS_fragment_scan(list_graph* graph, partition* parray, int pnum);

#endif /* GREEDYBFS_H_ */
