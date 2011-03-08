/*
 * queue.c
 *
 *  Created on: Jun 16, 2010
 *      Author: rodri145
 */

#include <stdio.h>
#include <malloc.h>
#include "list.h"

/**
 * Initialize the list
 */
void list_init(list* this){
	this->head = NULL;
	this->tail = NULL;
	this->size = 0;
}

/**
 * Insert node at the end of the list
 */
void list_insert(list* this, int id){

	//Allocate
	list_node* new_node = (list_node*) malloc(sizeof(list_node));

	//Populate
	new_node->id = id;
	new_node->next = NULL;

	//Insert
	if (this->head == NULL){
		this->head = new_node;
		this->tail = new_node;
	}
	else{
		this->tail->next = new_node;
		this->tail = new_node;
	}

	this->size++;
}

/**
 * Insert node at the end of the list
 */
void list_insert_payload(list* this, int id, int payload){

	//Allocate
	list_node* new_node = (list_node*) malloc(sizeof(list_node));

	//Populate
	new_node->id = id;
	new_node->next = NULL;
	new_node->payload = payload;

	//Insert
	if (this->head == NULL){
		this->head = new_node;
		this->tail = new_node;
	}
	else{
		this->tail->next = new_node;
		this->tail = new_node;
	}

	this->size++;
}

/**
 * Insert node into a sorted list. high to low.
 */
void list_insert_desc(list* this, int id){

	//Allocate
	list_node* new_node = (list_node*) malloc(sizeof(list_node));

	//Populate
	new_node->id = id;
	new_node->next = NULL;

    //Insertion at the front
    if(this->head == NULL){
    	new_node->next = (this->head);
    	this->head = new_node;
    	return;
    }
    else if(id >= this->head->id){
    	new_node->next = (this->head);
    	this->head = new_node;
    	return;
    }

    //Search for a spot
    list_node* tmp = this->head;
    list_node* prev = this->head;
    while(tmp){
    	if (id > tmp->id){
    		prev->next = new_node;
    		new_node->next = tmp;
    		return;
    	}
    	prev = tmp;
    	tmp = tmp->next;
    }

    //Insert at the end
    prev->next = new_node;
    return;
}

/**
 * Insert node into a sorted list. high to low.
 */
void list_insert_desc_payload(list* this, int id, int payload){

	//Allocate
	list_node* new_node = (list_node*) malloc(sizeof(list_node));

	//Populate
	new_node->id = id;
	new_node->next = NULL;
	new_node->payload = payload;

    //Insertion at the front
    if(this->head == NULL){
    	new_node->next = (this->head);
    	this->head = new_node;
    	return;
    }
    else if(payload >= this->head->payload){
    	new_node->next = (this->head);
    	this->head = new_node;
    	return;
    }

    //Search for a spot
    list_node* tmp = this->head;
    list_node* prev = this->head;
    while(tmp){
    	if (payload >= tmp->payload){
    		prev->next = new_node;
    		new_node->next = tmp;
    		return;
    	}
    	prev = tmp;
    	tmp = tmp->next;
    }

    //Insert at the end
    prev->next = new_node;

    return;
}

/**
 * Insert node at the front of the list
 */
void list_push(list* this, int id){

	//Allocate
	list_node* new_node = (list_node*) malloc(sizeof(list_node));

	//Populate
	new_node->id = id;
	new_node->next = this->head;

	//Insert
	this->head = new_node;

	this->size++;
}

/**
 * Pop a node from the list
 */
int list_pop(list* this){

	if (!this)
		return -1;

	int ret = -1;

	if (this->head){
		list_node* node = this->head;
		this->head = node->next;
		ret =  node->id;
		free(node);
	}

	this->size--;

	return ret;
}

/**
 * Peek at the front of the list
 */
int list_peek(list* this){

	if (this->head)
		return  this->head->id;

	return -1;
}

/**
 * Check if the list contains item id
 */
int list_contains(list* this, int id){
	list_node* head = this->head;

	while (head != NULL){
		if(head->id == id)
			return 1;
		head = head->next;
	}
	return 0;
}

/**
 * Search for item id in list and return the payload
 */
int list_search(list* this, int id){
	list_node* head = this->head;

	while (head != NULL){
		if(head->id == id)
			return head->payload;
		head = head->next;
	}
	return -1;
}

/**
 * Convert the list to an array
 */
int* list_to_array(list* this){

	int* array = (int*) malloc (sizeof(int)*this->size);

	int i = 0;
	list_node* tmp = this->head;

	for(i = 0;tmp != NULL; i++){
		array[i] = tmp->id;
		tmp = tmp->next;
	}

	return array;
}

/**
 * Print the list
 */
void list_print(list* this){
	list_node* tmp = this->head;

	while(tmp != NULL){
		printf("->%d", tmp->id);
		tmp = tmp->next;
	}
	printf("\n");
}

/**
 * Free nodes allocated in a list. Note that this does not free the head list
 * data structure itself
 */
void list_free(list* this){

	while(this->head){
		list_node* node = this->head;
		this->head = node->next;
		free(node);
	}
}


