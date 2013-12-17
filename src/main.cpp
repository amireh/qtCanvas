#include "include/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setOrganizationName("Instructure");
    app.setApplicationName("qtCanvas");

    MainWindow w;
    w.show();

    return app.exec();
}
