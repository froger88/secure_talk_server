#include "handle_connection.h"

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
			cout << "Client msg: " << buf;
			// answer with the same
			cerr << "handle_connection SSL_write" << endl << flush;
			SSL_write(ssl, buf, strlen(buf));
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
