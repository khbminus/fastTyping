cmake_minimum_required(VERSION 3.0)
project(fastTyping VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)


#set(EXTRA_CXX_FLAGS "-fsanitize=undefined -fsanitize=address -ggdb -O0")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_FLAGS}")
find_package(Boost 1.71 COMPONENTS system thread regex log REQUIRED)
find_package(Threads)

include(FetchContent)
FetchContent_Declare(
        libpqxx
        GIT_REPOSITORY https://github.com/jtv/libpqxx.git
        GIT_TAG a6b1d60e74c1427c8ac2324b85cd4a0dc2068332
)
set(PQXX_LIBRARIES pqxx_static)

FetchContent_MakeAvailable(libpqxx)

add_executable(sql server/src/database.cpp)

target_link_libraries(sql "${PQXX_LIBRARIES}" )

add_executable(server server/src/main.cpp server/src/server.cpp server/src/game.cpp server/include/user.h)
target_include_directories(server PRIVATE ${Boost_INCLUDE_DIRS} server/include common/include .)
target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} ${NETWORKING_LIBS} "${PQXX_LIBRARIES}")

add_executable(server-tests server/src/doctest_main.cpp server/src/userTests.cpp)
target_include_directories(server-tests PRIVATE ${Boost_INCLUDE_DIRS} server/include common/include .)
target_link_libraries(server-tests ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} ${NETWORKING_LIBS} "${PQXX_LIBRARIES}")


include_directories(PUBLIC common/include)

