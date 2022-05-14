cmake_minimum_required(VERSION 3.0)
project(fasttyping_client VERSION 1.0 LANGUAGES CXX)

# -*-*- QT CMake start -*-*-
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Network REQUIRED)
find_package(nlohmann_json 3.7.3 REQUIRED)


include_directories(include ../common/include)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        include/mainwindow.h
        src/joinwindow.cpp
        include/joinwindow.h
        src/createwindow.cpp
        include/createwindow.h
        src/windowcontroller.cpp
        include/windowcontroller.h
        src/gamewindow.cpp
        include/gamewindow.h
        src/statwindow.cpp
        include/statwindow.h
        src/errorHandler.cpp
        src/queryTemplates.cpp
        src/localManager.cpp
        src/responseHandler.cpp
        src/socketWrapper.cpp
        src/sonicSocket.cpp
        src/webManager.cpp
        src/confirmWindow.cpp
        include/gameManager.h
        include/errorHandler.h
        include/localManager.h
        include/managerUtils.h
        include/queryTemplates.h
        include/responseHandler.h
        include/socketWrapper.h
        include/sonicSocket.h
        include/webManager.h
        include/confirmWindow.h
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
target_link_libraries(client PRIVATE nlohmann_json::nlohmann_json)

set_target_properties(client PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )

target_include_directories(client PRIVATE include)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(client)
endif ()

# -*-*- QT CMake end -*-*-
