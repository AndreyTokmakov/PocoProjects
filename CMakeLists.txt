cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(PocoProjects)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -pthread")
set(PROJECT_VERSION 1.0.0.0)
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES CXX)

add_compile_options(-c -Wall -Wextra -O3 -std=c++2a)

set(CMAKE_PREFIX_PATH ../third_party/poco/cmake-build)


# ** FOR CONAN ** ??
# message(STATUS "CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")
# include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
# conan_basic_setup()

find_package(Poco REQUIRED Foundation Net)

# include all components
add_executable(PocoProjects
        main.cpp
        Foundation/Events.cpp Foundation/Events.h
        Processes/Processes.cpp Processes/Processes.h
        Networking/Networking.cpp Networking/Networking.h
        Networking/WebSockets.cpp Networking/WebSockets.h
        Environment/Environment.cpp Environment/Environment.h
        DataTime/Timer.h DataTime/Timer.cpp
        Random/Random.h Random/Random.cpp
        Cryptography/Cryptography.cpp Cryptography/Cryptography.h
        MultiThreading/MultiThreading.cpp MultiThreading/MultiThreading.h
)

TARGET_LINK_LIBRARIES(PocoProjects
        Poco::Foundation Poco::Net
        pthread
)

# ** FOR CONAN ** ??
# TARGET_LINK_LIBRARIES(PocoProjects  ${CONAN_LIBS})