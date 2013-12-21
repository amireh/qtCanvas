#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include "include/state.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    loginView(new Login),
    availableQuizzesView(new AvailableQuizzes)
{
    Viewport &viewport = Viewport::singleton();
    ui->setupUi(this);

    QObject::connect(ui->actionLogout, SIGNAL(triggered()),
                     this, SLOT(logout()));

    viewport.setLayout(ui->centralWidget->layout());
    viewport.setStatusBar(ui->statusBar);

    viewport.registerView("login", loginView);
    viewport.registerView("available_quizzes", availableQuizzesView);
    viewport.transition("login");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete loginView;
    delete availableQuizzesView;
    delete &State::singleton();
}

void MainWindow::logout()
{
    Viewport::singleton().transition("login");
    State::singleton().emit loggedOut();
    State::singleton().reset();
}
