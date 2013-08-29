#include "handle_connection.h"

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

using namespace std;

void handle_connection(int fd, SSL* ssl, SecureCounter* cnt_handle_connection)
{
	cnt_handle_connection->inc();
	char buf[1024];
	int count;

	// accept
	try {
		cerr << "handle_connection accepting" << endl << flush;
		if (SSL_accept(ssl) == 0) {
			// errors occured
			cerr << "handle_connection accept, errors occured" << endl << flush;
			goto cleanup;
		}
	} catch (std::exception &e) {
		cerr << "handle_connection: exception while accepting" << e.what()
		  << endl << flush;
	}

	while (((string) buf).find("quit") == string::npos) {
		cerr << "handle_connection read" << endl << flush;
		count = SSL_read(ssl, buf, sizeof (buf)); /* get request */
		if (count > 0) {
			buf[count] = 0;
			cerr << "Client msg count: " << count << endl <<flush;
			cerr << "Client msg: " <<flush;
			for(int i=0; i< count; ++i) {
				cerr << (int)(unsigned char)buf[i] << " " <<flush;
			}
			cerr << endl << flush;
			// answer with the same
			cerr << "handle_connection SSL_write" << endl << flush;
			SSL_write(ssl, buf, count);
		} else {
			// errors occured
			goto cleanup;
		}
		cerr << "handle_connection SSL_get_fd" << endl << flush;
		fd = SSL_get_fd(ssl);
	}
cleanup:
	cerr << "handle_connection cleanup" << endl << flush;
	// return back to pool
	SSL_free(ssl);
	close(fd);
	cnt_handle_connection->dec();
}
