#include "question_renderers/multiple_dropdowns.hpp"
#include "type_exports.hpp"

namespace QuestionRenderers {
    const QRegExp MultipleDropdowns::gBlankPattern = QRegExp("(\\[[\\w|_]+\\])");

    MultipleDropdowns::MultipleDropdowns(QuizQuestion *qq)
        : QuestionRenderer(qq), Canvas::Logger("MultipleDropdownsRenderer")
    {

    }

    MultipleDropdowns::~MultipleDropdowns()
    {
    }

    void MultipleDropdowns::render(QWidget *widget)
    {
        MultipleDropdownsQuestion const* qq = question();
        QString qqText = QString::fromStdString(qq->text());
        QLayout *layout = widget->layout();

        Fragments fragments = extractFragments(qqText);

        for (Fragment &fragment : fragments) {
            QLabel *pretextLabel = new QLabel(widget);
            pretextLabel->setTextFormat(Qt::RichText);
            pretextLabel->setText(fragment.pretext);

            layout->addWidget(pretextLabel);

            if (fragment.blankId.size()) {
                QComboBox *blankAnswers = new QComboBox(widget);
                const String blankId = fragment.blankId.toStdString();

                blankAnswers->addItem("-", 0);

                for (auto answer : qq->answers(blankId)) {
                    const QString answerText(answer->text().c_str());
                    const ID answerId(answer->id());

                    blankAnswers->addItem(answerText, answerId);
                }

                QuizQuestionAnswer const *currentAnswer = qq->chosenAnswer(blankId);

                if (currentAnswer) {
                    blankAnswers->setCurrentText(
                                QString::fromStdString(currentAnswer->text()));
                }

                blankAnswers->setProperty("blankId", fragment.blankId);
                blankAnswers->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
                layout->addWidget(blankAnswers);

                connect(blankAnswers, SIGNAL(currentIndexChanged(int)),
                                 this, SLOT(chooseAnswer(int)));
            }

            debug() << "pretext:" << fragment.pretext.toStdString()
                    << " => " << fragment.blankId.toStdString();
        }

    }

    bool MultipleDropdowns::hasRenderableText()
    {
        return false;
    }

    QLayout *MultipleDropdowns::renderLayout(QWidget *answerWidget)
    {
        return new QHBoxLayout(answerWidget);
    }

    void MultipleDropdowns::chooseAnswer(int index)
    {
        MultipleDropdownsQuestion *qq = question();
        QComboBox *blankAnswers = static_cast<QComboBox*>(QObject::sender());
        const ID answerId = blankAnswers->itemData(index).toUInt();
        const String blankId = blankAnswers->property("blankId").toString().toStdString();

        QuizQuestionAnswer const *previousAnswer = qq->chosenAnswer(blankId);

        // the "-" item has an ID of 0
        if (answerId == 0) {
            return;
        }

        // selection hasn't really changed?
        if (previousAnswer && answerId == previousAnswer->id()) {
            return;
        }

        qq->choose(blankId, answerId);

        emit answerModified(qq);
    }

    MultipleDropdowns::Fragments MultipleDropdowns::extractFragments(const QString &text)
    {
        Fragment trailing;
        Fragments fragments;
        QRegExp const &pattern = gBlankPattern;
        int cursor = 0, lastCursor = 0;

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

    MultipleDropdownsQuestion *MultipleDropdowns::question() const
    {
        return mQuestion->asMultipleDropdowns();
    }

}
