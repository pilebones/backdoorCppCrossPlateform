cmake_minimum_required(VERSION 3.3)
project(cppCrossPlateformSocketProvider)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lboost_program_options")

set(SOURCE_FILES
        main.cpp
        core/socket/SocketProvider.cpp
        core/socket/SocketClientProvider.cpp
        core/socket/SocketServerProvider.cpp)

set(HEADER_FILES
        core/socket/SocketProvider.h
        core/socket/SocketClientProvider.h
        core/socket/SocketServerProvider.h core/utils/Array.cpp core/utils/Array.h)

IF(WIN32)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lws2_32 -lwsock32")
ENDIF(WIN32)

add_executable(main ${SOURCE_FILES} ${HEADER_FILES})
