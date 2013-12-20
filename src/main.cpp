#include "include/mainwindow.h"
#include <canvas/canvas.hpp>
#include <QApplication>

int main(int argc, char *argv[])
{
    int rc;
    Canvas::init(argc, argv);

    QApplication app(argc, argv);

    app.setOrganizationName("Instructure");
    app.setApplicationName("qtCanvas");

    MainWindow w;
    w.show();

    rc = app.exec();

    Canvas::free();

    return rc;
}
