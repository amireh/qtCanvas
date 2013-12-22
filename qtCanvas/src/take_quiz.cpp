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
    mQuizSubmission = (QuizSubmission*)State::singleton().getStudent().quizSubmission(*mQuiz);

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
    int answerId = answerButton->property("answerId").toInt();
    MultipleChoice *qq = static_cast<MultipleChoice*>(*answerButton->property("qq").value<PQuizQuestion>());

    debug() << "choosing answer#" << answerId
            << "for question#" << qq->id();

    Canvas::JSONValue jsonAnswer = qq->answer(answerId);
    Canvas::Session &session = State::singleton().getSession();

    mQuizSubmission->saveAnswer(qq, jsonAnswer, session, [&](bool success) {
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
    for (auto question : mQuiz->questions()) {
        QuestionRenderer renderer = mRenderers[question->type()];

        if (!renderer) {
            warn() << "Can not render questions of type " << question->type();
            continue;
        }

        QWidget *qqWidget = new QWidget(this);
        assert(ui->scrollArea->widget()->layout());
        ui->scrollArea->widget()->layout()->addWidget(qqWidget);
        (this->*renderer)(question, qqWidget);
        question->setUserData<QWidget>("QWidget", qqWidget);
//        ui->scrollAreaWidgetContents->layout()->addWidget(qqWidget);
    }
}

void TakeQuiz::renderMultipleChoiceQuestion(QuizQuestion *baseQq, QWidget *widget)
{
    MultipleChoice* qq = static_cast<Canvas::QuizQuestions::MultipleChoice*>(baseQq);
    debug() << "Rendering a MultipleChoice question: " << qq->id();

    QVBoxLayout *masterLayout = new QVBoxLayout(widget);
    QGroupBox *box = new QGroupBox(widget);
    QGridLayout *layout = new QGridLayout;
    QLabel *qqText = new QLabel(QString::fromStdString(qq->text()), box);
//    QRadioButton *radioButton = new QRadioButton(, box);

    box->setTitle(QString::fromStdString(qq->name()));
    masterLayout->addWidget(box);
    layout->addWidget(qqText);

    QButtonGroup *answerButtons = new QButtonGroup(box);

    for (auto answer : qq->answers()) {
        QRadioButton *answerRadio = new QRadioButton(QString::fromStdString(answer->text()), box);
        answerRadio->setProperty("answerId", answer->id());
        answerRadio->setProperty("qq", QVariant::fromValue(PQuizQuestion(qq)));
        layout->addWidget(answerRadio);
        answerButtons->addButton(answerRadio);
    }

    QObject::connect(answerButtons, SIGNAL(buttonReleased(QAbstractButton*)),
                     this, SLOT(chooseMultipleChoiceAnswer(QAbstractButton*)));

    box->setLayout(layout);
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
