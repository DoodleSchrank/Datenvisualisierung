#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VisualizationAndGPUComputing
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS += -O2 -fopenmp
QMAKE_LFLAGS += -fopenmp

SOURCES += \
    main.cpp \
    horizontalslicetocontourlinemapper.cpp \
    horizontalcontourlinesrenderer.cpp \
    horizontalslicetoimagemapper.cpp \
    horizontalslicerenderer.cpp \
    mainwindow.cpp \
    opengldisplaywidget.cpp \
    datavolumeboundingboxrenderer.cpp \
    flowdatasource.cpp \
    streamlinesmapper.cpp \
    streamlinesrenderer.cpp

HEADERS += \
    horizontalcontourlinesrenderer.h \
    horizontalslicerenderer.h \
    horizontalslicetocontourlinemapper.h \
    horizontalslicetoimagemapper.h \
    mainwindow.h \
    opengldisplaywidget.h \
    datavolumeboundingboxrenderer.h \
    flowdatasource.h \
    streamlinesmapper.h \
    streamlinesrenderer.h

DISTFILES += \
    contour_fshader.glsl \
    contour_vshader.glsl \
    lines_fshader.glsl \
    lines_vshader.glsl \
    slice_fshader.glsl \
    slice_vshader.glsl \
    stream_fshader.glsl \
    stream_vshader.glsl
