#include "ssl_listen_thread.h"

using namespace std;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* ssl_listen_thread(void* arg)
{
	// create pthread_t queue
	vector <pair<pthread_t, pthread_attr_t> > pthread_vector;
	pthread_vector.resize(Config::max_client_fd);
	queue<int> pthread_queue;

	for (int i = 0; i < Config::max_client_fd; ++i) {
		pthread_queue.push(i);
	}

	int pthread_vector_pos = -1;

	SSL_library_init();
	SSL_CTX *ssl_ctx;

	const SSL_METHOD *ssl_method;
	OpenSSL_add_all_algorithms();

	ssl_method = SSLv3_server_method();
	ssl_ctx = SSL_CTX_new(ssl_method);
	if (ssl_ctx == NULL) {
		cerr << "ssl_ctx init fail" << endl << flush;
		abort();
	}

	// set up certificates
	if (SSL_CTX_use_certificate_file(ssl_ctx, Config::ssl_cert.c_str(), SSL_FILETYPE_PEM) <= 0) {
		cerr << "Fail to load CertFile" << endl << flush;
		abort();
	}
	if (SSL_CTX_use_PrivateKey_file(ssl_ctx, Config::ssl_key.c_str(), SSL_FILETYPE_PEM) <= 0) {
		cerr << "Fail to load PrivateKey" << endl << flush;
		abort();
	}
	if (!SSL_CTX_check_private_key(ssl_ctx)) {
		cerr << "Private key does not match the public certificate" << endl << flush;
		abort();
	}

	// start listen
	int fd;
	struct sockaddr_in addr;

	fd = socket(PF_INET, SOCK_STREAM, 0);
	memset(&addr, 0, sizeof (addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Config::ssl_port);
	addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(fd, (struct sockaddr*) &addr, sizeof (addr)) != 0) {
		cerr << "fail to bind" << endl << flush;
		abort();
	}
	if (listen(fd, 100) != 0) {
		cerr << "fail to listen on " << Config::ssl_ip << ":" << Config::ssl_port
		  << endl << flush;
		abort();
	}

	// accept and handle_connection
	while (true) {
		struct sockaddr_in addr;
		socklen_t len = sizeof (addr);
		SSL *ssl;

		int client_fd = accept(fd, (struct sockaddr*) &addr, &len);

		cout << "Connection: " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << endl;
		ssl = SSL_new(ssl_ctx);
		SSL_set_fd(ssl, client_fd);

		/*
		 * TODO:
		 * now just pthread_create, in future connection pool will be implemented
		 */

		pthread_mutex_lock(&mutex);
		if (!pthread_queue.empty()) {
			pthread_vector_pos = pthread_queue.front();
			pthread_queue.pop();
		}
		pthread_mutex_unlock(&mutex);

		if (pthread_vector_pos < 0) {
			// too much connections, drop
			close(client_fd);
			continue;
		}

		pthread_attr_init(&pthread_vector[pthread_vector_pos].second);
		pthread_attr_setdetachstate(&pthread_vector[pthread_vector_pos].second,
		  PTHREAD_CREATE_DETACHED
		  );

		handle_connection_arg_t* handle_arg = new handle_connection_arg_t;

		handle_arg->fd = client_fd;
		handle_arg->thread_data = &pthread_vector[pthread_vector_pos];
		handle_arg->thread_queue = &pthread_queue;
		handle_arg->pthread_vector_pos = pthread_vector_pos;
		handle_arg->ssl = ssl;

		if (pthread_create(&pthread_vector[pthread_vector_pos].first,
		  &pthread_vector[pthread_vector_pos].second,
		  handle_connection,
		  static_cast<void*> (handle_arg)) == 0) {
			// return back to pool
			pthread_queue.push(pthread_vector_pos);
			SSL_free(ssl);
			close(client_fd);
			delete handle_arg;
		}
		pthread_vector_pos = -1;
	}

	//serve forever
	while (true) {
		sleep(1);
	}

	pthread_exit(0);
}
