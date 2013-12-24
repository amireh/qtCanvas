#include "question_renderer.hpp"

QuestionRenderer::QuestionRenderer(QuizQuestion* question)
    : mQuestion(question)
{
}

QuestionRenderer::~QuestionRenderer()
{

}

bool QuestionRenderer::hasRenderableText()
{
    return true;
}

QWidget *QuestionRenderer::renderFrame(QWidget *qqWidget)
{
    QWidget *widget = new QWidget(qqWidget);

    renderLayout(widget);

    return widget;
}

QLayout *QuestionRenderer::renderLayout(QWidget *answerWidget)
{
    QGridLayout *layout;

    layout = new QGridLayout(answerWidget);
    layout->setContentsMargins(0,5,5,5);

    return layout;
}
