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
        client/src/socketWrapper.cpp
        client/include/socketWrapper.h
        client/src/sonicSocket.cpp
        client/include/sonicSocket.h
        client/src/queryTemplates.cpp
        client/include/queryTemplates.h
        client/src/responseHandler.cpp
        client/include/responseHandler.h
        client/src/errorHandler.cpp
        client/include/errorHandler.h
        client/src/localManager.cpp
        client/include/localManager.h
        client/include/gameManager.h
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
include_directories(PUBLIC common/include)
