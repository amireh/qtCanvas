#ifndef LOGIN_H
#define LOGIN_H

#include "include/qview.hpp"
#include <canvas/logger.hpp>

namespace Ui {
class Login;
}

class Login : public QView, public Canvas::Logger
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    void on_btnLogin_clicked();

    void on_txtToken_returnPressed();

private:
    Ui::Login *ui;
};

//Q_DECLARE_METATYPE(Login);

#endif // LOGIN_H
