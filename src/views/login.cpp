#include "views/login.hpp"
#include "include/state.h"
#include "include/viewport.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QView(parent),
    Logger("LoginView"),
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
    Canvas::Session &session = State::singleton().getSession();
    Canvas::Student &student = State::singleton().getStudent();

    setStatus(tr("Authenticating..."));

    debug() << "authentication token:"
            << ui->txtToken->text().toStdString();

    student.setApiToken(ui->txtToken->text().toStdString());
    session.authenticate(student);

    student.loadIdentity(session, [&](bool success) {
        setStatus(tr("Authentication ") + tr(success ? "succeeded." : "failed"));

        if (success) {
            Viewport::singleton().transition("AvailableQuizzes");
        }
        else {
            reportError(tr("Authentication failed."), "authentication_error");
        }
    });
}

void Login::on_txtToken_returnPressed()
{
//    ui->btnLogin->click();
}
