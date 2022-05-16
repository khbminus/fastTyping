cmake_minimum_required(VERSION 3.0)
project(fastTyping VERSION 0.1 LANGUAGES CXX)

option(BUILD_SERVER "Enable building server" OFF)
option(BUILD_CLIENT "Enable building client" OFF)
option(BUILD_ALL "Enable building all" OFF)


if (BUILD_SERVER OR BUILD_ALL)
    add_subdirectory(server)
endif ()
if (BUILD_CLIENT OR BUILD_ALL)
    add_subdirectory(client)
endif ()

include_directories(PUBLIC common/include)