#include "question_renderers/essay.hpp"
#include "type_exports.hpp"

namespace QuestionRenderers {
    Essay::Essay(QuizQuestion *qq)
        : QuestionRenderer(qq), Canvas::Logger("EssayRenderer")
    {

    }

    Essay::~Essay()
    {

    }

    void Essay::render(QWidget *widget)
    {
        EssayQuestion const* qq = question();
        QLayout *layout = widget->layout();
        mEditor = new qtCanvas::QTextEdit(widget);
        mEditor->setText(QString::fromStdString(qq->filledAnswer()));
        mEditor->setAcceptRichText(true);
        mEditor->setAutoFormatting(QTextEdit::AutoAll);
        layout->addWidget(mEditor);

        QObject::connect(mEditor, SIGNAL(editingFinished()),
                         this, SLOT(fillAnswer()));
    }

    void Essay::fillAnswer()
    {
        EssayQuestion *qq = question();
        String answer(mEditor->document()->toPlainText().toStdString());

        if (answer == qq->filledAnswer()) {
            return;
        }

        qq->fill(answer);

        emit answerModified(qq);
    }

    EssayQuestion *Essay::question() const
    {
        return mQuestion->asEssay();
    }

}
