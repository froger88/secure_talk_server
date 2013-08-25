#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

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
	
	/*
	 * TODO:
	 * prototype - listen on just one interface, one port and generally one
	 * listening thread. In future, listening on multiple interfaces,
	 * multiple threads etc will be implemented.
	 */
	// create ssl_listen_thread
	cout << "initializing ssl_listen_th" <<endl;
	thread t(ssl_listen_thread);
	t.detach();
	
	// serve forever ...
	while(true) {
		sleep(1);
	}
	
	return EXIT_SUCCESS;
}
