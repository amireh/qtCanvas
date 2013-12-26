#include "include/widgets/question_widget.hpp"

QuestionWidget::QuestionWidget(QWidget *parent, Canvas::QuizQuestion *question)
    : QWidget(parent), mQuestion(question)
{

}

Canvas::QuizQuestion *QuestionWidget::question() const
{
    return mQuestion;
}

void QuestionWidget::enterEvent(QEvent *)
{
    qDebug("mouse entered for question widget");
    emit focused(this);
}
