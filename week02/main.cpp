#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>


int main(int argc, char *argv[])
{
    // We stick with the standard "main" program that QtCreator generates
    // upon creation of a new "Qt Widgets Application". It creates a single
    // main window into which we can embed the OpenGL display.

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
