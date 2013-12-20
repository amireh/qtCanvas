#include "include/available_quizzes.h"
#include "ui_available_quizzes.h"
#include "include/state.h"
#include <canvas/resources/quiz_submission.hpp>
#include <canvas/utility.hpp>

AvailableQuizzes::AvailableQuizzes(QWidget *parent) :
    QWidget(parent),
    Logger("AvailableQuizzes"),
    ui(new Ui::AvailableQuizzes)
{
    ui->setupUi(this);

    QObject::connect(&State::singleton(), SIGNAL(courseAdded(Canvas::Course const&)),
                     this, SLOT(addCourse(Canvas::Course const&)));

    QObject::connect(&State::singleton(), SIGNAL(courseQuizAdded(Canvas::Quiz const&)),
                     this, SLOT(addCourseQuiz(Canvas::Quiz const&)));

    QObject::connect(&State::singleton(), SIGNAL(loggedOut()),
                     this, SLOT(reset()));
}

AvailableQuizzes::~AvailableQuizzes()
{
    delete ui;
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

void AvailableQuizzes::addCourseQuiz(Canvas::Quiz const &quiz)
{
    Canvas::Course const &course = *quiz.course();
    Canvas::Student &student = State::singleton().getStudent();
    Canvas::Session &session = State::singleton().getSession();

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

    debug() << "new tree item for quiz#" << quiz.id() << " "
            << "to course#" << quiz.course()->id()
            << ", titled: " << quiz.title();

    student.loadQuizSubmission(session, quiz, [&](bool success) {
        if (success) {
            Canvas::QuizSubmission const *qs = student.quizSubmission(quiz);
            quizItem->setText(2, QString("%1").arg(qs->keptScore()));
        }
    });
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


void AvailableQuizzes::reset() {
    ui->treeWidget->clear();
}
