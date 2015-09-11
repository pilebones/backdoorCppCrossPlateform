cmake_minimum_required(VERSION 3.3)
project(cppCrossPlateformSocketProvider)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    client.cpp
    SockerProvider.h
    SocketProvider.cpp
    SocketClientProvider.cpp
    SocketClientProvider.h
    SocketServerProvider.cpp
    SocketServerProvider.h)

add_executable(cppCrossPlateformSocketProvider ${SOURCE_FILES})