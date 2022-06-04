cmake_minimum_required(VERSION 3.14)
project(fasttyping_client VERSION 1.0 LANGUAGES CXX
        DESCRIPTION "a keyboard multi-/single-player game")

set(target ${PROJECT_NAME})
# -*-*- QT CMake start -*-*-
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Quick Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick Widgets QuickWidgets Network)
find_package(nlohmann_json 3.7.3 REQUIRED)


include_directories(include ../common/include)

if (WIN32)
    set(CMAKE_WIN32_EXECUTABLE ON)
elseif (APPLE)
    set(CMAKE_MACOSX_BUNDLE ON)
endif ()

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        include/mainwindow.h
        src/joinwindow.cpp
        include/joinwindow.h
        src/createwindow.cpp
        include/createwindow.h
        src/startgamewindow.cpp
        include/startgamewindow.h
        src/waitgamewindow.cpp
        include/waitgamewindow.h
        src/windowcontroller.cpp
        include/windowcontroller.h
        src/gamewindow.cpp
        include/gamewindow.h
        src/statwindow.cpp
        include/statwindow.h
        src/registerwindow.cpp
        include/registerwindow.h
        src/loginwindow.cpp
        include/loginwindow.h
        src/signwindow.cpp
        include/signwindow.h
        src/errorHandler.cpp
        src/queryTemplates.cpp
        src/localManager.cpp
        src/localManagerSolo.cpp
        src/responseHandler.cpp
        src/socketWrapper.cpp
        src/sonicSocket.cpp
        src/webManager.cpp
        src/webSoloManager.cpp
        src/confirmWindow.cpp
        src/responseParse.cpp
        src/gameContextManager.cpp
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
        include/responseParse.h
        include/gameContextManager.h

        src/createwindow.ui
        src/gamewindow.ui
        src/joinwindow.ui
        src/mainwindow.ui
        src/statwindow.ui
        src/loginwindow.ui
        src/registerwindow.ui
        src/signwindow.ui
        src/startgamewindow.ui
        src/waitgamewindow.ui

        qml.qrc

        include/keyboard.h
        src/keyboard.cpp

        include/textScreen.h
        src/textScreen.cpp

        include/WpmChartModel.h
        src/WpmChartModel.cpp
        )

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${target}
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            )
    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET client APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else ()
    add_executable(${target}
            ${PROJECT_SOURCES}
            )
endif ()

set_target_properties(${target} PROPERTIES AUTORCC_OPTIONS "--compress;9")
target_include_directories(${target} PUBLIC include ../common/include)

#target_compile_definitions(client
#        PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${target}
        PRIVATE Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Quick
        Qt${QT_VERSION_MAJOR}::QuickWidgets)

target_link_libraries(${target} PRIVATE nlohmann_json::nlohmann_json)
qt_add_qml_module(${target}
        URI fasttyping.keyboard
        VERSION 1.0
        SOURCES
        src/keyboard.cpp
        include/keyboard.h
        )

target_include_directories(${target} PRIVATE include src .)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${target})
endif ()


if (UNIX AND NOT APPLE)
    install(TARGETS ${target} RUNTIME DESTINATION bin)
elseif (WIN32)
    install(TARGETS ${target} RUNTIME DESTINATION bin)
elseif (APPLE)
    set(CMAKE_MACOSX_RPATH ON)
    set(CMAKE_MACOSX_BUNDLE ON)
    install(TARGETS ${target} BUNDLE DESTINATION .
            RUNTIME DESTINATION .)
    set_target_properties(${target} PROPERTIES
            MACOSX_BUNDLE_GUI_IDENTIFIER khbminus.fasttyping.client
            MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
            MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"
            MACOSX_BUNDLE_INFO_STRING "${PROJECT_NAME} Copyright (c) 2022 Sergey Khartiontsev-Beglov, Jegor Popow, Yurij Zaytcev"
            MACOSX_BUNDLE_COPYRIGHT "Copyright (c) 2022 Sergey Khartiontsev-Beglov, Jegor Popow, Yurij Zaytcev"
            MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
            MACOSX_BUNDLE TRUE
            )
endif ()

if (CMAKE_BUILD_TYPE_UPPER MATCHES "^(DEBUG|RELWITHDEBINFO)$")
    set(CPACK_STRIP_FILES FALSE)
else ()
    set(CPACK_STRIP_FILES TRUE)
endif ()

#set(desktop.path applications)
#set(desktop.files resources/example.desktop)
#set(icon.path icons/hicolor/64x64/apps)
#set(icon.files resources/icon64.png)
#set(iconsvg.path icons/hicolor/scalable/apps)
#set(iconsvg.files resources/icon.svg)
#foreach(items IN ITEMS desktop icon iconsvg)
#    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${${items}.files}
#            DESTINATION share/${${items}.path}
#            PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
#endforeach()

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/qwerty.json ${CMAKE_CURRENT_SOURCE_DIR}/ test.json
            DESTINATION .
            PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
    include(Packaging.cmake)
endif ()

