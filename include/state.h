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
#include <canvas/resources/quiz_submission.hpp>
#include <canvas/resources/quiz_question.hpp>
#include <vector>

using Canvas::QuizQuestion;

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

    void setActiveQuiz(Canvas::Quiz*);
    void setActiveQuizSubmission(Canvas::QuizSubmission*);

    Canvas::Quiz* activeQuiz();
    Canvas::QuizSubmission* activeQuizSubmission();
signals:
    void loggedOut();
    void questionModified(QuizQuestion const*);

private:
    State();

    static State *ginstance;

    Canvas::Student *mStudent;
    Canvas::Session mSession;
    Canvas::Quiz *mActiveQuiz;
    Canvas::QuizSubmission *mActiveQuizSubmission;
};

#endif // STATE_H
