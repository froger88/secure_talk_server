/* 
 * File:   ssl_listen_thread.h
 * Author: mazabinski
 *
 * Created on 24 sierpień 2013, 20:08
 */

#ifndef SSL_LISTEN_THREAD_H
#define	SSL_LISTEN_THREAD_H

#include <unistd.h>
#include <pthread.h>
#include <iostream>

void* ssl_listen_thread(void* arg);

#endif	/* SSL_LISTEN_THREAD_H */

