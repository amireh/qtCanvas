#include "include/available_quizzes.h"
#include "ui_available_quizzes.h"
#include "include/state.h"
#include "include/viewport.h"
#include <canvas/utility.hpp>
#include "include/type_exports.hpp"

using Canvas::Course;
using Canvas::Quiz;
using Canvas::QuizSubmission;

AvailableQuizzes::AvailableQuizzes(QWidget *parent) :
    QView(parent),
    Logger("AvailableQuizzes"),
    ui(new Ui::AvailableQuizzes)
{
    ui->setupUi(this);

    QObject::connect(this, SIGNAL(courseLoaded(Canvas::Course const&)),
                     this, SLOT(addCourse(Canvas::Course const&)));

    QObject::connect(this, SIGNAL(quizLoaded(Canvas::Quiz const&)),
                     this, SLOT(addQuiz(Canvas::Quiz const&)));

    QObject::connect(&State::singleton(), SIGNAL(loggedOut()),
                     this, SLOT(reset()));
    QObject::connect(ui->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,
                                                               QTreeWidgetItem*)),
                     this, SLOT(updateQuizActions(QTreeWidgetItem*,
                                                  QTreeWidgetItem*)));

    QObject::connect(this, SIGNAL(courseLoaded(Canvas::Course const&)),
                     this, SLOT(loadCourseQuizzes(Canvas::Course const&)));

    QObject::connect(this, SIGNAL(quizLoaded(Canvas::Quiz const&)),
                     this, SLOT(updateQuizStatus(Canvas::Quiz const&)));
    QObject::connect(this, SIGNAL(quizLoaded(Canvas::Quiz const&)),
                     this, SLOT(loadQuizSubmission(Canvas::Quiz const&)));

    QObject::connect(this, SIGNAL(quizSubmissionLoaded(Canvas::QuizSubmission const&)),
                     this, SLOT(updateQuizStatus(Canvas::QuizSubmission const&)));

    QObject::connect(ui->takeQuizButton, SIGNAL(released()),
                     this, SLOT(takeQuiz()));

}

AvailableQuizzes::~AvailableQuizzes()
{
    delete ui;
}

void AvailableQuizzes::setup()
{
    State &state = State::singleton();
    Canvas::Session &session = state.getSession();
    Canvas::Student &student = state.getStudent();

    setStatus("Loading courses...");

    student.loadCourses(session, [&](bool success) {
        if (success) {
            for (Canvas::Course *course : student.courses()) {
                emit courseLoaded(*course);
                debug() << "new course:" << course->id();
            }
        }
        else {
            Viewport::singleton().errorDialog()->showMessage(
                        tr("Unable to load courses."),
                        "api_error");
        }
    });
}

void AvailableQuizzes::cleanup()
{
}

QTreeWidgetItem* AvailableQuizzes::addCourse(Canvas::Course const &course, bool skipTest)
{
    QTreeWidgetItem *courseItem;

    if (!skipTest) {
        courseItem = courseTreeItem(course);

        if (courseItem) {
            warn() << "tree item for course#" << course.id()
                   << " already exists.";

            return courseItem;
        }
    }

    courseItem = new QTreeWidgetItem(ui->treeWidget);
    courseItem->setText(0, QString::fromStdString(course.name()));

    ui->treeWidget->addTopLevelItem(courseItem);

    debug() << "new tree item for course#" << course.id();

    return courseItem;
}

void AvailableQuizzes::addQuiz(Canvas::Quiz const &quiz)
{
    Canvas::Course const &course = *quiz.course();

    QTreeWidgetItem *quizItem;
    QTreeWidgetItem *courseItem = courseTreeItem(course);

    if (!courseItem) {
        warn() << "unable to find course tree item for course"
                << quiz.course()->id();

        courseItem = addCourse(course);
    }

    quizItem = new QTreeWidgetItem(courseItem);
    quizItem->setText(0, QString::fromStdString(quiz.title()));
    quizItem->setText(1, tr("-"));
    quizItem->setData(100, 0x0100, QVariant::fromValue(PQuiz(&quiz)));

    debug() << "new tree item for quiz#" << quiz.id() << " "
            << "to course#" << quiz.course()->id()
            << ", titled: " << quiz.title();
}

QTreeWidgetItem *AvailableQuizzes::courseTreeItem(const Canvas::Course &course)
{
    QList<QTreeWidgetItem*> candidateCourseItems = ui->treeWidget->findItems(
                QString::fromStdString(course.name()),
                Qt::MatchExactly,
                0);

    if (!candidateCourseItems.isEmpty()) {
        return candidateCourseItems.first();
    }

    return nullptr;
}

