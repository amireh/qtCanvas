#include "include/take_quiz.hpp"
#include "include/type_exports.hpp"
#include "include/viewport.h"
#include "ui_take_quiz.h"
#include "question_renderers/multiple_choice.hpp"
#include "question_renderers/true_false.hpp"
#include "question_renderers/fill_in_the_blank.hpp"
#include "question_renderers/fill_in_multiple_blanks.hpp"
#include "question_renderers/essay.hpp"
#include "question_renderers/calculated.hpp"
#include "question_renderers/numerical.hpp"
#include "question_renderers/matching.hpp"
#include "question_renderers/multiple_dropdowns.hpp"
#include "question_renderers/multiple_answers.hpp"

//using namespace Canvas::QuizQuestions;
using Canvas::Quiz;
using Canvas::QuizSubmission;

TakeQuiz::TakeQuiz(QWidget *parent) :
    QView(parent),
    Logger("TakeQuiz"),
    ui(new Ui::TakeQuiz)
{
    ui->setupUi(this);

    QObject::connect(ui->submitButton, SIGNAL(released()),
                     this, SLOT(submitQuiz()));
}

TakeQuiz::~TakeQuiz()
{
    std::for_each(mRenderers.begin(), mRenderers.end(), [](QuestionRenderer* r) {
        delete r;
    });

    mRenderers.clear();

    delete ui;
}

void TakeQuiz::setup()
{
    Canvas::Session &session = State::singleton().getSession();

    mQuiz = State::singleton().activeQuiz();
    mQuizSubmission = State::singleton().activeQuizSubmission();
    mQuestionIndex = new QuestionIndex(ui->indexFrame);

    ui->quizTitleLabel->setText(QString::fromStdString(mQuiz->title()));
    ui->indexFrame->layout()->addWidget(mQuestionIndex);
    debug() << "Rendering " << mQuiz->questions().size() << " questions";

    mQuiz->loadQuestions(session, [&](bool success) {
        if (success) {
            mQuizSubmission->loadAnswers(session, [&](bool success) {
                if (success) {
                    renderQuestions();
                    setStatus("Ready.");
                }
                else {
                    setStatus("Error: unable to load answers.");
                }
            });
            mQuestionIndex->render(mQuiz->questions());
        }
        else {
            setStatus("Error: unable to quiz questions.");
            error() << "Unable to load quiz questions.";
        }
    });
}

void TakeQuiz::cleanup()
{
    for (auto question : mQuiz->questions()) {
        QWidget *qqWidget = question->userData<QWidget>("QWidget");

        if (qqWidget) {
            delete qqWidget;
        }

        question->clearUserData("QWidget");
    }

    mQuiz = nullptr;
    mQuizSubmission = nullptr;
}

QuestionRenderer *TakeQuiz::generateRenderer(QuizQuestion *qq)
{
    using namespace QuestionRenderers;

    const String &qqType = qq->type();

    if (qqType == "multiple_choice_question") {
        return new MultipleChoice(qq);
    }
    else if (qqType == "true_false_question") {
        return new TrueFalse(qq);
    }
    else if (qqType == "short_answer_question") {
        return new FillInTheBlank(qq);
    }
    else if (qqType == "fill_in_multiple_blanks_question") {
        return new FillInMultipleBlanks(qq);
    }
    else if (qqType == "essay_question") {
        return new Essay(qq);
    }
    else if (qqType == "calculated_question") {
        return new Calculated(qq);
    }
    else if (qqType == "numerical_question") {
        return new Numerical(qq);
    }
    else if (qqType == "matching_question") {
        return new Matching(qq);
    }
    else if (qqType == "multiple_dropdowns_question") {
        return new MultipleDropdowns(qq);
    }
    else if (qqType == "multiple_answers_question") {
        return new MultipleAnswers(qq);
    }

    return nullptr;
}


void TakeQuiz::renderQuestions()
{
    QLayout *questionLayout = ui->scrollArea->widget()->layout();

    for (auto question : mQuiz->questions()) {
        QWidget *qqWidget = renderQuestion(question);

        if (qqWidget) {
            questionLayout->addWidget(qqWidget);
        }
    }
}

