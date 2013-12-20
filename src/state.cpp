#include "include/state.h"
#include <algorithm>

using Canvas::Course;

State* State::ginstance = nullptr;

State::State() : Logger("qtCanvas"), mStudent(nullptr) {
    Canvas::Settings::set("Canvas::_host", "HTTP://localhost");
    Canvas::Settings::set("Canvas::_port", "3000");

    QObject::connect(this, SIGNAL(courseAdded(Canvas::Course const&)),
                     this, SLOT(loadCourseQuizzes(Canvas::Course const&)));

    reset();
}

State::~State() {
    info() << "Cleaning up.";

    reset(true);

    info() << "Clean.";
}

State& State::singleton() {
    if (!ginstance) {
        ginstance = new State();
    }

    return *ginstance;
}

Canvas::Student &State::getStudent()
{
    return *mStudent;
}

Canvas::Session &State::getSession()
{
    return mSession;
}

void State::reset(bool shuttingDown)
{
    if (mStudent) {
        delete mStudent;
        mStudent = nullptr;
    }

    if (!shuttingDown) {
        mStudent = new Canvas::Student();

    }
}

void State::loadCourseQuizzes(Canvas::Course const &course)
{
    debug() << "Loading quizzes for course" << course.id();

    ((Canvas::Course&)course).loadQuizzes(mSession, [&](bool success) {
        if (success) {
            Course::Quizzes quizzes = course.quizzes();
            for (Canvas::Quiz const* quiz : quizzes) {
                debug() << "new quiz:" << quiz->id();
                emit courseQuizAdded(*quiz);
            }
        } else {
            error() << "loading quizzes failed";
        }
    });
}
