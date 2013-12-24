#ifndef CALCULATED_QUESTION_RENDERER_HPP
#define CALCULATED_QUESTION_RENDERER_HPP

#include <canvas/canvas.hpp>
#include <canvas/logger.hpp>
#include <canvas/resources/quiz_questions/calculated.hpp>
#include <canvas/resources/quiz_submission.hpp>

#include "question_renderer.hpp"

#include <QVariant>
#include <QLayout>
#include <QLineEdit>

using namespace Canvas;

namespace QuestionRenderers {
    typedef QuizQuestions::Calculated CalculatedQuestion;

    class Calculated : public QuestionRenderer, public Canvas::Logger
    {
        Q_OBJECT
    public:
        Calculated(QuizQuestion *qq);
        virtual ~Calculated();

        virtual void render(QWidget*);
        virtual QWidget *renderText(QWidget *widget, QString text="");

    private slots:
        void fillAnswer();

    protected:
        QLineEdit *mEditor;
        CalculatedQuestion* question() const;
        virtual void substitute(QString &);
    };
}

#endif // CALCULATED_QUESTION_RENDERER_HPP
