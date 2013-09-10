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

#include "handle_connection.h"

extern SecureLogger* sLog;

using namespace std;

void handle_connection(int fd, SSL* ssl, SecureCounter* cnt_handle_connection)
{
	cnt_handle_connection->inc();
	char buf[1024];
	int count;

	// accept
	try {
		sLog->log(L_DEBUG, "handle_connection: accepting");
		if (SSL_accept(ssl) == 0) {
			// errors occured
			sLog->log(L_ERROR, "handle_connection accept, errors occured");
			SSL_free(ssl);
			close(fd);
			cnt_handle_connection->dec();
		}
	} catch (std::exception &e) {
		sLog->log(L_ERROR, "handle_connection: exception while accepting, %s ", e.what());
		return;
	}

	// create client instance and handle requests
	Client c;
	c.setup(fd, ssl);
	c.set_timeouts_ms(1000, 1000);
	c.handle_connection();

	cnt_handle_connection->dec();

	// will be movd to handle_connection inside Client class
	//	while (((string) buf).find("quit") == string::npos) {
	//		sLog->log(L_DEBUG, "handle_connection: SSL_read");
	//		count = SSL_read(ssl, buf, sizeof (buf)); /* get request */
	//		if (count > 0) {
	//			buf[count] = 0;
	//
	//			// answer with the same
	//			sLog->log(L_DEBUG, "handle_connection: SSL_write");
	//			SSL_write(ssl, buf, count);
	//		} else {
	//			// errors occured
	//			goto cleanup;
	//		}
	//		sLog->log(L_DEBUG, "handle_connection: SSL_get_fd");
	//		fd = SSL_get_fd(ssl);
	//	}
}
