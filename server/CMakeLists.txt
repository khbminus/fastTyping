cmake_minimum_required(VERSION 3.0)
project(fasttyping_server)

set(CMAKE_CXX_STANDARD 17)
find_package(Boost 1.71 REQUIRED system log)
find_package(nlohmann_json 3.7.3 REQUIRED)
find_package(Threads)

include_directories(include ../common/include)
add_executable(server src/main.cpp src/server.cpp src/game.cpp include/user.h)
target_include_directories(server PRIVATE ${Boost_INCLUDE_DIRS} include)
target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} ${NETWORKING_LIBS})
target_link_libraries(server nlohmann_json::nlohmann_json)

add_executable(server-tests src/doctest_main.cpp src/userTests.cpp)
target_include_directories(server-tests PRIVATE ${Boost_INCLUDE_DIRS} include)
target_link_libraries(server-tests ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} ${NETWORKING_LIBS})
