#include "ssl_listen_thread.h"
#include "Config.h"
#include <cstdlib>

using namespace std;

void* ssl_listen_thread(void* arg)
{
	//serve forever
	while(true) {
		sleep(1);
	}
	
	pthread_exit(0);
}
