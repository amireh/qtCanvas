#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QErrorMessage>
#include <canvas/logger.hpp>

namespace Ui {
class Login;
}

class Login : public QWidget, public Canvas::Logger
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    void on_btnLogin_clicked();

private:
    Ui::Login *ui;
    QErrorMessage *errorMessageDialog;
};

#endif // LOGIN_H
