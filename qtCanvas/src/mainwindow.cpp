#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include "include/state.h"

MainWindow::MainWindow(QWidget *parent, QApplication &app) :
    QMainWindow(parent),
    mApplication(app),
    ui(new Ui::MainWindow)
{
    Viewport &viewport = Viewport::singleton();
    ui->setupUi(this);

    bindMenuActions();

    viewport.setLayout(ui->centralWidget->layout());
    viewport.setStatusBar(ui->statusBar);

    viewport.registerView("Login", []() -> QView* { return new Login; });
    viewport.registerView("AvailableQuizzes", []() -> QView* { return new AvailableQuizzes; });
    viewport.registerView("TakeQuiz", []() -> QView* { return new TakeQuiz; });
    viewport.registerDialog("Settings", []() -> QDialog* { return new SettingsDialog; });
    viewport.registerDialog("About", []() -> QDialog* { return new AboutDialog; });

    QFile css(":/qtCanvas.css");

    if (css.open(QIODevice::ReadOnly | QIODevice::Text)) {
        mApplication.setStyleSheet(css.readAll());
        css.close();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete &Viewport::singleton();
    delete &State::singleton();
}

void MainWindow::logout()
{
    Viewport::singleton().transition("Login");
    State::singleton().emit loggedOut();
    State::singleton().reset();
}

void MainWindow::showSettingsDialog()
{
    Viewport::singleton().showDialog("Settings");
}

void MainWindow::showAboutDialog()
{
    Viewport::singleton().showDialog("About");
}

void MainWindow::bindMenuActions()
{
    connect(ui->actionLogout, SIGNAL(triggered()), this, SLOT(logout()));
    connect(ui->actionOptions, SIGNAL(triggered()),
            this, SLOT(showSettingsDialog()));
    connect(ui->actionA_bout_qtCanvas, SIGNAL(triggered()),
            this, SLOT(showAboutDialog()));
}
