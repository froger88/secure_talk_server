cmake_minimum_required (VERSION 2.8)
project (test)

SET(CMAKE_COLOR_MAKEFILE TRUE)
SET(CMAKE_BUILD_TYPE Debug)

# use c++11
ADD_DEFINITIONS(
    -std=c++11
)


SET(SECURE_TALK_SERVER_CPP_DIR="../../../cpp")

add_executable (test
	main.cpp
	../../cpp/Logger.cpp
	../../cpp/Config.cpp
	../../cpp/SecureCounter.cpp
	../../cpp/Package.cpp
	../../cpp/PackageReader.cpp
)

target_link_libraries (test
	pthread
	config++
	ssl
)
