cmake_minimum_required(VERSION 3.21)

project(SortingAlgorithmsVisualization LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if(APPLE AND NOT DEFINED CMAKE_OSX_DEPLOYMENT_TARGET)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "13.0" CACHE STRING "" FORCE)
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(SortingAlgorithmsVisualization
    MACOSX_BUNDLE
    WIN32
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    widget.cpp
    widget.h
    widget.ui
    graphwidget.cpp
    graphwidget.h
    binarysearchwidget.cpp
    binarysearchwidget.h
    qt_arm_acle_workaround.h
    resource.qrc
)

target_link_libraries(SortingAlgorithmsVisualization PRIVATE Qt6::Widgets)

if(APPLE AND CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    target_compile_options(SortingAlgorithmsVisualization PRIVATE
        "-include"
        "${CMAKE_CURRENT_SOURCE_DIR}/qt_arm_acle_workaround.h"
    )
endif()

include(GNUInstallDirs)

install(TARGETS SortingAlgorithmsVisualization
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
