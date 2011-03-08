/*
 * File:   main.c
 * Author: andres
 *
 * Created on May 31, 2010, 12:40 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>

#include "cmd_host.h"

/**
 * Main entry point
 */
int main(int argc, char *argv[]) {

	listen_entry_point();

    return 0;
}


