#include "views/available_quizzes.hpp"
#include "ui_available_quizzes.h"
#include "include/state.h"
#include "include/viewport.h"
#include <canvas/utility.hpp>
#include "include/type_exports.hpp"

using Canvas::Course;
using Canvas::Quiz;
using Canvas::QuizSubmission;
using Canvas::Student;

AvailableQuizzes::AvailableQuizzes(QWidget *parent) :
    QView(parent),
    Logger("AvailableQuizzes"),
    ui(new Ui::AvailableQuizzes)
{
    ui->setupUi(this);

    connect(this, SIGNAL(coursesLoaded()), this, SLOT(addCourses()));

    connect(this, SIGNAL(courseLoaded(Canvas::Course *)),
            this, SLOT(addCourse(Canvas::Course *)));

    connect(this, SIGNAL(courseLoaded(Canvas::Course *)),
            this, SLOT(loadCourseQuizzes(Canvas::Course *)));

    connect(this, SIGNAL(quizLoaded(Canvas::Quiz *)),
            this, SLOT(addQuiz(Canvas::Quiz *)));

    connect(this, SIGNAL(quizLoaded(Canvas::Quiz *)),
            this, SLOT(updateQuizStatus(Canvas::Quiz *)));

    connect(this, SIGNAL(quizLoaded(Canvas::Quiz *)),
            this, SLOT(loadQuizSubmission(Canvas::Quiz *)));

    connect(this, SIGNAL(quizSubmissionLoaded(Canvas::QuizSubmission *)),
            this, SLOT(updateQuizStatus(Canvas::QuizSubmission *)));

    connect(ui->treeWidget,
            SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
            this,
            SLOT(updateQuizActions(QTreeWidgetItem*, QTreeWidgetItem*)));
    connect(ui->takeQuizButton, SIGNAL(released()),
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
            emit coursesLoaded();
        }
        else {
            Viewport::singleton().errorDialog()->showMessage(
                        tr("Unable to load courses."),
                        "api_error");
        }
    });

    ui->treeWidget->setFocus();
}

void AvailableQuizzes::cleanup()
{
}

void AvailableQuizzes::addCourses()
{
    Student &student = State::singleton().getStudent();

    for (Course *course : student.courses()) {
        debug() << "Course:" << course->id();
        emit courseLoaded(course);
    }
}

QTreeWidgetItem* AvailableQuizzes::addCourse(Canvas::Course *course, bool skipTest)
{
    QTreeWidgetItem *courseItem;

    if (!skipTest) {
        courseItem = courseTreeItem(course);

        if (courseItem) {
            warn() << "tree item for course#" << course->id()
                   << " already exists.";

            return courseItem;
        }
    }

    courseItem = new QTreeWidgetItem(ui->treeWidget);
    courseItem->setText(0, QString::fromStdString(course->name()));

    ui->treeWidget->addTopLevelItem(courseItem);

    if (ui->treeWidget->selectedItems().empty()) {
        ui->treeWidget->setCurrentItem(courseItem);
    }

    debug() << "new tree item for course#" << course->id();

    return courseItem;
}

void AvailableQuizzes::addQuiz(Canvas::Quiz *quiz)
{
    Canvas::Course *course = quiz->course();

    QTreeWidgetItem *quizItem;
    QTreeWidgetItem *courseItem = courseTreeItem(course);

    if (!courseItem) {
        warn() << "unable to find course tree item for course"
                << course->id();

        courseItem = addCourse(course);
    }

    quizItem = new QTreeWidgetItem(courseItem);
    quizItem->setText(0, QString::fromStdString(quiz->title()));
    quizItem->setText(1, tr("-"));
    quizItem->setData(100, 0x0100, QVariant::fromValue(PQuiz(quiz)));

    if (!courseItem->isExpanded()) {
        courseItem->setExpanded(true);
    }

    debug() << "new tree item for quiz#" << quiz->id() << " "
            << "to course#" << quiz->course()->id()
            << ", titled: " << quiz->title();
}

QTreeWidgetItem *AvailableQuizzes::courseTreeItem(Canvas::Course const *course)
{
    QList<QTreeWidgetItem*> candidateCourseItems = ui->treeWidget->findItems(
                QString::fromStdString(course->name()),
                Qt::MatchExactly,
                0);

    if (!candidateCourseItems.isEmpty()) {
        return candidateCourseItems.first();
    }

    return nullptr;
}

QTreeWidgetItem *AvailableQuizzes::quizTreeItem(const Quiz *quiz)
{
    QList<QTreeWidgetItem*> candidateItems = ui->treeWidget->findItems(
                QString::fromStdString(quiz->title()),
                Qt::MatchExactly | Qt::MatchRecursive,
                0);

    if (!candidateItems.isEmpty()) {
        return candidateItems.first();
    }

    return nullptr;
}

