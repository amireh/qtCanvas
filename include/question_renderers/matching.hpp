#ifndef MATCHING_QUESTION_RENDERER_HPP
#define MATCHING_QUESTION_RENDERER_HPP

#include <canvas/canvas.hpp>
#include <canvas/logger.hpp>
#include <canvas/resources/quiz_questions/matching.hpp>
#include <canvas/resources/quiz_submission.hpp>

#include "question_renderer.hpp"

#include <QVariant>
#include <QLayout>
#include <QLabel>
#include <QFormLayout>
#include <QComboBox>

using namespace Canvas;

namespace QuestionRenderers {
    typedef QuizQuestions::Matching MatchingQuestion;

    class Matching : public QuestionRenderer, public Canvas::Logger
    {
        Q_OBJECT
    public:
        Matching(QuizQuestion *qq);
        virtual ~Matching();

        virtual void render(QWidget*);
        virtual QLayout* renderLayout(QWidget *answerWidget);
    private slots:
        void chooseMatch(int);

    protected:
        MatchingQuestion* question() const;
        QComboBox *createMatchCombo() const;
    };
}

#endif // MATCHING_QUESTION_RENDERER_HPP
