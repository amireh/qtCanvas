#ifndef MULTIPLE_ANSWERS_QUESTION_RENDERER_HPP
#define MULTIPLE_ANSWERS_QUESTION_RENDERER_HPP

#include <canvas/canvas.hpp>
#include <canvas/logger.hpp>
#include <canvas/resources/quiz_questions/multiple_answers.hpp>

#include "question_renderer.hpp"

#include <QVariant>
#include <QCheckBox>
#include <QLabel>
#include <QGroupBox>
#include <QButtonGroup>
#include <QLayout>

using namespace Canvas;

namespace QuestionRenderers {
    typedef QuizQuestions::MultipleAnswers MultipleAnswersQuestion;

    class MultipleAnswers : public QuestionRenderer, public Canvas::Logger
    {
        Q_OBJECT
    public:
        MultipleAnswers(QuizQuestion *qq);
        virtual ~MultipleAnswers();

        virtual void render(QWidget*);

    private slots:
        void toggleAnswerSelection(QAbstractButton *answerButton);

    private:
        MultipleAnswersQuestion* question() const;
    };
}

#endif // QUESTION_RENDERER_HPP
