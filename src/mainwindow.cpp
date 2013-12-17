#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include "include/state.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    loginWidget(new Login)
{
    ui->setupUi(this);
    Viewport::singleton().setLayout(ui->centralWidget->layout());
    Viewport::singleton().setContent(loginWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete loginWidget;
    delete &State::singleton();
}