QWidget* TakeQuiz::renderQuestion(QuizQuestion* question) {
    QWidget *qqWidget;
    QFrame *qqTitleFrame;
    QLayout *qqLayout;
    QWidget *answerWidget;
    QuestionRenderer *renderer;

    renderer = generateRenderer(question);

    if (!renderer) {
        warn() << "Can not render questions of type " << question->type();
        return nullptr;
    }

    qqWidget = new QWidget(this);
    qqLayout = new QGridLayout(qqWidget);

    qqTitleFrame = renderQuestionFrame(question, qqWidget);
    answerWidget = renderer->renderFrame(qqWidget);

    qqLayout->addWidget(qqTitleFrame);

    if (renderer->hasRenderableText()) {
        qqLayout->addWidget(renderer->renderText(qqWidget));
    }

    qqLayout->addWidget(answerWidget);

    renderer->render(answerWidget);

    QObject::connect(renderer, SIGNAL(answerModified(const QuizQuestion*)),
                     this, SLOT(saveAnswer(const QuizQuestion*)));

    qqWidget->setObjectName("qq");
    qqWidget->setProperty("qq", QVariant::fromValue(PQuizQuestion(question)));
    question->setUserData<QWidget>("QWidget", qqWidget);

    return qqWidget;
}

QFrame* TakeQuiz::renderQuestionFrame(Canvas::QuizQuestion *qq, QWidget *widget)
{
    QString qqTitle;
    QFrame *titleWidget;
    QHBoxLayout *titleLayout;
    QLabel *titleLabel;
    QLabel *pointsLabel;

//    qqTitle = QString::fromStdString(qq->name();
    qqTitle = QString("Question %1").arg(qq->position());

    // Question title widget:
    // [= %questionName "%questionPts pts" =]
    titleWidget = new QFrame(widget);
    titleLayout = new QHBoxLayout(titleWidget);
    titleLabel = new QLabel(qqTitle, titleWidget);
    pointsLabel =  new QLabel(QString("%1 pts").arg(qq->pointsPossible()),
                              titleWidget);

    titleLayout->setContentsMargins(0,0,0,0);
    titleLayout->addWidget(titleLabel, 0, Qt::AlignLeft);
    titleLayout->addWidget(pointsLabel, 0, Qt::AlignRight);

    titleWidget->setObjectName("qqTitle");


    return titleWidget;
}

QWidget *TakeQuiz::renderMarkButton(QuizQuestion const* qq, QWidget *widget)
{
    QToolButton *markButton = new QToolButton(widget);

    markButton->setIcon(QIcon::fromTheme("emblem-important"));
    markButton->setCheckable(true);
    markButton->setChecked(qq->isMarked());

    QObject::connect(markButton, SIGNAL(toggled(bool)),
                     this, SLOT(markQuestion(bool)));

    return markButton;
}

void TakeQuiz::submitQuiz()
{
    QMessageBox confirmation;
    int confirmationRc;

    confirmation.setText("This action may be irreversible.");
    confirmation.setInformativeText("Do you want to submit your answers?");
    confirmation.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    confirmation.setDefaultButton(QMessageBox::Save);
    confirmationRc = confirmation.exec();

    if (confirmationRc != QMessageBox::Save) {
        return;
    }

    setStatus("Submitting your answers... please wait.");

    mQuizSubmission->complete(State::singleton().getSession(),
                              [&](bool success) {
        if (success) {
            setStatus("Your quiz attempt has been completed.");
            Viewport::singleton().transition("AvailableQuizzes");
        }
        else {
            setStatus("Error: unable to submit quiz, please try again.");
        }
    });
}

void TakeQuiz::markQuestion(bool isMarked)
{
    QObject *qqWidget = QObject::sender()->parent()->parent();
    QuizQuestion *qq = *qqWidget->property("qq").value<PQuizQuestion>();

    if (!qq) {
        error() << "unable to find quiz question";
        return;
    }

    qq->mark(isMarked);
    mQuizSubmission->save(qq, State::singleton().getSession());
}

void TakeQuiz::saveAnswer(QuizQuestion const* qq)
{
    Canvas::Session &session = State::singleton().getSession();

    mQuizSubmission->save(qq, session, [&](bool success) {
        if (success) {
            setStatus("Answer saved.");
        }
        else {
            setStatus("Error: answer could not saved.");
        }
    });
}
