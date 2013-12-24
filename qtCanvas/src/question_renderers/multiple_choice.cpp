#include "question_renderers/multiple_choice.hpp"
#include "type_exports.hpp"

namespace QuestionRenderers {
    MultipleChoice::MultipleChoice(QuizQuestion *qq)
        : QuestionRenderer(qq), Canvas::Logger("MultipleChoiceRenderer")
    {

    }

    MultipleChoice::~MultipleChoice()
    {

    }

    void MultipleChoice::render(QWidget *widget)
    {
        MultipleChoiceQuestion const* qq = question();
        QButtonGroup *answerButtons = new QButtonGroup(widget);
        QLayout *layout = widget->layout();

        debug() << "Rendering a MultipleChoice question: " << qq->id();

        for (auto answer : qq->answers()) {
            QRadioButton *answerRadio = new QRadioButton(QString::fromStdString(answer->text()), widget);
            answerRadio->setProperty("answerId", answer->id());
//            answerRadio->setProperty("qq", QVariant::fromValue(PQuizQuestion(qq)));
            answerRadio->setChecked(qq->chosenAnswer() == answer);
            layout->addWidget(answerRadio);
            answerButtons->addButton(answerRadio);
        }

        QObject::connect(answerButtons, SIGNAL(buttonReleased(QAbstractButton*)),
                         this, SLOT(chooseAnswer(QAbstractButton*)));
    }

    void MultipleChoice::chooseAnswer(QAbstractButton *answerButton)
    {
        int answerId = answerButton->property("answerId").toInt();
        MultipleChoiceQuestion *qq = question();

        debug() << "choosing answer#" << answerId << "for question#" << qq->id();

        qq->choose(answerId);

        emit answerModified(qq);
    }

    MultipleChoiceQuestion *MultipleChoice::question() const
    {
        return mQuestion->asMultipleChoice();
    }

}
