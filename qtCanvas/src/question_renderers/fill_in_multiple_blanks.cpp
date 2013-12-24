#include "question_renderers/fill_in_multiple_blanks.hpp"
#include "type_exports.hpp"

namespace QuestionRenderers {
    const QRegExp FillInMultipleBlanks::gBlankPattern = QRegExp("(\\[[\\w|_]+\\])");

    FillInMultipleBlanks::FillInMultipleBlanks(QuizQuestion *qq)
        : QuestionRenderer(qq), Canvas::Logger("FillInMultipleBlanksRenderer")
    {

    }

    FillInMultipleBlanks::~FillInMultipleBlanks()
    {

    }

    void FillInMultipleBlanks::render(QWidget *widget)
    {
        FillInMultipleBlanksQuestion const* qq = question();
        QString qqText = QString::fromStdString(qq->text());
        QLayout *layout = widget->layout();

        Fragments fragments = extractFragments(qqText);

        for (Fragment &fragment : fragments) {
            QLabel *pretextLabel = new QLabel(widget);
            pretextLabel->setTextFormat(Qt::RichText);
            pretextLabel->setText(fragment.pretext);

            layout->addWidget(pretextLabel);

            if (fragment.blankId.size()) {
                QLineEdit *blankEditor = new QLineEdit(widget);
                blankEditor->setText(
                            QString::fromStdString(
                                qq->filledAnswer(fragment.blankId.toStdString())));

                blankEditor->setProperty("blankId", fragment.blankId);
                layout->addWidget(blankEditor);

                QObject::connect(blankEditor, SIGNAL(editingFinished()),
                                 this, SLOT(fillAnswer()));
            }

            debug() << "pretext:" << fragment.pretext.toStdString()
                    << " => " << fragment.blankId.toStdString();
        }

    }

    bool FillInMultipleBlanks::hasRenderableText()
    {
        return false;
    }

    QLayout *FillInMultipleBlanks::renderLayout(QWidget *answerWidget)
    {
        return new QHBoxLayout(answerWidget);
    }

    void FillInMultipleBlanks::fillAnswer()
    {
        FillInMultipleBlanksQuestion *qq = question();
        QLineEdit *editor = static_cast<QLineEdit*>(QObject::sender());

        String answer(editor->text().toStdString());
        String blank(editor->property("blankId").toString().toStdString());

        if (answer == qq->filledAnswer(blank)) {
            return;
        }

        qq->fill(blank, answer);

        emit answerModified(qq);
    }

    FillInMultipleBlanks::Fragments FillInMultipleBlanks::extractFragments(const QString &text)
    {
        Fragment trailing;
        Fragments fragments;
        QRegExp const &pattern = gBlankPattern;
        int cursor = 0, lastCursor = 0;
//        size_t nrBlanks = qq->blanks().size();

        while (cursor != -1) {
            Fragment fragment;
            cursor = pattern.indexIn(text, cursor);

            if (cursor == -1) {
                break;
            }

            fragment.pretext = text.mid(lastCursor, cursor - lastCursor).trimmed();
            fragment.blankId = pattern.cap(1).replace(QRegExp("[\\[|\\]]"), "");
            fragments.push_back(fragment);

            lastCursor = cursor += pattern.matchedLength();
        }

        trailing.pretext = text.mid(lastCursor, text.size() - lastCursor);

        fragments.push_back(trailing);

        return fragments;
    }

    FillInMultipleBlanksQuestion *FillInMultipleBlanks::question() const
    {
        return mQuestion->asFillInMultipleBlanks();
    }

}
