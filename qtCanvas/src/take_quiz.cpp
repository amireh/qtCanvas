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

void TakeQuiz::setup()
{
    mQuiz = State::singleton().activeQuiz();

    for (auto question : mQuiz->questions()) {
        QWidget *qqWidget = new QWidget(this);
        question->setUserData<QWidget>("QWidget", qqWidget);
    }
}

void TakeQuiz::cleanup()
{
    for (auto question : mQuiz->questions()) {
        QWidget *qqWidget = question->userData<QWidget>("QWidget");
        assert(qqWidget);
        delete qqWidget;
        question->clearUserData("QWidget");
    }
    mQuiz = nullptr;
}
