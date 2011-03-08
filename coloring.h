/*
 * coloring.h
 *
 *  Created on: Jul 5, 2010
 *      Author: rodri145
 */

#ifndef COLORING_H_
#define COLORING_H_

/******************************************
 * 		Entry points for external use	  *
 ******************************************/

/**
 * Greedy graph coloring approximation. The upper bound on colors
 * is max_degree + 1.
 */
int coloring_greedy(list_graph* graph);

/**
 * Pick the seeds according to the following criteria:
 * 1. How many edges exist outside the constraint subgraph
 * 2. Traffic aware decisions
 */
int* coloring_get_seeds(list_graph* constraints, list_graph* traffic, int* num_colors);

/******************************************
 * 		Internal helper functions		  *
 ******************************************/

/**
 * Create a list of vertices sorted by degree. High to low.
 */
list* sort_degree(list_graph* graph);

/**
 * Create a list of vertices sorted by incidence. High to low.
 */
list* sort_influence(list_graph* constraints, list_graph* traffic);

/**
 * Color a single vertex according to neighbor colors
 */
int color_vertex(list_graph* graph, int id);

/**
 * Print the colors of a graph to stdout
 */
void coloring_print(list_graph* graph);

/**
 *
 */
int adjacent_seed_wgt(list_graph* graph, int v);
#endif /* COLORING_H_ */
