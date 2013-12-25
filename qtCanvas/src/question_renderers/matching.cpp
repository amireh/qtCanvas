#include "include/question_renderers/matching.hpp"
namespace QuestionRenderers {

    Matching::Matching(QuizQuestion *qq)
        : QuestionRenderer(qq), Canvas::Logger("MatchingRenderer")
    {
    }

    Matching::~Matching()
    {

    }

    void Matching::render(QWidget *qqWidget)
    {
        QFormLayout *layout = static_cast<QFormLayout*>(qqWidget->layout());
        MatchingQuestion *qq = question();

        for (auto answer : qq->answers()) {
            QLabel *left = new QLabel(QString::fromStdString(answer->left()), qqWidget);
            QComboBox *matchCombo = createMatchCombo();

            matchCombo->setProperty("answerId", answer->id());

            QObject::connect(matchCombo, SIGNAL(currentIndexChanged(int)),
                             this, SLOT(chooseMatch(int)));

            if (qq->hasMatching(answer)) {
                debug() << "setting current match: " << qq->matchedPair(answer).text;
                matchCombo->setCurrentText(QString::fromStdString(
                                               qq->matchedPair(answer).text));
            }

            matchCombo->setParent(qqWidget);
            layout->addRow(left, matchCombo);
        }
    }

    QLayout *Matching::renderLayout(QWidget *answerWidget)
    {
        return new QFormLayout(answerWidget);
    }

    void Matching::chooseMatch(int selectionIdx)
    {
        MatchingAnswer const* answer;
        MatchingQuestion *qq = question();
        QComboBox *matchCombo = static_cast<QComboBox*>(QObject::sender());

        Canvas::ID matchId = matchCombo->itemData(selectionIdx).toUInt();
        Canvas::ID answerId = matchCombo->property("answerId").toUInt();

        answer = qq->findAnswer(answerId);

        // skip saves if the selection hasn't changed
        try {
            if (qq->matchedPair(answer).id == matchId) {
                return;
            }
        } catch(...) {
        }

        if (!matchId) {
            qq->clearMatching(answer);
        } else {
            qq->matchPair(answer, matchId);
        }

        emit answerModified(qq);
    }

    MatchingQuestion *Matching::question() const
    {
        return mQuestion->asMatching();
    }

    QComboBox *Matching::createMatchCombo() const
    {
        MatchingQuestion *qq = question();
        QComboBox *widget = new QComboBox;

        widget->addItem("-", 0);

        for (auto match : qq->matches()) {
            widget->addItem(QString::fromStdString(match.text), match.id);
        }

        return widget;
    }
}
