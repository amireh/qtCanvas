#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>

#include "viewport.h"
#include "views/login.hpp"
#include "views/available_quizzes.hpp"
#include "views/take_quiz.hpp"
#include "views/take_quiz_oqaat.hpp"
#include "dialogs/settings_dialog.hpp"
#include "dialogs/about_dialog.hpp"

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
