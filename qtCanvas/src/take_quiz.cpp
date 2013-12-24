#include "include/take_quiz.hpp"
#include "include/type_exports.hpp"
#include "include/viewport.h"
#include "ui_take_quiz.h"

using namespace Canvas::QuizQuestions;
using Canvas::Quiz;
using Canvas::QuizSubmission;

TakeQuiz::TakeQuiz(QWidget *parent) :
    QView(parent),
    Logger("TakeQuiz"),
    ui(new Ui::TakeQuiz)
{
    ui->setupUi(this);
    mRenderers.insert(std::make_pair("multiple_choice_question",
                                     &TakeQuiz::renderMultipleChoiceQuestion));

    QObject::connect(ui->submitButton, SIGNAL(released()),
                     this, SLOT(submitQuiz()));
}

TakeQuiz::~TakeQuiz()
{
    delete ui;
}

void TakeQuiz::setup()
{
    mQuiz = State::singleton().activeQuiz();
    mQuizSubmission = State::singleton().activeQuizSubmission();

    QVBoxLayout *scrollLayout = new QVBoxLayout;
    ui->scrollArea->setLayout(scrollLayout);
    ui->scrollArea->widget()->setLayout(scrollLayout);

    ui->quizTitleLabel->setText(QString::fromStdString(mQuiz->title()));

    debug() << "Rendering " << mQuiz->questions().size() << " questions";

    mQuiz->loadQuestions(State::singleton().getSession(), [&](bool success) {
        if (success) {
            renderQuestions();
        }
        else {
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
}

void TakeQuiz::chooseMultipleChoiceAnswer(QAbstractButton *answerButton)
{
    Canvas::Session &session = State::singleton().getSession();

    int answerId = answerButton->property("answerId").toInt();
    MultipleChoice *qq = static_cast<MultipleChoice*>(*answerButton->property("qq").value<PQuizQuestion>());

    debug() << "choosing answer#" << answerId << "for question#" << qq->id();

    qq->choose(answerId);

    mQuizSubmission->save(qq, session, [&](bool success) {
        if (success) {
            setStatus("Answer saved.");
        }
        else {
            setStatus("Error: answer could not saved.");
        }
    });
}

void TakeQuiz::renderQuestions()
{
    QLayout *questionLayout = ui->scrollArea->widget()->layout();

    for (auto question : mQuiz->questions()) {
        QuestionRenderer renderer = mRenderers[question->type()];

        if (!renderer) {
            warn() << "Can not render questions of type " << question->type();
            continue;
        }

        QWidget *qqWidget = new QWidget(this);
        QLayout *qqLayout = renderQuestion(question, qqWidget);

        QWidget *answerWidget = new QWidget(qqWidget);
        QGridLayout *answerLayout = new QGridLayout(answerWidget);
        answerLayout->setContentsMargins(0,5,5,5);
        qqLayout->addWidget(answerWidget);

        (this->*renderer)(question, answerWidget, answerLayout);

        question->setUserData<QWidget>("QWidget", qqWidget);
        questionLayout->addWidget(qqWidget);
        qqWidget->setProperty("qq", QVariant::fromValue(PQuizQuestion(question)));

//        ui->scrollAreaWidgetContents->layout()->addWidget(qqWidget);
    }
}

QLayout* TakeQuiz::renderQuestion(Canvas::QuizQuestion *qq, QWidget *widget)
{
    QGridLayout *layout = new QGridLayout(widget);

    // Question title widget:
    // [= %questionName "%questionPts pts" =]
    QFrame *titleWidget = new QFrame(widget);
    QHBoxLayout *titleLayout = new QHBoxLayout;
    QLabel *titleLabel =
            new QLabel(QString::fromStdString(qq->name()), titleWidget);
    QLabel *pointsLabel =
            new QLabel(QString("%1 pts").arg(qq->pointsPossible()), titleWidget);
    QToolButton *markButton =
            new QToolButton(titleWidget);

    markButton->setIcon(QIcon::fromTheme("emblem-important"));
    markButton->setCheckable(true);

    titleLayout->setContentsMargins(5, 5, 5, 5);

    titleLayout->addWidget(titleLabel, 0, Qt::AlignLeft);
    titleLayout->addWidget(markButton, 0, Qt::AlignRight);
    titleLayout->addWidget(pointsLabel, 0, Qt::AlignRight);

    titleWidget->setLayout(titleLayout);
    titleWidget->setFrameShape(QFrame::Box);
    titleWidget->setFrameShadow(QFrame::Sunken);

    // Question text:
    // [= %text =]
    QLabel *textLabel = new QLabel(QString::fromStdString(qq->text()), widget);
    textLabel->setContentsMargins(5,5,5,5);

    layout->addWidget(titleWidget);
    layout->addWidget(textLabel);

    QObject::connect(markButton, SIGNAL(toggled(bool)),
                     this, SLOT(markQuestion(bool)));

    return layout;
}

void TakeQuiz::renderMultipleChoiceQuestion(QuizQuestion *baseQq, QWidget *widget, QLayout *layout)
{
    MultipleChoice* qq = static_cast<Canvas::QuizQuestions::MultipleChoice*>(baseQq);
    QButtonGroup *answerButtons = new QButtonGroup(widget);

    debug() << "Rendering a MultipleChoice question: " << qq->id();

    for (auto answer : qq->answers()) {
        QRadioButton *answerRadio = new QRadioButton(QString::fromStdString(answer->text()), widget);
        answerRadio->setProperty("answerId", answer->id());
        answerRadio->setProperty("qq", QVariant::fromValue(PQuizQuestion(qq)));
        layout->addWidget(answerRadio);
        answerButtons->addButton(answerRadio);
    }

    QObject::connect(answerButtons, SIGNAL(buttonReleased(QAbstractButton*)),
                     this, SLOT(chooseMultipleChoiceAnswer(QAbstractButton*)));
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
