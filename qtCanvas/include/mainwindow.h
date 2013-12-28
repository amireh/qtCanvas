#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>

#include "include/viewport.h"
#include "include/login.h"
#include "include/available_quizzes.h"
#include "include/take_quiz.hpp"
#include "include/dialogs/settings_dialog.hpp"
#include "include/dialogs/about_dialog.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent, QApplication&);
    ~MainWindow();

public slots:
    void logout();
    void showSettingsDialog();
    void showAboutDialog();

private:
    QApplication &mApplication;
    Ui::MainWindow *ui;

    void bindMenuActions();
};

#endif // MAINWINDOW_H
