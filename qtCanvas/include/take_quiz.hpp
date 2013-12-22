#ifndef TAKE_QUIZ_HPP
#define TAKE_QUIZ_HPP

#include "include/qview.hpp"
#include "include/state.h"
#include <canvas/resources/quiz.hpp>
#include <canvas/resources/quiz_question.hpp>

namespace Ui {
class TakeQuiz;
}

class TakeQuiz : public QView
{
    Q_OBJECT

public:
    explicit TakeQuiz(QWidget *parent = 0);
    ~TakeQuiz();

    virtual void setup();
    virtual void cleanup();

private:
    Ui::TakeQuiz *ui;
    Canvas::Quiz *mQuiz;
};

#endif // TAKE_QUIZ_HPP
