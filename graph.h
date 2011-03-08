/* 
 * File:   graph.h
 * Author: rodri145
 *
 * Created on May 31, 2010, 12:50 PM
 */

#ifndef _GRAPH_H
#define	_GRAPH_H

#include "list.h"

/**
 * The graph data structures. Consists of an array of adjacency lists.
 */
typedef struct edge_node_t {
    int id;
    int wgt;
    struct edge_node_t* next;
}edge;

typedef struct vertex_t {
	int color;
	int partition;
	int degree;
	int link;
	int seed;
	list constraints;
	edge* edge_list;
}vertex;

typedef struct list_graph_t {
	int id;
    int size;
    int wgt;
    int grow;
    int unused;
    int ncolors;
    int max_degree;
    vertex* vertex;
}list_graph;

/******************************************
 * List Graph related functions are here  *
 ******************************************/

/**
 * Initialize the graph data structure
 */
void list_graph_init(list_graph* this, int size, int id);

/**
 * Create the edges to connect nodes x and y of weight wgt
 */
void list_graph_add_edge(list_graph* this, int adjx, int adjy, int wgt, int constraint);

/**
 * Grow the node list - equivalent to batch node add.
 */
void list_graph_grow(list_graph* this, int increment);

/**
 * Grow the node list with an increment of 1
 */
void list_graph_add_vertex(list_graph* this);

/**
 * Set the link variable of a vertex
 */
void list_graph_set_link(list_graph* this, int id, int link);

/**
 * Delete all edges that link vertexes x and y
 */
void list_graph_unlink(list_graph* this, int adjx, int adjy);

/**
 * Return the weight of an edge connecting two vertexes
 * returns 0 if they are disconnected
 */
int list_graph_get_wgt(list_graph* this, int vx, int vy);

/**
 * Print a List Graph.
 * Refer to Macros.h to edit the formatting.
 */
void list_graph_print(list_graph* this);

/******************************************
 * Vertex related functions are here      *
 *                                        *
 * Consider these mostly helper functions *
 * for List Graph                         *
 ******************************************/

/**
 * Add an edge to a vertex
 */
int vertex_add_edge(vertex* this, int adjy, int wgt);

/**
 * Remove all edges that link to vertex adjy
 */
int vertex_unlink(vertex* this, int adjy);

/******************************************
 * Edge related functions are here        *
 *                                        *
 * Consider these mostly helper functions *
 * for List Graph                         *
 ******************************************/

/**
 * Allocate Space for an edge_node and insert it at the front of the list
 */
int edge_add(edge** head, int id, int wgt);

/**
 * Delete the first instance of an edge linking to vertex id
 */
int edge_delete(edge** head, int id);

/**
 * Delete all instances of edges connecting to vertex id
 *
 * returns number of edges deleted
 */
int edge_delete_all(edge** head, int id);

/**
 * Print an Edge Node list.
 * Refer to Macros.h to edit the formatting.
 */
void edge_print(edge* head);


/******************************************
 * 			Some old stuff      		  *
 ******************************************/

/**
 * Keep this in case I want to use the csr data structure later on. This is the
 * data structure that METIS uses.
 */

typedef struct csr_graph_t {
    /*
     * Data
     */
    int vertices;
    int edges;
    int* index;
    int* adjcy;
    int* wgt;

}csr_graph;

#endif	/* _GRAPH_H */

