#include "include/mainwindow.h"
#include <canvas/canvas.hpp>
#include <canvas/resources/quiz.hpp>
#include <QApplication>
#include "include/type_exports.hpp"

int main(int argc, char *argv[])
{
    QTCANVAS_EXPORT_TYPES

    int rc;
    Canvas::init(argc, argv);
    Canvas::Logger::unmute();

    QApplication app(argc, argv);

    app.setOrganizationName("Instructure");
    app.setApplicationName("qtCanvas");

    MainWindow w;
    w.show();

    rc = app.exec();

    Canvas::free();

    return rc;
}
