#include "widgets/question_index.hpp"
#include "ui_question_index.h"
#include "type_exports.hpp"
#include "state.h"
#include "viewport.h"

QuestionIndex::QuestionIndex(QWidget *parent) :
    QWidget(parent),
    Canvas::Logger("QuestionIndex"),
    ui(new Ui::QuestionIndex),
    mScrollArea(nullptr),
    mInternalSelectionUpdate(false)
{
    ui->setupUi(this);
}

QuestionIndex::~QuestionIndex()
{
    delete ui;
}

void QuestionIndex::render(Quiz::Questions const &questions,
                           QScrollArea * scrollArea,
                           int presentationFlags)
{
    mScrollArea = scrollArea;

    connect(&State::singleton(), SIGNAL(questionModified(const QuizQuestion*)),
            this, SLOT(updateQuestionStatus(const QuizQuestion*)));

    connect(ui->markButton, SIGNAL(released()),
            this, SLOT(markQuestion()));

    if (presentationFlags & QuizPresentation::CantGoBack) {
        ui->listWidget->setEnabled(false);
    }

    for (auto qq : questions) {
        QListWidgetItem *qqItem = renderQuestionEntry(qq, ui->listWidget);

        if (qqItem) {
            ui->listWidget->addItem(qqItem);
            qq->setUserData<QListWidgetItem>("IndexItem", qqItem);
        }

        QuestionWidget *qqWidget = qq->userData<QuestionWidget>("QWidget");

        connect(qqWidget, SIGNAL(focused(QuestionWidget*)),
                this, SLOT(selectFocusedQuestion(QuestionWidget*)));
    }
}

QListWidgetItem *QuestionIndex::renderQuestionEntry(const Canvas::QuizQuestion *qq, QListWidget *view)
{
    QListWidgetItem *qqItem;

    qqItem = new QListWidgetItem(QString("Question %1").arg(qq->position()),
                                 view);
    qqItem->setData(0x0100, QVariant::fromValue(PQuizQuestion(qq)));

    updateQuestionStatus(qq, qqItem);

    return qqItem;
}

void QuestionIndex::updateQuestionStatus(const Canvas::QuizQuestion *qq, QListWidgetItem *qqItem)
{
    if (qq->isMarked()) {
        qqItem->setIcon(QIcon(":icons/emblem-important.png"));
    }
    else {
        if (qq->isAnswered()) {
            qqItem->setIcon(QIcon(":icons/emblem-success.png"));
        }
        else {
            qqItem->setIcon(QIcon(":icons/emblem-unreadable.png"));
        }
    }

}

void QuestionIndex::updateMarkButton(const Canvas::QuizQuestion *qq)
{
    if (qq->isMarked()) {
        ui->markButton->setText(tr("Unmark Question"));
    }
    else {
        ui->markButton->setText(tr("Mark Question"));
    }
}

Canvas::QuizQuestion *QuestionIndex::selectedQuestion()
{
    PQuizQuestion qqPtr = ui->listWidget->currentItem()->data(0x0100).value<PQuizQuestion>();

    return *qqPtr;
}

void QuestionIndex::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *)
{
    QuizQuestion *qq = *current->data(0x0100).value<PQuizQuestion>();

    updateMarkButton(qq);

    // this means the item selection was changed by us spying on the QuestionWidget,
    // so don't scroll it into view
    if (mInternalSelectionUpdate) {
        mInternalSelectionUpdate = false;
        return;
    }

    if (qq) {
        QuestionWidget *qqWidget = qq->userData<QuestionWidget>("QWidget");
        mScrollArea->ensureWidgetVisible(qqWidget);

        emit questionFocused(qq, qqWidget);
    }
}

void QuestionIndex::selectFocusedQuestion(QuestionWidget *qqWidget)
{
    QuizQuestion *qq = qqWidget->question();

    QList<QListWidgetItem*> items =
            ui->listWidget->findItems(QString("Question %1").arg(qq->position()),
                                      Qt::MatchExactly);

    if (!items.empty()) {
        mInternalSelectionUpdate = true;
        QListWidgetItem *qqItem = items.first();
        ui->listWidget->setCurrentItem(qqItem);
    }
}

void QuestionIndex::updateQuestionStatus(Canvas::QuizQuestion const *qq)
{
    QListWidgetItem *qqItem = qq->userData<QListWidgetItem>("IndexItem");

    if (qqItem) {
        updateQuestionStatus(qq, qqItem);
    }
}

void QuestionIndex::markQuestion()
{
    Canvas::Session &session = State::singleton().getSession();
    Canvas::QuizQuestion *qq = selectedQuestion();
    Canvas::QuizSubmission *qs = State::singleton().activeQuizSubmission();

    if (!qq) {
        error() << "unable to find quiz question";
        return;
    }

    if (qq->isMarked()) {
        Viewport::singleton().setStatus("Unmarking question...");
    }
    else {
        Viewport::singleton().setStatus("Marking question...");
    }

    qq->mark(!qq->isMarked());

    qs->save(qq, session, [&, qq](bool success) {
        if (success) {
            Viewport::singleton().setStatus(QString("Question %1")
                                            .arg(qq->isMarked() ? "marked" : "unmarked."));
            State::singleton().emit questionModified(qq);
        }
        else {
            Viewport::singleton().setStatus(
                        "Error: unable to change question status.");
        }
    });
}

