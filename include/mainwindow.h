#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "include/viewport.h"
#include "include/login.h"
#include "include/available_quizzes.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void logout();

private:
    Ui::MainWindow *ui;
    Login *loginView;
    AvailableQuizzes *availableQuizzesView;
};

#endif // MAINWINDOW_H
