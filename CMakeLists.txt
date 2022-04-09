cmake_minimum_required(VERSION 3.0)
project(fastTyping VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)


#set(EXTRA_CXX_FLAGS "-fsanitize=undefined -fsanitize=address -ggdb -O0")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_FLAGS}")
find_package(Boost 1.71 COMPONENTS system thread regex log REQUIRED)
find_package(Threads)
add_executable(server server/src/main.cpp server/src/server.cpp server/src/game.cpp server/include/user.h)
target_include_directories(server PRIVATE ${Boost_INCLUDE_DIRS} server/include common/include .)
target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} ${NETWORKING_LIBS})

add_executable(server-tests server/src/doctest_main.cpp server/src/userTests.cpp)
target_include_directories(server-tests PRIVATE ${Boost_INCLUDE_DIRS} server/include common/include .)
target_link_libraries(server-tests ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} ${NETWORKING_LIBS})


include_directories(PUBLIC common/include)