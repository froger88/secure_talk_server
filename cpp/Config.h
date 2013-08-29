/* 
 * File:   Config.h
 * Author: mazabinski
 *
 * Created on 24 sierpień 2013, 16:56
 */

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

#ifndef CONFIG_H
#define	CONFIG_H

#include <string>
#include <libconfig.h++>

class Config {
public:
	Config();
	Config(const Config& orig);
	virtual ~Config();
	
	// host IP
	static std::string ssl_ip;
	
	// host port
	static int ssl_port;
	
	// ssl key
	static std::string ssl_key;

	// ssl cert
	static std::string ssl_cert;
	
	// max client fd
	static int max_client_fd;
	
	static bool read_config(const char* file="secure_talk_server.conf");
	static void show_usage();
	static void show_configuration();
	static void write_conf_example(const char* file="secure_talk_server.conf.example");
private:
};

#endif	/* CONFIG_H */

