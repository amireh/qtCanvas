#include "include/login.h"
#include "include/state.h"
#include "include/viewport.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    Logger("LoginView"),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    errorMessageDialog = new QErrorMessage(this);
}

Login::~Login()
{
    delete ui;
    delete errorMessageDialog;
}

void Login::on_btnLogin_clicked()
{
    State &state = State::singleton();
    Canvas::Session &session = State::singleton().getSession();
    Canvas::Student &student = State::singleton().getStudent();
    Viewport &viewport = Viewport::singleton();

    viewport.setStatus(tr("Authenticating..."));

    debug() << "authentication token:"
            << ui->txtToken->text().toStdString();

    student.setApiToken(ui->txtToken->text().toStdString());
    session.authenticate(student);

    student.loadCourses(session, [&](bool success) -> void {
        viewport.setStatus(tr("Authentication ") + tr(success ? "succeeded." : "failed"));

        if (success) {
            for (Canvas::Course *course : student.courses()) {
                state.emit courseAdded(*course);
                debug() << "new course:" << course->id();
            }

            viewport.transition("available_quizzes");
        }
        else {
            errorMessageDialog->showMessage(tr("Authentication failed."),
                                            "authentication_error");
        }
    });
}
