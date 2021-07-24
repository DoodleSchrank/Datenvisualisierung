cmake_minimum_required(VERSION 3.19)
project(week02)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)

include_directories(.)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)

add_executable(week02
        opengldisplaywidget.h
        mainwindow.h
        datavolumeboundingboxrenderer.h
        flowdatasource.h
        horizontalcontourlinesrenderer.h
        horizontalslicerenderer.h
        horizontalslicetocontourlinemapper.h
        horizontalslicetoimagemapper.h
        streamlinesmapper.h
        streamlinesrenderer.h
        datavolumeboundingboxrenderer.cpp
        flowdatasource.cpp
        horizontalcontourlinesrenderer.cpp
        horizontalslicerenderer.cpp
        horizontalslicetocontourlinemapper.cpp
        horizontalslicetoimagemapper.cpp
        main.cpp
        mainwindow.cpp
        opengldisplaywidget.cpp
        streamlinesmapper.cpp
        streamlinesrenderer.cpp)

target_link_libraries(week02 Qt5::Widgets)
target_link_libraries(week02 Qt5::Core)
