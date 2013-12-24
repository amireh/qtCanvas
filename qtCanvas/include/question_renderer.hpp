#ifndef QUESTION_RENDERER_HPP
#define QUESTION_RENDERER_HPP

#include <QWidget>
#include <QObject>
#include <canvas/resources/quiz_question.hpp>

using namespace Canvas;

class QuestionRenderer : public QObject
{
    Q_OBJECT
public:
    QuestionRenderer(QuizQuestion*);
    virtual ~QuestionRenderer();

    virtual void render(QWidget* baseWidget) = 0;
signals:
    void answerModified(QuizQuestion const*);
protected:
    QuizQuestion* mQuestion;
};

#endif // QUESTION_RENDERER_HPP
