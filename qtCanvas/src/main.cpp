#include "include/mainwindow.h"
#include <canvas/canvas.hpp>
#include <canvas/resources/quiz.hpp>
#include <QApplication>
#include "include/type_exports.hpp"
#include "include/viewport.h"

int main(int argc, char *argv[])
{
    int rc;

    QTCANVAS_EXPORT_TYPES;

    Canvas::init(argc, argv);
    Canvas::Logger::unmute();

    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Algol Labs");
    QCoreApplication::setOrganizationDomain("algollabs.com");
    QCoreApplication::setApplicationName("qtCanvas");

    MainWindow w(nullptr, app);
    w.show();

    Viewport::singleton().transition("Login");

    rc = app.exec();

    Canvas::free();

    return rc;
}
