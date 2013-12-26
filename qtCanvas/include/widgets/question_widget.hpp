#ifndef QUESTION_WIDGET_HPP
#define QUESTION_WIDGET_HPP

#include <QFrame>
#include <canvas/resources/quiz_question.hpp>

using Canvas::QuizQuestion;

class QuestionWidget : public QFrame
{
    Q_OBJECT
public:
    explicit QuestionWidget(QWidget *parent, QuizQuestion *);

    QuizQuestion* question() const;
signals:
    void focused(QuestionWidget*);

protected:
    QuizQuestion *mQuestion;
    virtual void enterEvent(QEvent*);


};

#endif // QUESTION_WIDGET_HPP
