cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(clientserver
    VERSION 1.0
    LANGUAGES CXX)

find_package(Boost 1.78 REQUIRED)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif (Boost_FOUND)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-lboost_system -lpthread -Wall -Wpedantic -Wextra")

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)

if (UNIX)
    add_executable(server src/Server/main.cpp src/StringExt.cpp)
    add_executable(client src/Client/client.cpp src/Client/main.cpp src/StringExt.cpp)

    set_target_properties(server PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/src/Server")
    set_target_properties(client PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/src/Client")
endif (UNIX)

if (WIN32)
    add_executable(server.exe src/Server/main.cpp src/StringExt.cpp)
    add_executable(client.exe src/Client/client.cpp src/Client/main.cpp src/StringExt.cpp)

    set_target_properties(server.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/src/Server")
    set_target_properties(client.exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/src/Client")
endif (WIN32)
