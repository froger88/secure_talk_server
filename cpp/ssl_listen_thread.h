/* 
 * File:   ssl_listen_thread.h
 * Author: mazabinski
 *
 * Created on 24 sierpień 2013, 20:08
 */

#ifndef SSL_LISTEN_THREAD_H
#define	SSL_LISTEN_THREAD_H

#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <cstdlib>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <openssl/ossl_typ.h>
#include <openssl/ssl23.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <netdb.h>
#include <queue>
#include <vector>

#include "Config.h"
#include "handle_connection.h"

void* ssl_listen_thread(void* arg);

#endif	/* SSL_LISTEN_THREAD_H */

