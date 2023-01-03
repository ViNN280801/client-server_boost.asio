cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

project(clientserver
    VERSION 1.0
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-lboost_system -Wall -Wpedantic -Wextra")

find_package(Boost 1.78.0 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)

add_executable(server src/Server/server.cpp src/Server/main.cpp)
add_executable(client src/Client/client.cpp src/Client/main.cpp)

set_target_properties(server PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/src/Server")
set_target_properties(client PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/src/Client")
