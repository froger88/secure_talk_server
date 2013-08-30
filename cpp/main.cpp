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

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

//logger
#include "Logger.h"

// configuration
#include "Config.h"

// ssl listen thread
#include "ssl_listen_thread.h"

using namespace std;

SecureLogger *sLog;

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
	
	sLog = new SecureLogger();

	sLog->log(L_DEBUG, "debug_test");	
//	log.log(L_DEBUG, "debug_test");
//	log.log(L_NOTICE, "notice_test");
//	log.log(L_INFO, "info_test");
//	log.log(L_SUCCESS, "success_test");
//	log.log(L_ERROR, "error_test");
//	log.log(L_FATAL, "fatal_test");
	
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
