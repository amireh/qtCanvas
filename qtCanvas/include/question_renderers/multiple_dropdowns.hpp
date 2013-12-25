#ifndef MULTIPLE_DROPDOWNS_QUESTION_RENDERER_HPP
#define MULTIPLE_DROPDOWNS_QUESTION_RENDERER_HPP

#include <canvas/canvas.hpp>
#include <canvas/logger.hpp>
#include <canvas/resources/quiz_questions/multiple_dropdowns.hpp>

#include "question_renderer.hpp"

#include <QVariant>
#include <QLayout>
#include <QLabel>
#include <QComboBox>
#include <QFormLayout>
#include <QHBoxLayout>

using namespace Canvas;

namespace QuestionRenderers {
    typedef QuizQuestions::MultipleDropdowns MultipleDropdownsQuestion;

    class MultipleDropdowns : public QuestionRenderer, public Canvas::Logger
    {
        Q_OBJECT
    public:
        MultipleDropdowns(QuizQuestion *qq);
        virtual ~MultipleDropdowns();

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
        void chooseAnswer(int);

    private:
        typedef struct {
            QString pretext;
            QString blankId;
        } Fragment;

        typedef std::vector<Fragment> Fragments;
        Fragments extractFragments(QString const&);

        MultipleDropdownsQuestion* question() const;
        static const QRegExp gBlankPattern;
    };
}

#endif // MULTIPLE_DROPDOWNS_QUESTION_RENDERER_HPP
