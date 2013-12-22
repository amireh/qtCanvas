#ifndef TAKE_QUIZ_HPP
#define TAKE_QUIZ_HPP

#include "include/qview.hpp"

namespace Ui {
class TakeQuiz;
}

class TakeQuiz : public QView
{
    Q_OBJECT

public:
    explicit TakeQuiz(QWidget *parent = 0);
    ~TakeQuiz();

private:
    Ui::TakeQuiz *ui;
};

#endif // TAKE_QUIZ_HPP
