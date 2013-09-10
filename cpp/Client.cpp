/* 
 * File:   Client.cpp
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


#include "Client.h"

extern SecureLogger* sLog;

Client::Client()
{
	generate_client_id();
	ssl = NULL;
}

Client::Client(const Client& orig) { }

Client::~Client()
{
	SSL_free(ssl);
	close(fd);
}

void Client::setup(int fd, SSL* ssl)
{
	this->fd = fd;
	this->ssl = ssl;
}

void Client::set_timeouts_ms(int t_read, int t_write)
{
	read_timeout = t_read;
	write_timeout = t_write;
}

void Client::generate_client_id()
{
	// generate random 64bits clientid
	int32_t x = random();
	memcpy(&id, &x, 4);
	x = random();
	memcpy((int64_t*) & id + 4, &x, 4);
}

void Client::ssl_read(SecureTalkServer::PackageReader& pkg_reader, int timeout)
{
	SSL_read(ssl, buf, sizeof (buf));
	// and handle it...
}

void Client::ssl_write(SecureTalkServer::Package& pkg, int timeout)
{
	const pair<char*, size_t>* pkg_ptr;
	pkg_ptr = pkg.get();
	SSL_write(ssl, pkg_ptr->first, pkg_ptr->second);
	pkg_ptr = NULL;
}

void Client::handle_connection()
{
	sLog->log(L_DEBUG, "Client #%lld handle_connection", id);
	char buf[1024];

	while (true) {
		SecureTalkServer::PackageReader p;
		ssl_read(p);
		
		switch(p.get_pkg_type()) {
			case SecureTalkServer::PT_CREATE_SESSION:
				// create session
				break;
			case SecureTalkServer::PT_DESTROY_SESSION:
				// destroy session
				break;
			case SecureTalkServer::PT_INVITE_CLIENT:
				// invite
				break;
			case SecureTalkServer::PT_JOIN_SESSION:
				// join
				break;
			case SecureTalkServer::PT_PING:
				// ping
				break;
			case SecureTalkServer::PT_PONG:
				// pong
				break;
			case SecureTalkServer::PT_QUIT:
				// quit
			case SecureTalkServer::PT_UNKNOWN:
			default:
				sLog->log(L_ERROR, "Client #%lld unknown package", id);
		}
	}
}