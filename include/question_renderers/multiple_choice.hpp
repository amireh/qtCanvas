#ifndef MULTIPLE_CHOICE_QUESTION_RENDERER_HPP
#define MULTIPLE_CHOICE_QUESTION_RENDERER_HPP

#include <canvas/canvas.hpp>
#include <canvas/logger.hpp>
#include <canvas/resources/quiz_questions/multiple_choice.hpp>

#include "question_renderer.hpp"

#include <QVariant>
#include <QLabel>
#include <QRadioButton>
#include <QGroupBox>
#include <QButtonGroup>
#include <QLayout>

using namespace Canvas;

namespace QuestionRenderers {
    typedef QuizQuestions::MultipleChoice MultipleChoiceQuestion;

    class MultipleChoice : public QuestionRenderer, public Canvas::Logger
    {
        Q_OBJECT
    public:
        MultipleChoice(QuizQuestion *qq);
        virtual ~MultipleChoice();

        virtual QLayout *renderLayout(QWidget *answerWidget);
        virtual void render(QWidget*);
    private slots:
        void chooseAnswer(QAbstractButton *answerButton);

    private:
        MultipleChoiceQuestion* question() const;
    };
}

#endif // QUESTION_RENDERER_HPP
