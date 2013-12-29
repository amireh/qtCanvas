#include "question_renderers/numerical.hpp"
#include "type_exports.hpp"

namespace QuestionRenderers {
    Numerical::Numerical(QuizQuestion *qq)
        : QuestionRenderer(qq), Canvas::Logger("NumericalRenderer")
    {

    }

    Numerical::~Numerical()
    {

    }

    void Numerical::render(QWidget *widget)
    {
        NumericalQuestion const* qq = question();
        QLayout *layout = widget->layout();
        mEditor = new QLineEdit(widget);
        mEditor->setText(QString("%1").arg(qq->answer()));
        layout->addWidget(mEditor);

        QObject::connect(mEditor, SIGNAL(editingFinished()),
                         this, SLOT(fillAnswer()));
    }

    void Numerical::fillAnswer()
    {
        NumericalQuestion *qq = question();
        bool conversionOk = false;
        double answer(mEditor->text().toDouble(&conversionOk));

        if (!conversionOk) {
            error() << "Bad number, could not be coerced into double:" << mEditor->text().toStdString();
            return;
        }

        if (answer == qq->answer()) {
            return;
        }

        qq->setAnswer(answer);

        emit answerModified(qq);
    }

    NumericalQuestion *Numerical::question() const
    {
        return mQuestion->asNumerical();
    }

}
