#ifndef TRUE_FALSE_QUESTION_RENDERER_HPP
#define TRUE_FALSE_QUESTION_RENDERER_HPP

#include <canvas/canvas.hpp>
#include <canvas/logger.hpp>
#include <canvas/resources/quiz_questions/true_false.hpp>

#include "question_renderer.hpp"

#include <QVariant>
#include <QRadioButton>
#include <QLabel>
#include <QButtonGroup>
#include <QLayout>

using namespace Canvas;
namespace QuestionRenderers {
    typedef QuizQuestions::TrueFalse TrueFalseQuestion;

    class TrueFalse : public QuestionRenderer, public Canvas::Logger
    {
        Q_OBJECT
    public:
        TrueFalse(QuizQuestion *qq);
        virtual ~TrueFalse();

        virtual void render(QWidget*);

    private slots:
        void chooseAnswer(QAbstractButton *answerButton);

    private:
        TrueFalseQuestion* question() const;
    };
}
#endif // TRUE_FALSE_HPP
