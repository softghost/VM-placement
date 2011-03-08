/*
 * coloring.c
 *
 *  Created on: Jul 5, 2010
 *      Author: rodri145
 */

#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include "graph.h"
#include "list.h"
#include "coloring.h"

/**
 * Greedy graph coloring approximation. The upper bound on colors
 * is max_degree + 1.
 */
int coloring_greedy(list_graph* graph){

	list* vertex_order = sort_degree(graph);

	int maxcolor = 0;
	int color = 0;

	int next = list_pop(vertex_order);
	do{
		color = color_vertex(graph, next);

		if (color > maxcolor)
			maxcolor = color;

		next = list_pop(vertex_order);
	}while(next != -1);

	list_free(vertex_order);
	free(vertex_order);

	return maxcolor;
}

/**
 * Pick a seed according to the following criteria:
 * 1. How many edges exist outside the constraint subgraph
 * 2. Traffic aware decisions
 */
int* coloring_get_seeds(list_graph* constraints, list_graph* traffic, int* num_colors){

	//Color the graph
	int colors = coloring_greedy(constraints);
	coloring_print(constraints);
	*num_colors = colors;

	//Sort by highest influence
	list* s = sort_influence(constraints, traffic);

	//Get a seed from each color
	int* seeds = (int*) malloc (sizeof(int)*colors);
	memset(seeds, 0xFF, sizeof(int)*(colors));

	int i = 0;
	while (i < colors){
		int tmp = list_pop(s);
		int tmpcolor = constraints->vertex[tmp].color - 1;
		if(seeds[tmpcolor] == -1){
			seeds[tmpcolor] = tmp;
			constraints->vertex[tmp].seed = 1;
			i++;
		}
	}

	//Check for traffic issues between seeds
	int* twgt = (int*) malloc (sizeof(int)*colors);
	memset(twgt, 0x0, sizeof(int)*(colors));

	/**
	 * TODO:improve sorting
	 */
	int j, tmp;
	for (i = 0; i<colors; i++){
		for (j = i+1; j < colors; j++){
			if (i !=  j){
				tmp = list_graph_get_wgt(constraints, i, j);
				twgt[i] += tmp;
				twgt[j] += tmp;
			}
		}
	}

	list trank;
	list_init(&trank);
	for (i = 0; i< colors; i++){
		list_insert_desc_payload(&trank, i, twgt[i]);
	}

	for (i = 0; i< colors; i++){
		int seed = list_pop(&trank);
		int color = seed + 1;
		int current_wgt = adjacent_seed_wgt(constraints, seeds[seed]);

		for (j = 0; j < constraints->size; j++){
			if (constraints->vertex[j].color  == color && \
					constraints->vertex[j].seed == 0){
				int new_wgt = adjacent_seed_wgt(constraints, j);

				if (new_wgt < current_wgt){
					constraints->vertex[seed].seed = 0;
					constraints->vertex[j].seed = 1;
					seeds[seed] = j;
					current_wgt = new_wgt;
				}
			}
		}
	}

	printf("---seeds---\n");
	for ( i = 0; i < colors; i++){
		seeds[i] = constraints->vertex[seeds[i]].link;
		printf("[%d]-[%d]\n", i, seeds[i]);

	}
	printf("---seeds---\n");
	return seeds;
}

int adjacent_seed_wgt(list_graph* graph, int v){
	//Get the edge list
	edge* head  = graph->vertex[v].edge_list;

	int acc = 0;
	while(head){
		if (graph->vertex[head->id].seed)
			acc += head->wgt;
		head = head->next;
	}

	return acc;
}

list* sort_influence(list_graph* constraints, list_graph* traffic){

	int* incidence = (int*) malloc (sizeof(int)*constraints->size);

	list* s = (list*) malloc(sizeof(list));
	list_init(s);

	int i = 0;
	for (i = 0; i < constraints->size; i++){
		int link = constraints->vertex[i].link;
		incidence[i] = traffic->vertex[link].degree - constraints->vertex[i].degree;
		list_insert_desc_payload(s, i, incidence[i]);
	}

	return s;
}

/**
 * Create a list of vertices sorted by degree. High to low.
 */
list* sort_degree(list_graph* graph){

	list* s = (list*) malloc(sizeof(list));
	list_init(s);

	/**
	 * TODO: improve sorting
	 */
	int i, j;
	for (i = 0; i < graph->size; i++){
		for (j = 0; j < graph->size; j++){
			if(graph->vertex[j].degree == i){
				list_push(s, j);
			}
		}
	}

	return s;
}

/**
 * Color a single vertex according to neighbor colors
 */
int color_vertex(list_graph* graph, int id){

	/**
	 * The color 0 is reserved for unvisited vertexes
	 */
	int csize = graph->max_degree + 2;
	int* colors = (int*) malloc (sizeof(int)*(csize));
	memset(colors, 0x0, sizeof(int)*(csize));

	edge* head = graph->vertex[id].edge_list;

	//Check neighbor colors
	while (head != NULL){
		colors[graph->vertex[head->id].color]++;
		head = head->next;
	}

	//Color self
	int i;
	for (i = 1; i < csize; i++){
		if (colors[i] == 0){
			colors[i]++;
			graph->vertex[id].color = i;
			free(colors);
			return i;
		}
	}

	printf("problem coloring\n");
	return -1;
}

/**
 * Print the colors of a graph to stdout
 */
void coloring_print(list_graph* graph){
	printf("---colors---\n");
	int i;
	printf("Graph size: %d\n", graph->size);
	for (i = 0; i < graph->size; i++){
		printf("[%d]-[%d]\n", i , graph->vertex[i].color);
	}
	printf("---colors---\n");
}

/**
 * Check if an array contains a number
 */
int checkArray(int* array, int size, int key){
	int i;
	for (i = 0; i < size; i++){
		if(array[i] == key)
			return 1;
	}

	return 0;
}
