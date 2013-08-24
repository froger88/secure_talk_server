#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// configuration
#include "Config.h"

// ssl listen thread
#include "ssl_listen_thread.h"

using namespace std;

int main(int argc, char* argv[])
{
	if( (argc == 1 && !Config::read_config()) ||
	  (argc == 2 && !Config::read_config(argv[1])) ) {
		// if errors occured during Config read
		Config::show_usage();
		Config::write_conf_example();
		return EXIT_FAILURE;
	}
	
	// if config successfully read, show configuration
	Config::show_configuration();
	
	// setup ssl_listen_thread
	pthread_t ssl_listen_th;
	pthread_attr_t ssl_listen_th_attr;
	pthread_attr_init(&ssl_listen_th_attr);
	pthread_attr_setdetachstate(&ssl_listen_th_attr, PTHREAD_CREATE_DETACHED);
	pthread_setconcurrency(10);
	
	// create ssl_listen_thread
	cout << "initializing ssl_listen_th" <<endl;
	if(pthread_create(&ssl_listen_th, &ssl_listen_th_attr, ssl_listen_thread, NULL) != 0) {
		cerr << "fail to create ssl_listen_thread" << endl;
		return EXIT_FAILURE;
	}
	
	// serve forever ...
	while(true) {
		sleep(1);
	}
	
	return EXIT_SUCCESS;
}
