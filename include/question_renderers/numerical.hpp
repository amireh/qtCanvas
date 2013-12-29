#ifndef NUMERICAL_QUESTION_RENDERER_HPP
#define NUMERICAL_QUESTION_RENDERER_HPP

#include <canvas/canvas.hpp>
#include <canvas/logger.hpp>
#include <canvas/resources/quiz_questions/numerical.hpp>

#include "question_renderer.hpp"

#include <QVariant>
#include <QLayout>
#include <QLineEdit>

using namespace Canvas;

namespace QuestionRenderers {
    typedef QuizQuestions::Numerical NumericalQuestion;

    class Numerical : public QuestionRenderer, public Canvas::Logger
    {
        Q_OBJECT
    public:
        Numerical(QuizQuestion *qq);
        virtual ~Numerical();

        virtual void render(QWidget*);

    private slots:
        void fillAnswer();

    protected:
        QLineEdit *mEditor;
        NumericalQuestion* question() const;
    };
}

#endif // NUMERICAL_QUESTION_RENDERER_HPP
