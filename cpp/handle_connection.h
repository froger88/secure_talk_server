/* 
 * File:   handle_connection.h
 * Author: mazabinski
 *
 * Created on 24 sierpień 2013, 22:45
 */

/*
                    +IMMMM .~MMZ.
                 .MM NMMMMM  .MMMM
                MMM. MMMMMMZ   MMMM.
              .MMM, .MMMMMMM  ..MMMM
              .MMM. ZMMMMMMM.   MMMM.
              .MMM  =MMMMMMM.   MMMM.
              .MMM . MMMMMMM.  MMMM
                MMM: MMMMMMM .ZMMM
                  MMM MMMMMM.~MO
                      ~MMMN..   ...M.
                        .?M8 .. +.NI
                       . .....  MNM D
                        : D..Z...MO.?.
                          NM . M..  .~
                         .~I...     .,
                          .M.       M.
                         .M.        :
                        .M        .MM
                       .7           M.
                       M            MO
                      M.            .8       .=MMMMMMM .
                     M.             .I    MM$          ,M
                    .                M MM .             .M
                    M                 N              .   M
                   .:                                M   ,.
                                                     +   .
                    ,                                Z  .M
                   .M                               ..  ,
                    M                  .            M.  M..  =+, .
                     M        ?        D           :+   7  ..M$ ..
                     .Z        M.       ,         DMM.     .. =M.
                      ,M      .8,       M        M.. .MMM,...
               .. N  M$ MI.    MM.      :.   ..M$
             .$...  =  MM . D,,MM7       MMMM,
                   Z...    MN   MM.      MMOMMM7..
                      .D.,8      M.      :..:NM$
                                 MM.      .MM~.,
                                  MM.  ~=7DMMM$.

          S   E   A    L       O   F      T   H   E       D   A   Y
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
#include "Logger.h"
#include "Config.h"

using namespace std;

void handle_connection(int fd, SSL* ssl, SecureCounter* cnt_handle_connection);

#endif	/* HANDLE_CONNECTION_H */