QTreeWidgetItem *AvailableQuizzes::quizTreeItem(const Quiz &quiz)
{
    QList<QTreeWidgetItem*> candidateItems = ui->treeWidget->findItems(
                QString::fromStdString(quiz.title()),
                Qt::MatchExactly | Qt::MatchRecursive,
                0);

    if (!candidateItems.isEmpty()) {
        return candidateItems.first();
    }

    return nullptr;
}

void AvailableQuizzes::updateQuizActions(QTreeWidgetItem *current, QTreeWidgetItem */*previous*/)
{
    PQuiz selectedQuiz;

    ui->takeQuizButton->setEnabled(false);

    if (!current) {
        return;
    }

    selectedQuiz = *current->data(100, 0x0100).value<PQuiz>();

    if (selectedQuiz) {
        Canvas::Quiz const& quiz = **selectedQuiz;
        Canvas::Student const& student = State::singleton().getStudent();

        debug() << "selection is now: quiz#" << quiz.id();

        if (student.canTakeQuiz(quiz)) {
            ui->takeQuizButton->setEnabled(true);
        }
    }
}

void AvailableQuizzes::reset() {
    ui->treeWidget->clear();
}

void AvailableQuizzes::loadCourseQuizzes(Canvas::Course const &course)
{
    debug() << "Loading quizzes for course" << course.id();
    Canvas::Session &session = State::singleton().getSession();

    ((Course&)course).loadQuizzes(session, [&](bool success) {
        if (success) {
            Course::Quizzes quizzes = course.quizzes();
            for (Canvas::Quiz const* quiz : quizzes) {
                debug() << "new quiz:" << quiz->id();
                emit quizLoaded(*quiz);
            }
        } else {
            error() << "loading quizzes failed";
        }
    });
}

void AvailableQuizzes::loadQuizSubmission(const Canvas::Quiz &quiz) {
    Canvas::Student& student = State::singleton().getStudent();
    Canvas::Session &session = State::singleton().getSession();

    student.loadQuizSubmission(session, quiz, [&](bool success) {
        if (success) {
            emit quizSubmissionLoaded(*student.quizSubmission(quiz));
        }
    });
}

void AvailableQuizzes::updateQuizStatus(const Canvas::Quiz &quiz)
{
    QTreeWidgetItem *treeItem = quizTreeItem(quiz);

    if (!treeItem) {
        error() << "Unable to find tree item for Quiz#" << quiz.id() << ". "
                << "Quiz status can not be updated.";

        return;
    }

    Canvas::Student &student = State::singleton().getStudent();
    QString status;

    if (student.canTakeQuiz(quiz)) {
        status = "Available";
    }
    else {
        status = "Not Available";
    }

    treeItem->setText(1, status);
}

void AvailableQuizzes::updateQuizStatus(const Canvas::QuizSubmission &qs)
{
    QString status;
    Canvas::Quiz const& quiz = *qs.quiz();
    QTreeWidgetItem *treeItem = quizTreeItem(quiz);

    if (!treeItem) {
        error() << "Unable to find tree item for Quiz#" << quiz.id() << ". "
                << "Quiz status can not be updated.";

        return;
    }

    if (qs.isTakeable()) {
        status = "In Progress";
    }
    else if (qs.isComplete()) {
        status = "Complete";
    }
    else if (qs.isPendingReview()) {
        status = "Under Review";
    }

    debug() << "Updating quiz status from QS status: " << status.toStdString();

    treeItem->setText(1, status);
    treeItem->setText(2,
                      QString("%1 (out of %2)")
                      .arg(QString::number(qs.keptScore(), 'g', 2))
                      .arg(quiz.pointsPossible()));
    treeItem->setText(3, QString("%1").arg(qs.attempt()));

}

void AvailableQuizzes::takeQuiz()
{
    PQuiz selectedQuiz = ui->treeWidget->currentItem()->data(100, 0x0100).value<PQuiz>();
    Canvas::Student &student = State::singleton().getStudent();
    Canvas::Session &session = State::singleton().getSession();

    if (selectedQuiz) {
        setStatus("Requesting a quiz-taking session...");

        student.takeQuiz(session, **selectedQuiz, [&](QuizSubmission const* qs) {
            if (qs) {
                State::singleton().setActiveQuiz(*selectedQuiz);
                State::singleton().setActiveQuizSubmission((QuizSubmission*)qs);

                Viewport::singleton().transition("TakeQuiz");
            }
            else {
                setStatus("Error: the Canvas server has rejected your request.");
            }
        });
    }
}

