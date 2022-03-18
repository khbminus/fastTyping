cmake_minimum_required(VERSION 3.0)
project(fastTyping VERSION 0.1 LANGUAGES CXX)

# -*-*- QT CMake start -*-*-
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Network REQUIRED)

set(PROJECT_SOURCES
        client/src/main.cpp
        client/src/mainwindow.cpp
        client/include/mainwindow.h
        client/src/joinwindow.cpp
        client/include/joinwindow.h
        client/src/createwindow.cpp
        client/include/createwindow.h
        client/src/windowcontroller.cpp
        client/include/windowcontroller.h
        client/src/gamewindow.cpp
        client/include/gamewindow.h
        client/src/statwindow.cpp
        client/include/statwindow.h
        client/src/gameLogic.cpp
        client/include/gameLogic.h
        client/src/tcp_client.cpp
        client/include/tcp_client.h
        client/src/gameClient.cpp
        client/include/gameClient.h
        common/include/json.hpp
        )

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(client
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            )
    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET client APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else ()
    add_executable(client
            ${PROJECT_SOURCES}
            )
endif ()

target_link_libraries(client PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(client PRIVATE Qt${QT_VERSION_MAJOR}::Network)

set_target_properties(client PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )

target_include_directories(client PRIVATE client/include)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(client)
endif ()

# -*-*- QT CMake end -*-*-

find_package(Boost 1.71 REQUIRED system)  # Required by Boost.DLL

add_executable(server server/src/main.cpp server/src/server.cpp server/src/game.cpp server/include/user.h)
target_include_directories(server PRIVATE ${Boost_INCLUDE_DIRS} server/include)
target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} ${NETWORKING_LIBS})

add_executable(server-tests server/src/doctest_main.cpp server/src/userTests.cpp)
target_include_directories(server-tests PRIVATE ${Boost_INCLUDE_DIRS} server/include)
target_link_libraries(server-tests ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} ${NETWORKING_LIBS})


include_directories(PUBLIC common/include)