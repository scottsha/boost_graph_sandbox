cmake_minimum_required(VERSION 3.21)
project(kevin_bacon)

set(CMAKE_CXX_STANDARD 23)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(CONANFILE conanfile.py
        BASIC_SETUP TARGETS
        BUILD missing)
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})

find_package(Boost REQUIRED)
# This should include boost which is header only, so there is no need to link

add_subdirectory(kevin_bacon_example)
add_subdirectory(colored_subgraph_matching)
