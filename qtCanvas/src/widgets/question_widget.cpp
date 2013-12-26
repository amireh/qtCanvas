#include "include/widgets/question_widget.hpp"

QuestionWidget::QuestionWidget(QWidget *parent, Canvas::QuizQuestion *question)
    : QFrame(parent), mQuestion(question)
{

}

Canvas::QuizQuestion *QuestionWidget::question() const
{
    return mQuestion;
}

void QuestionWidget::enterEvent(QEvent *)
{
    emit focused(this);
}
