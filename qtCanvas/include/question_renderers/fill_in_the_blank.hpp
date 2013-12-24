#ifndef FILL_IN_THE_BLANK_QUESTION_RENDERER_HPP
#define FILL_IN_THE_BLANK_QUESTION_RENDERER_HPP

#include <canvas/canvas.hpp>
#include <canvas/logger.hpp>
#include <canvas/resources/quiz_questions/fill_in_the_blank.hpp>

#include "question_renderer.hpp"

#include <QVariant>
#include <QLayout>
#include <QLineEdit>

using namespace Canvas;

namespace QuestionRenderers {
    typedef QuizQuestions::FillInTheBlank FillInTheBlankQuestion;

    class FillInTheBlank : public QuestionRenderer, public Canvas::Logger
    {
        Q_OBJECT
    public:
        FillInTheBlank(QuizQuestion *qq);
        virtual ~FillInTheBlank();

        virtual void render(QWidget*);

    private slots:
        void fillAnswer();

    private:
        QLineEdit *mEditor;
        FillInTheBlankQuestion* question() const;
    };
}

#endif // FILL_IN_THE_BLANK_QUESTION_RENDERER_HPP
