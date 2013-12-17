#include "include/login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_btnLogin_clicked()
{
//    qDebug(("logging in with:" + ui->txtToken->text().toStdString()).c_str());
//    session_.authenticate(ui->txtToken->text().toStdString());

//    session_.get("/courses/1", [](bool success, cnvs::http::response response) -> void {
//        qDebug(response.body.c_str());
//    });
}
