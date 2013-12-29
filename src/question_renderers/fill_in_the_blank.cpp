#include "question_renderers/fill_in_the_blank.hpp"
#include "type_exports.hpp"

namespace QuestionRenderers {
    FillInTheBlank::FillInTheBlank(QuizQuestion *qq)
        : QuestionRenderer(qq), Canvas::Logger("FillInTheBlankRenderer")
    {

    }

    FillInTheBlank::~FillInTheBlank()
    {

    }

    void FillInTheBlank::render(QWidget *widget)
    {
        FillInTheBlankQuestion const* qq = question();
        QLayout *layout = widget->layout();
        mEditor = new QLineEdit(widget);
        mEditor->setText(QString::fromStdString(qq->filledAnswer()));
        layout->addWidget(mEditor);

        QObject::connect(mEditor, SIGNAL(editingFinished()),
                         this, SLOT(fillAnswer()));
    }

    void FillInTheBlank::fillAnswer()
    {
        FillInTheBlankQuestion *qq = question();
        String answer(mEditor->text().toStdString());

        if (answer == qq->filledAnswer()) {
            return;
        }

        qq->fill(answer);

        emit answerModified(qq);
    }

    FillInTheBlankQuestion *FillInTheBlank::question() const
    {
        return mQuestion->asFillInTheBlank();
    }

}
