#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include "include/state.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    Viewport &viewport = Viewport::singleton();
    ui->setupUi(this);

    QObject::connect(ui->actionLogout, SIGNAL(triggered()),
                     this, SLOT(logout()));

    viewport.setLayout(ui->centralWidget->layout());
    viewport.setStatusBar(ui->statusBar);

    viewport.registerView("Login", []() -> QView* { return new Login; });
    viewport.registerView("AvailableQuizzes", []() -> QView* { return new AvailableQuizzes; });
    viewport.registerView("TakeQuiz", []() -> QView* { return new TakeQuiz; });

    viewport.transition("Login");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete &State::singleton();
}

void MainWindow::logout()
{
    Viewport::singleton().transition("Login");
    State::singleton().emit loggedOut();
    State::singleton().reset();
}
