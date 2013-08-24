cmake_minimum_required (VERSION 2.8)
project (secure_talk_server)

add_executable (secure_talk_server
	main.cpp
	Config.cpp
	ssl_listen_thread.cpp
	handle_connection.cpp
)

target_link_libraries (secure_talk_server
	pthread
	config++
)
