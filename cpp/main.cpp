#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// configuration
#include "Config.h"

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

	return EXIT_SUCCESS;
}