/*
 * File:   graph.c
 * Author: andres
 *
 * Created on May 31, 2010, 12:40 PM
 */

#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include "macros.h"
#include "graph.h"



/**
 * Initialize the graph data structure
 */
void list_graph_init(list_graph* this, int size, int id){
    this->size = 0;
    this->grow = 10;
    this->unused = 0;
    this->vertex = NULL;
    this->wgt = 0;
    this->id = id;
    this->ncolors = 0;
    this->max_degree = 0;
    list_graph_grow(this, size);
}

/**
 * Create the edges x->y and y->x
 */
void list_graph_add_edge(list_graph* this, int adjx, int adjy, int wgt, int constraint){
    if (wgt > 0 || constraint){
		vertex_add_edge(&(this->vertex[adjx]), adjy, wgt);
		//vertex_add_edge(&(this->vertex[adjy]), adjx, wgt);

		this->vertex[adjx].degree++;
		//this->vertex[adjy].degree++;
		this->wgt += wgt;

		if(this->vertex[adjx].degree > this->max_degree)
			this->max_degree = this->vertex[adjx].degree;

		//if(this->vertex[adjy].degree > this->max_degree)
			//this->max_degree = this->vertex[adjy].degree;
    }
    if(constraint){
    	list_insert(&(this->vertex[adjx].constraints), adjy);
    	list_insert(&(this->vertex[adjy].constraints), adjx);
    }
}

/**
 * Delete all edges that link vertexes x and y
 */
void list_graph_unlink(list_graph* this, int adjx, int adjy){
	vertex_unlink(&(this->vertex[adjx]), adjy);
	vertex_unlink(&(this->vertex[adjy]), adjx);
}

/**
 * Grow the node list - equivalent to batch node add.
 */
void list_graph_grow(list_graph* this, int increment){


    if (this->unused >= increment){
        this->unused -= increment;
    }
    else{

        //Calculate a multiple of this->grow that is able to fit size
        int grow_size = (increment/this->grow + 1)*this->grow;

        this->vertex = (vertex*) realloc(this->vertex, sizeof(vertex)*grow_size);
        this->unused = this->unused + grow_size - increment;

        //Zero-out the new block of memory
        void* begin = this->vertex + this->size;
        memset(begin, 0x0, sizeof(vertex)*grow_size);

    }

    this->size += increment;
}

/**
 * Grow the node list with an increment of 1
 */
void list_graph_add_vertex(list_graph* this){
    list_graph_grow(this, 1);
}

/**
 * Set the link variable of a vertex
 */
void list_graph_set_link(list_graph* this, int id, int link){
	this->vertex[id].link = link;
}

/**
 * Return the weight of an edge connecting two vertexes
 * returns 0 if they are disconnected
 */
int list_graph_get_wgt(list_graph* this, int vx, int vy){
	edge* adjl = this->vertex[vx].edge_list;

	while (adjl != NULL){
		if (adjl->id == vy)
			return adjl->wgt;
		adjl = adjl->next;
	}

	return 0;
}

/**
 * Add an edge to a vertex
 */
int vertex_add_edge(vertex* this, int adjy, int wgt){
	edge_add(&(this->edge_list), adjy, wgt);
}

/**
 * Remove all edges that link to vertex adjy
 */
int vertex_unlink(vertex* this, int adjy){
	edge_delete_all(&(this->edge_list), adjy);
}

/**
 * Allocate Space for an edge_node and insert it at the front of the list
 */
int edge_add(edge** head, int id, int wgt){

    //Allocate
    edge* new_node = (edge*) malloc(sizeof(edge));

    //Populate
    new_node->id = id;
    new_node->wgt = wgt;

    //Insertion at the front
    if((*head) == NULL){
    	new_node->next = (*head);
    	*head = new_node;
    	return 0;
    }
    else if(wgt >= (*head)->wgt){
    	new_node->next = (*head);
    	*head = new_node;
    	return 0;
    }
    
    //Search for a spot
    edge* tmp = *head;
    edge* prev = *head;
    while(tmp){
    	if (wgt > tmp->wgt){
    		prev->next = new_node;
    		new_node->next = tmp;
    		return 0;
    	}
    	prev = tmp;
    	tmp = tmp->next;
    }

    //Insert at the end
    prev->next = new_node;

    return 0;
}

/**
 * Delete the first instance of an edge connecting to vertex id
 *
 * returns number of edges deleted - (maximum 1)
 */
int edge_delete(edge** head, int id){

	assert(head);

	edge* tmp = (*head)->next;
    edge* prev = *head;

	//Check the first node
	if ((*head)->id == id){
		free(*head);
		*head = prev;
		return 1;
	}

    while (tmp != NULL){
            if (tmp->id == id){

				#if DEBUG > 0
					//Check
					assert(tmp);
					assert(prev);
				#endif

                //Delete
				prev->next = tmp->next;
                free(tmp);

                //Done
                return 1;
            }

            prev = tmp;
            tmp = tmp->next;
    }
    return 0;
}

/**
 * Delete all instances of edges connecting to vertex id
 *
 * returns number of edges deleted
 */
int edge_delete_all(edge** head, int id){

    int count = 0;
	assert(head);

	edge* tmp = *head;
    edge* prev = *head;

    while (tmp != NULL){
    	if (tmp->id == id){


    		if(prev == tmp){
    			*head = tmp->next;
    			prev = *head;
    			free(tmp);
    			tmp = *head;
    		}
			else{
				prev->next = tmp->next;
				free(tmp);
				prev = tmp;
				tmp = tmp->next;
			}

    		count++;

    	}
    	else{
    		prev = tmp;
    		tmp = tmp->next;
    	}

    }

    return count;
}

/**
 * Print an Edge Node list.
 * Refer to Macros.h to edit the formatting.
 */
void edge_print(edge* head){

    while (head != NULL){

    	printf(FEDGE, head->id, head->wgt);
        head = head->next;

    }
}

/**
 * Print a List Graph.
 * Refer to Macros.h to edit the formatting.
 */
void list_graph_print(list_graph* this){

    int i = 0;
    for (i=0; i < this->size; i++){

    	printf(FVERTEX, i);
    	edge_print((this->vertex[i]).edge_list);
    	PRINT("\n");

    }
}

void list_graph_print_degree(list_graph* this){

    int i = 0;
    for (i=0; i < this->size; i++){

    	printf("[%d]:[%d]", i, this->vertex[i].degree);
    	PRINT("\n");

    }
}