QString AvailableQuizzes::humanReadableTime(qint64 seconds) const
{
    if (seconds < 60) {
        return "Less than a minute.";
    }
    else if (seconds < 3600) {
        return QString("%1 minutes").arg(seconds / 60);
    }
    else if (seconds < 86400) {
        return QString("%1 hours").arg(seconds / 3600);
    }
    else {
        return QString("Over a day.");
    }
}

qint64 AvailableQuizzes::timestampToSeconds(const Canvas::String &date) const
{
    QDateTime from =
            QDateTime::fromString(QString::fromStdString(date),
                                  "yyyy-MM-dd'T'HH:mm:ss'Z'");
    return from.secsTo(QDateTime::currentDateTime());
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
        Canvas::Quiz *quiz = *selectedQuiz;
        Canvas::Student const& student = State::singleton().getStudent();

        debug() << "selection is now: quiz#" << quiz->id();

        if (student.canTakeQuiz(*quiz)) {
            ui->takeQuizButton->setEnabled(true);
        }
    }
}

void AvailableQuizzes::reset() {
//    ui->treeWidget->clear();
}

void AvailableQuizzes::loadCourseQuizzes(Canvas::Course *course)
{
    debug() << "Loading quizzes for course" << course->id();

    course->loadQuizzes(State::singleton().getSession(), [&, course](bool success) {
        if (success) {
            Course::Quizzes quizzes = course->quizzes();

            for (Canvas::Quiz* quiz : quizzes) {
                debug() << "new quiz:" << quiz->id();
                emit quizLoaded(quiz);
            }
        } else {
            error() << "loading quizzes failed";
            reportError(QString("Unable to load quizzes for the course %1.")
                        .arg(QString::fromStdString(course->name())));
        }
    });
}

void AvailableQuizzes::loadQuizSubmission(Canvas::Quiz *quiz) {
    Canvas::Student &student = State::singleton().getStudent();
    Canvas::Session &session = State::singleton().getSession();

    student.loadQuizSubmission(session, *quiz, [&, quiz](bool success) {
        if (success) {
            Canvas::Student &student = State::singleton().getStudent();

            debug() << "quiz: " << quiz->id();
            debug() << "student: " << student.id();

            emit quizSubmissionLoaded(student.quizSubmission(*quiz));
        }
    });
}

void AvailableQuizzes::updateQuizStatus(Canvas::Quiz *quiz)
{
    QTreeWidgetItem *treeItem = quizTreeItem(quiz);

    if (!treeItem) {
        error() << "Unable to find tree item for Quiz#" << quiz->id() << ". "
                << "Quiz status can not be updated.";

        return;
    }

    Canvas::Student &student = State::singleton().getStudent();
    QString status;

    if (student.canTakeQuiz(*quiz)) {
        status = "Available";
    }
    else {
        status = "Not Available";
    }

    treeItem->setText(1, status);
}

void AvailableQuizzes::updateQuizStatus(Canvas::QuizSubmission *qs)
{
    QString status;
    Canvas::Quiz const* quiz = qs->quiz();
    QTreeWidgetItem *treeItem = quizTreeItem(quiz);

    if (!treeItem) {
        error() << "Unable to find tree item for Quiz#" << quiz->id() << ". "
                << "Quiz status can not be updated.";

        return;
    }

    if (qs->isTakeable()) {
        status = QString("In Progress (for %1)")
                .arg(humanReadableTime(timestampToSeconds(qs->startedAt())));

    }
    else if (qs->isComplete()) {
        status = "Complete";
    }
    else if (qs->isPendingReview()) {
        status = "Under Review";
    }

    debug() << "Updating quiz status from QS status: " << status.toStdString();

    treeItem->setText(1, status);
    treeItem->setText(2,
                      QString("%1 (out of %2)")
                      .arg(QString::number(qs->keptScore(), 'g', 2))
                      .arg(quiz->pointsPossible()));
    treeItem->setText(3, QString("%1").arg(qs->attempt()));

}

void AvailableQuizzes::takeQuiz()
{
    PQuiz selectedQuiz = ui->treeWidget->currentItem()->data(100, 0x0100).value<PQuiz>();
    Canvas::Quiz *quiz = *selectedQuiz;

    if (selectedQuiz) {
        Canvas::Student &student = State::singleton().getStudent();
        Canvas::Session &session = State::singleton().getSession();

        setStatus("Requesting a quiz-taking session...");

        student.takeQuiz(session, *quiz, [&, quiz](QuizSubmission const* qs) {
            if (qs) {
                State::singleton().setActiveQuiz(quiz);
                State::singleton().setActiveQuizSubmission((QuizSubmission*)qs);

                if (quiz->isOQAAT()) {
                    Viewport::singleton().transition("TakeQuizOQAAT");
                }
                else {
                    Viewport::singleton().transition("TakeQuiz");
                }
            }
            else {
                setStatus("Error: the Canvas server has rejected your request.");
            }
        });
    }
}
