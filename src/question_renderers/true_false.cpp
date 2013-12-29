#include "include/question_renderers/true_false.hpp"


namespace QuestionRenderers {
    TrueFalse::TrueFalse(QuizQuestion *qq)
        : QuestionRenderer(qq), Canvas::Logger("TrueFalseRenderer")
    {

    }

    TrueFalse::~TrueFalse()
    {

    }

    void TrueFalse::render(QWidget *widget)
    {
        TrueFalseQuestion const* qq = question();
        QButtonGroup *choices = new QButtonGroup(widget);
        QLayout *layout = widget->layout();

        debug() << "Rendering a TrueFalse question: " << qq->id();

        for (auto answer : qq->answers()) {
            QRadioButton *answerRadio =
                    new QRadioButton(QString::fromStdString(answer->text()),
                                     widget);
            answerRadio->setProperty("answerId", answer->id());
            answerRadio->setChecked(qq->chosenAnswer() == answer);

            choices->addButton(answerRadio);
            layout->addWidget(answerRadio);
        }

        QObject::connect(choices, SIGNAL(buttonReleased(QAbstractButton*)),
                         this, SLOT(chooseAnswer(QAbstractButton*)));
    }

    void TrueFalse::chooseAnswer(QAbstractButton *answerButton)
    {
        int answerId = answerButton->property("answerId").toInt();
        TrueFalseQuestion *qq = question();

        debug() << "choosing answer#" << answerId << "for question#" << qq->id();

        qq->choose(answerId);

        emit answerModified(qq);
    }

    TrueFalseQuestion *TrueFalse::question() const
    {
        return mQuestion->asTrueFalse();
    }

}
