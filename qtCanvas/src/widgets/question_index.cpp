#include "widgets/question_index.hpp"
#include "ui_question_index.h"
#include "type_exports.hpp"
#include "state.h"

QuestionIndex::QuestionIndex(QWidget *parent) :
    QWidget(parent),
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

void QuestionIndex::render(Quiz::Questions const &questions, QScrollArea * scrollArea)
{
    connect(&State::singleton(), SIGNAL(questionModified(const QuizQuestion*)),
            this, SLOT(updateQuestionStatus(const QuizQuestion*)));
    mScrollArea = scrollArea;

    for (auto qq : questions) {
        QListWidgetItem *qqItem = renderQuestionEntry(qq, ui->listWidget);

        if (qqItem) {
            ui->listWidget->addItem(qqItem);
            qq->setUserData<QListWidgetItem>("IndexItem", qqItem);
        }

        QuestionWidget *qqWidget = qq->userData<QuestionWidget>("QWidget");
        QObject::connect(qqWidget, SIGNAL(focused(QuestionWidget*)),
                         this, SLOT(updateSelection(QuestionWidget*)));
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
        qqItem->setIcon(QIcon::fromTheme("emblem-important",
                                         QIcon(":icons/emblem-important.png")));
    }
    else {
        if (qq->isAnswered()) {
            qqItem->setIcon(QIcon::fromTheme("emblem-default",
                                             QIcon(":icons/emblem-success.png")));
        }
        else {
            qqItem->setIcon(QIcon::fromTheme("emblem-unreadable",
                                             QIcon(":icons/emblem-unreadable.png")));
        }
    }

}

void QuestionIndex::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    // this means the item selection was changed by us spying on the QuestionWidget,
    // so don't scroll it into view
    if (mInternalSelectionUpdate) {
        mInternalSelectionUpdate = false;
        return;
    }

    PQuizQuestion qqPtr = current->data(0x0100).value<PQuizQuestion>();
    QuizQuestion *qq = *qqPtr;

    if (qq) {
        assert(mScrollArea);

        QuestionWidget *qqWidget = qq->userData<QuestionWidget>("QWidget");
        mScrollArea->ensureWidgetVisible(qqWidget);
    }
}

void QuestionIndex::updateSelection(QuestionWidget *qqWidget)
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

void QuestionIndex::updateQuestionStatus(const Canvas::QuizQuestion *qq)
{
    QListWidgetItem *qqItem = qq->userData<QListWidgetItem>("IndexItem");

    if (qqItem) {
        updateQuestionStatus(qq, qqItem);
    }
}

