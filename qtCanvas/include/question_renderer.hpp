#ifndef QUESTION_RENDERER_HPP
#define QUESTION_RENDERER_HPP

#include <QWidget>
#include <QObject>
#include <canvas/resources/quiz_question.hpp>
#include <QGridLayout>
#include <QLabel>

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

    /**
     * Render a Label that displays the question's text.
     * @param[in] widget The question's widget.
     * @return The text widget you should add to a layout.
     */
    virtual QWidget *renderText(QWidget *widget, QString text="");
signals:
    void answerModified(QuizQuestion const*);
    void focused(QWidget*);
protected:
    virtual QLayout *renderLayout(QWidget *answerWidget);
    QuizQuestion* mQuestion;
};

#endif // QUESTION_RENDERER_HPP
