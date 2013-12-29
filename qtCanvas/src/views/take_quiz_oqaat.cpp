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
    addNavigationButtons();
    mCursor = 0;

    connect(mQuestionIndex, SIGNAL(questionFocused(const QuizQuestion*,QuestionWidget*)),
            this, SLOT(focusQuestion(const QuizQuestion*,QuestionWidget*)));
}

void TakeQuizOQAAT::prevQuestion()
{
    QuizQuestion *question = locatePrevQuestion(&mCursor);

    if (question) {
        focusQuestion(question);

        mNextButton->setEnabled(true);
        mPrevButton->setEnabled(!!locatePrevQuestion());
    }
}

void TakeQuizOQAAT::nextQuestion()
{
    QuizQuestion *question = locateNextQuestion(&mCursor);

    if (question) {
        focusQuestion(question);

        if (mPrevButton) {
            mPrevButton->setEnabled(true);
        }

        mNextButton->setEnabled(!!locateNextQuestion());
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

void TakeQuizOQAAT::addNavigationButtons()
{
    QGridLayout *layout = static_cast<QGridLayout*>(ui->quizFrame->layout());
    QHBoxLayout *btnLayout = new QHBoxLayout;
    layout->addLayout(btnLayout, 1, 0);


    if (!(mPresentationFlags & QuizPresentation::CantGoBack)) {
        mPrevButton = new QPushButton(tr("Previous"), this);
        btnLayout->addWidget(mPrevButton, 0, Qt::AlignLeft);

        connect(mPrevButton, SIGNAL(released()), this, SLOT(prevQuestion()));
    }

    mNextButton = new QPushButton(tr("Next"), this);

    btnLayout->addStretch();
    btnLayout->addWidget(mNextButton, 0, Qt::AlignRight);

    connect(mNextButton, SIGNAL(released()), this, SLOT(nextQuestion()));
}

QVBoxLayout *TakeQuizOQAAT::questionLayout()
{
    return static_cast<QVBoxLayout*>(ui->quizQuestions->layout());
}

void TakeQuizOQAAT::focusQuestion(QuizQuestion const* question, bool broadcast)
{
    QuestionWidget *qqWidget = question->userData<QuestionWidget>("QWidget");
    QVBoxLayout *layout = questionLayout();

    assert(qqWidget);
    clearLayout(layout);

    layout->addWidget(qqWidget);
    layout->addStretch();

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

        if (!question->isAnswered()) {
            if (cursor) {
                *cursor = i;
            }

            return question;
        }
    }

    return nullptr;
}
