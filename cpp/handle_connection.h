/* 
 * File:   handle_connection.h
 * Author: mazabinski
 *
 * Created on 24 sierpień 2013, 22:45
 */

#ifndef HANDLE_CONNECTION_H
#define	HANDLE_CONNECTION_H

#include <thread>
#include <openssl/ssl.h>
#include <openssl/ssl3.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include "SecureCounter.h"

#include "Config.h"

using namespace std;

void handle_connection(int fd, SSL* ssl, SecureCounter* cnt_handle_connection);

#endif	/* HANDLE_CONNECTION_H */

