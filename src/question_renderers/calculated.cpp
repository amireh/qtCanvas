#include "question_renderers/calculated.hpp"
#include "type_exports.hpp"
#include <QDoubleValidator>
#include "state.h"

namespace QuestionRenderers {
    class MyDoubleValidator : public QDoubleValidator
    {
    public:
        MyDoubleValidator( QObject* parent = 0)
           : QDoubleValidator( parent) {}
       MyDoubleValidator( double bottom, double top, int decimals, QObject* parent = 0)
          : QDoubleValidator( bottom, top, decimals, parent)
       {}

       QValidator::State validate(QString & input, int &) const
       {
          const double b = bottom();
          const double t = top();
          const int d = decimals();

          QRegExp empty(QString::fromLatin1("-?\\.?"));
          if (input.contains(' '))
             return Invalid;
          if (b >= 0 && input.startsWith(QLatin1Char('-')))
             return Invalid;
          if (empty.exactMatch(input))
             return Intermediate;

          double entered = input.toDouble();
          int nume = input.count('e', Qt::CaseInsensitive);

          int i;
          if (input.contains(','))
             i = input.indexOf(',');
          else
             i = input.indexOf('.');

          if (i >= 0 && nume == 0) {
             i++;
             int j = i;
             while(input[j].isDigit())
                j++;
             if (j - i > d)
                return Invalid;
          }

          if (entered < b || entered > t)
             return Invalid;

          return Acceptable;
       }
    };

    Calculated::Calculated(QuizQuestion *qq)
        : QuestionRenderer(qq), Canvas::Logger("CalculatedRenderer")
    {

    }

    Calculated::~Calculated()
    {

    }

    void Calculated::render(QWidget *widget)
    {
        CalculatedQuestion const* qq = question();
        QLayout *layout = widget->layout();
        mEditor = new QLineEdit(widget);
        mEditor->setText(QString("%1").arg(qq->answer()));
        mEditor->setValidator(new MyDoubleValidator(mEditor));
        layout->addWidget(mEditor);

        QObject::connect(mEditor, SIGNAL(editingFinished()),
                         this, SLOT(fillAnswer()));
    }

    QWidget *Calculated::renderText(QWidget *widget, QString text)
    {
        text = QString::fromStdString(mQuestion->text());

        substitute(text);

        return QuestionRenderer::renderText(widget, text);
    }

    void Calculated::fillAnswer()
    {
        CalculatedQuestion *qq = question();
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

    CalculatedQuestion *Calculated::question() const
    {
        return mQuestion->asCalculated();
    }

    void Calculated::substitute(QString &text)
    {
        Canvas::QuizSubmission *qs = State::singleton().activeQuizSubmission();

        for (auto variable : question()->variables()) {
            double value = question()->variableValue(*qs, variable);
            text.replace(
                        QString("[%1]").arg(QString::fromStdString(variable.name)),
                        QString("%1").arg(value));
        }
    }

}
