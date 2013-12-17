#include "include/available_quizzes.h"
#include "ui_available_quizzes.h"

AvailableQuizzes::AvailableQuizzes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AvailableQuizzes)
{
    ui->setupUi(this);
}

AvailableQuizzes::~AvailableQuizzes()
{
    delete ui;
}
