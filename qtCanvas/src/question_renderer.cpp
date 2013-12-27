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
    layout->setContentsMargins(0,0,0,0);
    layout->setSizeConstraint(QLayout::SetMinimumSize);

    return layout;
}

QWidget* QuestionRenderer::renderText(QWidget *widget, QString text) {
    if (!text.size()) {
        text = QString::fromStdString(mQuestion->text());
    }
    // Question text:
    // [= %text =]
    QLabel *textLabel = new QLabel(text, widget);
    textLabel->setContentsMargins(5,5,5,5);
    textLabel->setTextFormat(Qt::RichText);

    return textLabel;
}
