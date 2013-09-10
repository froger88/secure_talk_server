/* 
 * File:   Client.h
 * Author: mazabinski
 *
 * Created on 7 wrzesień 2013, 19:30
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
#ifndef CLIENT_H
#define	CLIENT_H

#include <string.h>
#include <thread>
#include <openssl/ssl.h>
#include <openssl/ssl3.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

#include "Config.h"
#include "Package.h"
#include "PackageReader.h"
#include "Logger.h"

class Client {
public:
	Client();
	Client(const Client& orig);
	virtual ~Client();
	
	// setup class with fd and SSL*
	void setup(int fd, SSL* ssl);
	void set_timeouts_ms(int t_read=1000, int t_write=1000);
	
	void create_session();
	void message();
	void invite();
	void handle_connection();
	
private:

	// read packages
	void ssl_read(SecureTalkServer::PackageReader& pkg_reader, int timeout=1000);
	
	// write packages
	void ssl_write(SecureTalkServer::Package& pkg, int timeout=1000);
	
	// generateeunique client_id
	void generate_client_id();
	
	// 64-bit client id
	int64_t id;
	
	// client fd
	int fd;
	
	// client SSL*
	SSL* ssl;
	
	// read timeout on SSL (not supported yet)
	int read_timeout;
	
	// write timeout on SSL (not supported yet)
	int write_timeout;
	
	// read buffer
	char buf[1024];
};

#endif	/* CLIENT_H */

