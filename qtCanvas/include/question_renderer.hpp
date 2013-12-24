#ifndef QUESTION_RENDERER_HPP
#define QUESTION_RENDERER_HPP

#include <QWidget>
#include <QObject>
#include <canvas/resources/quiz_question.hpp>
#include <QGridLayout>

using namespace Canvas;

class QuestionRenderer : public QObject
{
    Q_OBJECT
public:
    QuestionRenderer(QuizQuestion*);
    virtual ~QuestionRenderer();

    virtual void render(QWidget* baseWidget) = 0;
    virtual bool hasRenderableText();
    virtual QWidget *renderFrame(QWidget *qqWidget);

signals:
    void answerModified(QuizQuestion const*);
protected:
    virtual QLayout *renderLayout(QWidget *answerWidget);
    QuizQuestion* mQuestion;
};

#endif // QUESTION_RENDERER_HPP
