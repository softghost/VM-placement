/*
 * cmd_host.h
 *
 *  Created on: Jul 19, 2010
 *      Author: rodri145
 */

#ifndef CMD_HOST_H_
#define CMD_HOST_H_

/**
 * Entry point for Server Listen Thread
 */
void listen_entry_point();

/**
 * Entry point for a graph worker thread
 */
void* cmd_worker_thread(void* acceptfd);

#endif /* CMD_HOST_H_ */
