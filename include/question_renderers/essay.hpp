#ifndef ESSAY_QUESTION_RENDERER_HPP
#define ESSAY_QUESTION_RENDERER_HPP

#include <canvas/canvas.hpp>
#include <canvas/logger.hpp>
#include <canvas/resources/quiz_questions/essay.hpp>

#include "question_renderer.hpp"

#include <QVariant>
#include <QLayout>
#include "widgets/qtextedit.hpp"

using namespace Canvas;

namespace QuestionRenderers {
    typedef QuizQuestions::Essay EssayQuestion;

    class Essay : public QuestionRenderer, public Canvas::Logger
    {
        Q_OBJECT
    public:
        Essay(QuizQuestion *qq);
        virtual ~Essay();

        virtual void render(QWidget*);

    private slots:
        void fillAnswer();

    private:
        qtCanvas::QTextEdit *mEditor;
        EssayQuestion* question() const;
    };
}

#endif // ESSAY_QUESTION_RENDERER_HPP
