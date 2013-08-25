/* 
 * File:   Config.h
 * Author: mazabinski
 *
 * Created on 24 sierpień 2013, 16:56
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

