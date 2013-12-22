#ifndef AVAILABLE_QUIZZES_H
#define AVAILABLE_QUIZZES_H

#include <QWidget>
#include <QTreeWidget>
#include <canvas/resources/course.hpp>
#include <canvas/resources/quiz.hpp>
#include <canvas/resources/quiz_submission.hpp>
#include <canvas/logger.hpp>
#include "include/qview.hpp"

namespace Ui {
    class AvailableQuizzes;
}

class AvailableQuizzes : public QView, public Canvas::Logger
{
    Q_OBJECT

public:
    explicit AvailableQuizzes(QWidget *parent = 0);
    ~AvailableQuizzes();

    virtual void setup();
    virtual void cleanup();

signals:
    void courseLoaded(Canvas::Course const&);
    void quizLoaded(Canvas::Quiz const&);
    void quizSubmissionLoaded(Canvas::QuizSubmission const&);

private slots:
    /**
     * @brief addCourse
     * Look up, or create if necessary, a tree item to represent the course.
     *
     * @param skipTest
     * Pass as true to skip testing if the course item has already been created,
     * ie, you're sure it hasn't.
     *
     * @return The course tree item.
     */
    QTreeWidgetItem* addCourse(Canvas::Course const&, bool skipTest = false);

    /**
     * @brief addQuiz
     * Add a tree-item to represent the quiz.
     */
    void addQuiz(Canvas::Quiz const&);

    /**
     * @brief reset
     * Clear all the course and quiz items in the tree.
     */
    void reset();

    /**
     * @brief updateQuizActions
     * Update the possible actions on a Quiz when the quiz selection changes,
     * stuff like disabling/enabling the "Take Quiz" button, etc.
     */
    void updateQuizActions(QTreeWidgetItem*, QTreeWidgetItem*);

    /**
     * @brief loadCourseQuizzes
     * Load the quizzes for each course once it gets loaded.
     */
    void loadCourseQuizzes(Canvas::Course const&);

    /**
     * @brief loadQuizSubmission
     * Load the QS for the newly-loaded quiz, if any.
     */
    void loadQuizSubmission(Canvas::Quiz const&);

    /**
     * @brief updateQuizStatus
     * Update the UI to show the Quiz "taking" status; whether a QS is active,
     * complete, or doesn't exist.
     */
    void updateQuizStatus(Canvas::Quiz const&);
    void updateQuizStatus(Canvas::QuizSubmission const&);

    /**
     * @brief takeQuiz
     * Start taking the selected quiz.
     */
    void takeQuiz();

private:
    Ui::AvailableQuizzes *ui;
    QTreeWidgetItem* courseTreeItem(Canvas::Course const&);
    QTreeWidgetItem* quizTreeItem(Canvas::Quiz const&);
};

#endif // AVAILABLE_QUIZZES_H
