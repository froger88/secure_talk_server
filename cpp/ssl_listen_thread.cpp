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


#include "ssl_listen_thread.h"
#include "Logger.h"

using namespace std;

extern SecureLogger* sLog;

void ssl_listen_thread(void)
{
	sLog->log(L_NOTICE, "ssl_listen_thread");
	
	SSL_library_init();
	SSL_CTX *ssl_ctx;

	const SSL_METHOD *ssl_method;
	OpenSSL_add_all_algorithms();

	ssl_method = SSLv3_server_method();
	ssl_ctx = SSL_CTX_new(ssl_method);
	if (!ssl_ctx) {
		sLog->log(L_FATAL, "ssl_listen_thread: ssl_ctx == NULL");
		abort();
	}

	// set up certificates
	if (SSL_CTX_use_certificate_file(ssl_ctx, Config::ssl_cert.c_str(), SSL_FILETYPE_PEM) <= 0) {
		sLog->log(L_FATAL,  "ssl_listen_thread: Fail to load CertFile");
		abort();
	}
	if (SSL_CTX_use_PrivateKey_file(ssl_ctx, Config::ssl_key.c_str(), SSL_FILETYPE_PEM) <= 0) {
		sLog->log(L_FATAL, "ssl_listen_thread: Fail to load PrivateKey");
		abort();
	}
	if (!SSL_CTX_check_private_key(ssl_ctx)) {
		sLog->log(L_FATAL, "ssl_listen_thread: Private key does not match the public certificate");
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
		sLog->log(L_FATAL, "ssl_listen_thread: fail to bind");
		abort();
	}
	if (listen(fd, 100) != 0) {
		sLog->log(L_FATAL, "ssl_listen_thread: fail to listen on %s:%d", 
		  Config::ssl_ip.c_str(), Config::ssl_port);
		abort();
	}
	SecureCounter cnt_handle_connection;
	// accept and handle_connection
	while (true) {
		struct sockaddr_in addr;
		socklen_t len = sizeof (addr);
		SSL *ssl;

		int client_fd = accept(fd, (struct sockaddr*) &addr, &len);

		sLog->log(L_DEBUG, "ssl_listen_thread: Connection: %s:%hd",
		  inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
		
		ssl = SSL_new(ssl_ctx);
		SSL_set_fd(ssl, client_fd);
		
		if(cnt_handle_connection.get() >= Config::max_client_fd) {
			sLog->log(L_ERROR, "ssl_listen_thread: Too much connections, drop");

			SSL_free(ssl);
			close(client_fd);
			continue;
		}
		
		try {
			thread t (handle_connection, client_fd, ssl, &cnt_handle_connection);
			t.detach();
		} catch (std::exception &e) {
			sLog->log(L_ERROR, "ssl_listen_thread: Exception catched: %s", e.what());

			SSL_free(ssl);
			close(client_fd);
		}
	}

	//serve forever
	while (true) {
		sleep(1);
	}

	pthread_exit(0);
}
