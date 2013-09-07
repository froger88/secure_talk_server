cmake_minimum_required (VERSION 2.8)
project (secure_talk_server)

SET(CMAKE_COLOR_MAKEFILE TRUE)
SET(CMAKE_BUILD_TYPE Debug)

# use c++11
ADD_DEFINITIONS(
    -std=c++11
)

add_executable (secure_talk_server
	main.cpp
	Logger.cpp
	Config.cpp
	SecureCounter.cpp
	ssl_listen_thread.cpp
	handle_connection.cpp
	Package.cpp
	PackageReader.cpp
	utils.cpp
	Client.cpp
)

target_link_libraries (secure_talk_server
	pthread
	config++
	ssl
)
