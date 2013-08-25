/* 
 * File:   Config.cpp
 * Author: mazabinski
 * 
 * Created on 24 sierpień 2013, 16:56
 */

#include "Config.h"
#include <iostream>

// host IP
extern std::string Config::ssl_ip;

// host port
extern int Config::ssl_port;

// ssl key
extern std::string Config::ssl_key;

// ssl cert
extern std::string Config::ssl_cert;

// max client fd
extern int Config::max_client_fd;

Config::Config() { }

Config::Config(const Config& orig) { }

Config::~Config() { }

bool Config::read_config(const char* file)
{
	if (!file) {
		return false;
	}

	libconfig::Config cfg;
	// Read the file. If there is an error, report it and exit.
	try {
		cfg.readFile(file);

	} catch (const libconfig::FileIOException &fioex) {
		std::cerr << "I/O error while reading file." << std::endl;
		return false;

	} catch (const libconfig::ParseException &pex) {
		std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
		  << " - " << pex.getError() << std::endl;
		return false;
	}

	try {
		cfg.lookupValue("ssl_ip", ssl_ip);
		cfg.lookupValue("ssl_port", ssl_port);
		cfg.lookupValue("ssl_cert", ssl_cert);
		cfg.lookupValue("ssl_key", ssl_key);
		cfg.lookupValue("max_client_fd", max_client_fd);

	} catch (std::exception &e) {
		std::cout << "Exception while read config file: " << e.what() << std::endl;
		throw;
	}

	return true;
}

void Config::show_usage()
{
	std::cout << "usage: " << std::endl;
	std::cout << "./secure_talk_server [config_file_dir]" << std::endl;
	std::cout << "if config_file_dir not specified, program will look" << std::endl;
	std::cout << "for secure_talk_server.conf file. if doesnt't exists" << std::endl;
	std::cout << "program will exit with EXIT_FAIL status and example" << std::endl;
	std::cout << "secure_talk_server.conf.example will be written" << std::endl;
}

void Config::show_configuration()
{
	std::cout << "######################## CONFIGURATION ########################"
	  << std::endl;
	std::cout << "\tssl_cert = " << ssl_cert << std::endl;
	std::cout << "\tssl_key = " << ssl_key << std::endl;
	std::cout << "\tssl_ip = " << ssl_ip << std::endl;
	std::cout << "\tssl_port = " << ssl_port << std::endl;
	std::cout << "######################## END OF CONFIG ########################"
	  << std::endl;
}

void Config::write_conf_example(const char* file)
{
	libconfig::Config cfg;

	libconfig::Setting &root = cfg.getRoot();

	// Add some settings to the configuration.

	root.add("ssl_ip", libconfig::Setting::TypeString) = "0.0.0.0";
	root.add("ssl_port", libconfig::Setting::TypeInt) = 9900;
	root.add("ssl_cert", libconfig::Setting::TypeString) = "./ssl/cert.pem";
	root.add("ssl_key", libconfig::Setting::TypeString) = "./ssl/key.pem";
	root.add("max_client_fd", libconfig::Setting::TypeInt) = 100;

	// Write out the new configuration.
	try {
		cfg.writeFile(file);
		std::cerr << "New configuration successfully written to: " << file << std::endl;

	} catch (const libconfig::FileIOException &fioex) {
		std::cerr << "I/O error while writing file: " << file << std::endl;
		throw;
	}
	return;
}
