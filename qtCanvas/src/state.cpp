#include "include/state.h"
#include <algorithm>
#include <QSettings>

using Canvas::Course;

State* State::ginstance = nullptr;

State::State() : Logger("qtCanvas"), mStudent(nullptr) {
    QSettings settings;

    Canvas::Settings::set("canvas_host",
                          settings.value("canvas/host", "http://localhost")
                            .toString()
                            .toStdString());

    Canvas::Settings::set("canvas_port",
                          settings.value("canvas/port", "3000")
                            .toString()
                            .toStdString());

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

void State::setActiveQuiz(Canvas::Quiz *quiz)
{
    mActiveQuiz = quiz;
}

void State::setActiveQuizSubmission(Canvas::QuizSubmission *qs)
{
    mActiveQuizSubmission = qs;
}

Canvas::Quiz *State::activeQuiz()
{
    return mActiveQuiz;
}

Canvas::QuizSubmission *State::activeQuizSubmission()
{
    return mActiveQuizSubmission;
}
