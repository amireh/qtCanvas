#ifndef STATE_H
#define STATE_H

#include <QString>
#include <QObject>
#include <canvas/canvas.hpp>
#include <canvas/session.hpp>
#include <canvas/settings.hpp>
#include <canvas/resource_parser.hpp>
#include <canvas/resources/student.hpp>
#include <canvas/resources/course.hpp>
#include <canvas/resources/quiz.hpp>
#include <vector>

class State : public QObject, public Canvas::Logger
{
    Q_OBJECT
public:
    virtual ~State();
    static State& singleton();

    Canvas::Student& getStudent();
    Canvas::Session& getSession();

    /**
     * @brief
     * Unload resources, like courses and quizzes.
     */
    void reset(bool shuttingDown = false);

signals:
    void loggedOut();

private:
    State();

    Canvas::Student *mStudent;
    Canvas::Session mSession;
    static State *ginstance;
};

#endif // STATE_H
