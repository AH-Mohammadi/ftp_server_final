cmake_minimum_required(VERSION 3.10)

# Project name
project(FTPServerClient)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable for server
add_executable(server server.cpp)

# Add executable for client
add_executable(client client.cpp)

# Include necessary libraries
target_link_libraries(server pthread)
target_link_libraries(client pthread)
