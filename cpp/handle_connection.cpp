#include "handle_connection.h"

using namespace std;

extern pthread_mutex_t mutex;

void* handle_connection(void* arg)
{
	cerr << "handle_connection start" << endl << flush;
	handle_connection_arg_t* handle_arg = static_cast<handle_connection_arg_t*> (arg);
	int fd = handle_arg->fd;
	SSL* ssl = handle_arg->ssl;

	char buf[1024];
	int count;

	// accept
	cerr << "handle_connection accepting" << endl << flush;
	if (SSL_accept(ssl) == 0) {
		// errors occured
		cerr << "handle_connection accept, errors occured" << endl << flush;
		goto cleanup;
	}
	
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

cleanup:
	cerr << "handle_connection cleanup" << endl << flush;
	// return back to pool
	SSL_free(ssl);
	close(fd);
	handle_arg->thread_queue->push(handle_arg->pthread_vector_pos);
	delete handle_arg;
}
