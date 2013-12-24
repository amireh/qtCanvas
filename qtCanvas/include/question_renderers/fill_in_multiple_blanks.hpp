#ifndef FILL_IN_MULTIPLE_BLANKS_QUESTION_RENDERER_HPP
#define FILL_IN_MULTIPLE_BLANKS_QUESTION_RENDERER_HPP

#include <canvas/canvas.hpp>
#include <canvas/logger.hpp>
#include <canvas/resources/quiz_questions/fill_in_multiple_blanks.hpp>

#include "question_renderer.hpp"

#include <QVariant>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QHBoxLayout>

using namespace Canvas;

namespace QuestionRenderers {
    typedef QuizQuestions::FillInMultipleBlanks FillInMultipleBlanksQuestion;

    class FillInMultipleBlanks : public QuestionRenderer, public Canvas::Logger
    {
        Q_OBJECT
    public:
        FillInMultipleBlanks(QuizQuestion *qq);
        virtual ~FillInMultipleBlanks();

        virtual void render(QWidget*);

        /**
         * This question type already renders its text into QLabels
         * as parts of the answer body (along with the editor fields) so we skip
         * the generic question text rendering.
         *
         * @return false
         */
        virtual bool hasRenderableText();

    protected:

        /**
         * Builds a horizontal box layout.
         *
         * @param[in] answerWidget The answer "frame" widget.
         * @return HBoxLayout
         */
        virtual QLayout* renderLayout(QWidget *answerWidget);
    private slots:
        void fillAnswer();

    private:
        typedef struct {
            QString pretext;
            QString blankId;
        } Fragment;

        typedef std::vector<Fragment> Fragments;
        Fragments extractFragments(QString const&);

        FillInMultipleBlanksQuestion* question() const;
        static const QRegExp gBlankPattern;
    };
}

#endif // FILL_IN_MULTIPLE_BLANKS_QUESTION_RENDERER_HPP
