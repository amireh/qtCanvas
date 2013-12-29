#include "include/widgets/question_widget.hpp"
#include <QComboBox>
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

bool QuestionWidget::eventFilter( QObject * o, QEvent * e ) {
    if ( e->type() == QEvent::Wheel && qobject_cast<QComboBox*>( o ) ) {
        e->ignore();
        return true;
    }

    return QWidget::eventFilter( o, e );
}
