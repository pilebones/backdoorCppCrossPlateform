cmake_minimum_required(VERSION 3.3)
project(backdoorCppCrossPlateform)
message(STATUS "OS Detected : ${CMAKE_SYSTEM_NAME}")

# see :
# CMake Move default directories => http://stackoverflow.com/questions/18826789/cmake-output-build-directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

# Compilation settings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
IF(WIN32)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lws2_32 -lwsock32")
        set(CMAKE_GENERATOR "MinGW Makefiles")
        set(CMAKE_C_COMPILER "C:\\MinGW\\bin\\gcc.exe")
        set(CMAKE_CXX_COMPILER "C:\\MinGW\\bin\\g++.exe")

        # see :
        # Install Boost Lib on Windows => http://andres.jaimes.net/718/how-to-install-the-c-boost-libraries-on-windows/
        # Configure CMake on Windows => http://stackoverflow.com/questions/28761323/how-to-include-external-library-boost-into-clion-c-project-with-cmake
        set(BOOST_INCLUDEDIR "C:\\Boost\\include\\boost-1_59\\boost")
        set(BOOST_LIBRARYDIR "C:\\Boost\\lib")
        set(BOOST_ROOT "C:\\Boost")
        set(Boost_ADDITIONAL_VERSIONS "1.59")
        # set(Boost_USE_STATIC_LIBS ON)
        # set(Boost_USE_MULTITHREADED ON)
        # set(Boost_USE_STATIC_RUNTIME OFF)

ELSE()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lboost_program_options")
ENDIF(WIN32)

message(STATUS "Compiler Flags used : ${CMAKE_CXX_FLAGS}")

find_package(
        Boost
        1.59
        COMPONENTS program_options
        REQUIRED
)

if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIR})
endif()

set(SOURCE_FILES
        src/main.cpp
        src/core/cli/Interface.cpp
        src/core/utils/Array.cpp
        src/core/utils/Network.cpp
        src/core/socket/SocketProvider.cpp
        src/core/socket/SocketClientProvider.cpp
        src/core/socket/SocketServerProvider.cpp
        src/core/socket/SocketWrapper.cpp
)

set(HEADER_FILES
        src/core/common/Constant.h
        src/core/cli/Interface.h
        src/core/utils/Array.h
        src/core/utils/Network.h
        src/core/socket/SocketProvider.h
        src/core/socket/SocketClientProvider.h
        src/core/socket/SocketWrapper.h
        src/core/socket/SocketServerProvider.h
)

add_executable(main ${SOURCE_FILES} ${HEADER_FILES})
if(WIN32)
    target_link_libraries(main wsock32 ws2_32)
endif()

if(Boost_FOUND)
    target_link_libraries(main ${Boost_LIBRARIES})
endif()
