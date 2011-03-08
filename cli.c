/*
 * cli.c
 *
 *  Created on: Jul 3, 2010
 *      Author: rodri145
 */

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "cli.h"
#include "graph.h"

int graph_get_id(){
	return 1;
}

void cli_load_file(char* filename, list_graph* t_graph, list_graph* c_graph){
	printf("---%s---\n", filename);


	//Get the file
	FILE *file = fopen ( filename, "r" );

	//Maximum line size
	char line [128];

	//Create a link map to keep track of the relation between constraint and traffic
	list map;
	list_init(&map);
	list_graph_init(c_graph, 0, graph_get_id());

	//Check the header
	int size;
	if(fscanf(file, "%*s %d", &size) > 0){
		list_graph_init(t_graph, size, graph_get_id());
	}
	else{
		printf("malformed file\n");
		return;
	}

	int i = 0, j = 0, c = 0, csize = 0;
	for (i = 0; i < size; i++){
		for (j = 0; j < size; j++){
			if(fscanf(file, "%s", line) < 0){
				printf("malformed file\n");
				return;
			}

			c = line[0] == 'X' || line[0] == 'x';
			int wgt = atoi(line + c);
			list_graph_add_edge(t_graph, i, j, wgt, c);

			if (c){
				if(!list_contains(&map, i)){
					list_insert_payload(&map, i, csize);
					list_graph_add_vertex(c_graph);
					list_graph_set_link(c_graph, csize, i);
					csize++;
				}

				if(!list_contains(&map, j)){
					list_insert_payload(&map, j, csize);
					list_graph_add_vertex(c_graph);
					list_graph_set_link(c_graph, csize, j);
					csize++;
				}

				int ci = list_search(&map, i);
				int cj = list_search(&map, j);
				list_graph_add_edge(c_graph, ci, cj, wgt, c);
			}
		}
	}

	printf("Constraints:\n");
	list_graph_print(c_graph);
	printf("Traffic:\n");
	list_graph_print(t_graph);

	printf("---%s---\n", filename);

	fclose(file);
}
