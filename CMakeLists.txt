cmake_minimum_required(VERSION 3.5)

project(Qt-thread-test VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui

#        toola.h        toola.cpp

        ./fake_send&recv/fake_SR.cpp
        ./fake_send&recv/fake_SR.h
        ./fake_send&recv/ntlog.cpp
        ./fake_send&recv/ntlog.h
)

set(DEMO
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

set(DEV
    ./fake_send&recv/fake_SR.cpp
    ./fake_send&recv/fake_SR.h
    ./fake_send&recv/ntlog.cpp
    ./fake_send&recv/ntlog.h
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)

qt_add_executable(Qt-thread-test
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Qt-thread-test APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Qt-thread-test SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Qt-thread-test
            ${PROJECT_SOURCES}
        )
    endif()
endif()

add_executable(Qt-thread-demo
    ${DEMO}
)

add_library(Qt-thread-dev SHARED
    ${DEV}
)

target_link_libraries(Qt-thread-test PRIVATE Qt${QT_VERSION_MAJOR}::Widgets -pthread)

target_link_libraries(Qt-thread-demo PRIVATE Qt${QT_VERSION_MAJOR}::Widgets -pthread -ldl)

target_link_libraries(Qt-thread-dev -ldl)

set_target_properties(Qt-thread-test PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Qt-thread-test)
endif()
