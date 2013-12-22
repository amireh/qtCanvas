#include "include/take_quiz.hpp"
#include "ui_take_quiz.h"

TakeQuiz::TakeQuiz(QWidget *parent) :
    QView(parent),
    ui(new Ui::TakeQuiz)
{
    ui->setupUi(this);
}

TakeQuiz::~TakeQuiz()
{
    delete ui;
}
