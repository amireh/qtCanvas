#include "question_renderers/multiple_answers.hpp"
#include "type_exports.hpp"

namespace QuestionRenderers {
    MultipleAnswers::MultipleAnswers(QuizQuestion *qq)
        : QuestionRenderer(qq), Canvas::Logger("MultipleAnswersRenderer")
    {

    }

    MultipleAnswers::~MultipleAnswers()
    {

    }

    void MultipleAnswers::render(QWidget *widget)
    {
        MultipleAnswersQuestion const* qq = question();
        QButtonGroup *answerButtons = new QButtonGroup(widget);
        answerButtons->setExclusive(false);
        QLayout *layout = widget->layout();

        debug() << "Rendering a MultipleAnswers question: " << qq->id();

        for (auto answer : qq->answers()) {
            QCheckBox *answerRadio = new QCheckBox(QString::fromStdString(answer->text()), widget);
            answerRadio->setProperty("answerId", answer->id());
            answerRadio->setChecked(qq->isChosen(answer));
            layout->addWidget(answerRadio);
            answerButtons->addButton(answerRadio);
        }

        QObject::connect(answerButtons, SIGNAL(buttonReleased(QAbstractButton*)),
                         this, SLOT(toggleAnswerSelection(QAbstractButton*)));
    }

    void MultipleAnswers::toggleAnswerSelection(QAbstractButton *answerButton)
    {
        int answerId = answerButton->property("answerId").toInt();
        MultipleAnswersQuestion *qq = question();

        debug() << "choosing answer#" << answerId << "for question#" << qq->id();

        if (answerButton->isChecked()) {
            if (qq->isChosen(answerId)) {
                return;
            }

            qq->choose(answerId);
        }
        else {
            if (!qq->isChosen(answerId)) {
                return;
            }

            qq->unchoose(answerId);
        }

        emit answerModified(qq);
    }

    MultipleAnswersQuestion *MultipleAnswers::question() const
    {
        return mQuestion->asMultipleAnswers();
    }

}
