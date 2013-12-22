#include "include/login.h"
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
    Viewport &viewport = Viewport::singleton();

    viewport.setStatus(tr("Authenticating..."));

    debug() << "authentication token:"
            << ui->txtToken->text().toStdString();

    student.setApiToken(ui->txtToken->text().toStdString());
    session.authenticate(student);
    student.loadIdentity(session, [&](bool success) {
        viewport.setStatus(tr("Authentication ") + tr(success ? "succeeded." : "failed"));

        if (success) {
            viewport.transition("available_quizzes");
        }
        else {
            Viewport::singleton().errorDialog()->showMessage(
                        tr("Authentication failed."),
                        "authentication_error");
        }
    });
}
