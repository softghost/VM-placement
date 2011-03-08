/*
 * queue.h
 *
 *  Created on: Jun 16, 2010
 *      Author: rodri145
 */

#ifndef _LIST_H
#define	_LIST_H

/*****************************
 * The list data structures *
 *****************************/

/**
 * A node within the queue linked list
 */
typedef struct list_node_t {
	int id;
	int payload;
	struct list_node_t* next;
}list_node;

/**
 * The head list data structure
 */
typedef struct list_t {
	struct list_node_t* head;
	struct list_node_t* tail;
	int size;
}list;


/*****************************
 *     List Operations      *
 *****************************/

/**
 * Initialize the list
 */
void list_init(list* this);

/**
 * Insert node at the end of the list
 */
void list_insert(list* this, int id);

/**
 * Insert node into a sorted list. high to low.
 */
void list_insert_payload(list* this, int id, int payload);

/**
 * Insert node into a sorted list. high to low.
 */
void list_insert_desc(list* this, int id);

/**
 * Insert node into a list sorted by payload. high to low.
 */
void list_insert_desc_payload(list* this, int id, int payload);

/**
 * Insert node at the front of the list
 */
void list_push(list* this, int id);

/**
 * Pop a node from the list
 */
int list_pop(list* this);

/**
 * Pop a node from the list
 */
int list_peek(list* this);

/**
 * Insert node into a sorted list. high to low.
 */
void list_insert_desc(list* this, int id);

/**
 * Peek at the front of the list
 */
int list_peek(list* this);

/**
 * Check if the list contains item id
 */
int list_contains(list* this, int id);

/**
 * Search the list for an id and return the payload
 */
int list_search(list* this, int id);

/**
 * Convert the list to an array
 */
int* list_to_array(list* this);

/**
 * Print the list
 */
void list_print(list* this);

/**
 * Free nodes allocated in a list. Note that this does not free the head list
 * data structure itself
 */
void list_free(list* this);

#endif	/* _LIST_H */

