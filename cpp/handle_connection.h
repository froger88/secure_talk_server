/* 
 * File:   handle_connection.h
 * Author: mazabinski
 *
 * Created on 24 sierpień 2013, 22:45
 */

#ifndef HANDLE_CONNECTION_H
#define	HANDLE_CONNECTION_H

#include <pthread.h>
#include <vector>
#include <queue>
#include <openssl/ssl.h>
#include <openssl/ssl3.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

#include "Config.h"

using namespace std;

void* handle_connection(void* arg);

typedef struct handle_connection_arg_s
{
	int fd;
	pair<pthread_t, pthread_attr_t>* thread_data;
	queue<int >* thread_queue;
	int pthread_vector_pos;
	SSL* ssl;
} handle_connection_arg_t;

#endif	/* HANDLE_CONNECTION_H */

