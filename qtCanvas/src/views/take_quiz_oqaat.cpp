#include "include/views/take_quiz_oqaat.hpp"
#include "take_quiz.hpp"
#include "ui_take_quiz.h"
#include <QGridLayout>

TakeQuizOQAAT::TakeQuizOQAAT(QWidget *parent)
    : TakeQuiz(parent),
      mCursor(0),
      mSinkWidget(new QWidget(0)),
      mSinkLayout(new QHBoxLayout(mSinkWidget)),
      mNextButton(nullptr),
      mPrevButton(nullptr)
{
}

TakeQuizOQAAT::~TakeQuizOQAAT()
{
    delete mSinkLayout;
    mSinkWidget = nullptr;
    mSinkLayout = nullptr;
}

void TakeQuizOQAAT::setup()
{
    TakeQuiz::setup();

    mCursor = -1;

    connect(mQuestionIndex, SIGNAL(questionFocused(const QuizQuestion*,QuestionWidget*)),
            this, SLOT(focusQuestion(const QuizQuestion*,QuestionWidget*)));
}

void TakeQuizOQAAT::prevQuestion()
{
    QuizQuestion *question = locatePrevQuestion(&mCursor);

    if (question) {
        focusQuestion(question);
    }
}

void TakeQuizOQAAT::nextQuestion()
{
    QuizQuestion *question = locateNextQuestion(&mCursor);

    if (question) {
        focusQuestion(question);
    }
}

void TakeQuizOQAAT::focusQuestion(const QuizQuestion *question, QuestionWidget *)
{
    focusQuestion(question, false);
}

void TakeQuizOQAAT::clearLayout(QVBoxLayout *layout)
{
    QLayoutItem *currentItem;

    if (!layout) {
        layout = questionLayout();
    }

    while ((currentItem = layout->itemAt(0))) {
        if (currentItem->spacerItem()) {
            layout->removeItem(currentItem);
            delete currentItem;
        } else {
            mSinkLayout->addWidget(currentItem->widget());
        }
    }
}

void TakeQuizOQAAT::renderQuestions()
{
    for (auto question : mQuiz->questions()) {
        renderQuestion(question);
    }

    QTimer::singleShot(0, this, SLOT(nextQuestion()));
}

void TakeQuizOQAAT::renderActions(QHBoxLayout *btnLayout)
{
    if (!mQuiz->cantGoBack()) {
        mPrevButton = new QPushButton(tr("Previous"), this);
        mPrevButton->setEnabled(false);

        btnLayout->addWidget(mPrevButton, 0, Qt::AlignLeft);

        connect(mPrevButton, SIGNAL(released()), this, SLOT(prevQuestion()));
    }

    mNextButton = new QPushButton(tr("Next"), this);
    mNextButton->setDefault(true);
    mNextButton->setEnabled(false);

    ui->submitButton->setVisible(false);

    btnLayout->addStretch();
    btnLayout->addWidget(mNextButton, 0, Qt::AlignRight);
    btnLayout->addWidget(ui->submitButton);

    connect(mNextButton, SIGNAL(released()), this, SLOT(nextQuestion()));
}

void TakeQuizOQAAT::updateActionState()
{
    if (mPrevButton) {
        mPrevButton->setEnabled(!!locatePrevQuestion());
    }

    auto highlight = [](QPushButton *btn, bool f = true) {
        btn->setEnabled(f);
        btn->setVisible(f);
        btn->setDefault(f);
    };

    // there are still more questions
    if (!!locateNextQuestion()) {
        highlight(mNextButton, true);
        highlight(ui->submitButton, false);
    }
    // last question
    else {
        highlight(ui->submitButton);
        highlight(mNextButton, false);
    }
}

QVBoxLayout *TakeQuizOQAAT::questionLayout()
{
    return static_cast<QVBoxLayout*>(ui->quizQuestions->layout());
}

void TakeQuizOQAAT::focusQuestion(QuizQuestion const* question, bool broadcast)
{
    int cursor;

    QuestionWidget *qqWidget = question->userData<QuestionWidget>("QWidget");
    QVBoxLayout *layout = questionLayout();

    assert(qqWidget);
    clearLayout(layout);

    layout->addWidget(qqWidget);
    layout->addStretch();

    // make sure the cursor is pointing to the correct question
    cursor = 0;
    for (auto q : mQuiz->questions()) {
        if (q == question) {
            break;
        }

        ++cursor;
    }

    if (mCursor != cursor) {
        warn() << "Cursor is out of sync:" << mCursor << " vs " << cursor;
        warn() << "Forcing update.";

        mCursor = cursor;
    }

    debug() << "Focusing QuizQuestion@" << mCursor;

    updateActionState();

    if (broadcast) {
        qqWidget->emit focused(qqWidget);
    }
}

QuizQuestion *TakeQuizOQAAT::locatePrevQuestion(int *cursor) const
{
    return locateQuestion(false, mCursor, cursor);
}

QuizQuestion *TakeQuizOQAAT::locateNextQuestion(int *cursor) const
{
    return locateQuestion(true, mCursor, cursor);
}

QuizQuestion *TakeQuizOQAAT::locateQuestion(bool forward, int position, int *cursor) const
{
    using Canvas::Quiz;

    int i;
    const Quiz::Questions& questions = mQuiz->questions();
    const int anchor = position + (forward ? 1 : -1);
    const int lBound = 0, uBound = questions.size();

    for (i = anchor; (forward ? i < uBound : i >= lBound); (forward ? ++i : --i)) {
        QuizQuestion *question = questions.at(i);

        if (isFocusable(question)) {
            if (cursor) {
                *cursor = i;
            }

            return question;
        }
    }

    return nullptr;
}

bool TakeQuizOQAAT::isFocusable(const QuizQuestion *question) const
{
    return !question->isAnswered();
}
