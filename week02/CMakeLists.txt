cmake_minimum_required(VERSION 3.19)
project(week02)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(week02
        datavolumeboundingboxrenderer.cpp
        datavolumeboundingboxrenderer.h
        flowdatasource.cpp
        flowdatasource.h
        horizontalcontourlinesrenderer.cpp
        horizontalcontourlinesrenderer.h
        horizontalslicerenderer.cpp
        horizontalslicerenderer.h
        horizontalslicetocontourlinemapper.cpp
        horizontalslicetocontourlinemapper.h
        horizontalslicetoimagemapper.cpp
        horizontalslicetoimagemapper.h
        main.cpp
        mainwindow.cpp
        mainwindow.h
        opengldisplaywidget.cpp
        opengldisplaywidget.h
        streamlinesmapper.cpp
        streamlinesmapper.h
        streamlinesrenderer.cpp
        streamlinesrenderer.h)

target_link_libraries(week02 Qt5::Widgets)
