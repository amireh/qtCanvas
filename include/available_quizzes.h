#ifndef AVAILABLE_QUIZZES_H
#define AVAILABLE_QUIZZES_H

#include <QWidget>
#include <QTreeWidget>
#include <canvas/resources/course.hpp>
#include <canvas/resources/quiz.hpp>
#include <canvas/logger.hpp>

namespace Ui {
class AvailableQuizzes;
}

class AvailableQuizzes : public QWidget, public Canvas::Logger
{
    Q_OBJECT

public:
    explicit AvailableQuizzes(QWidget *parent = 0);
    ~AvailableQuizzes();

private slots:
    QTreeWidgetItem* addCourse(Canvas::Course const&, bool skipTest = false);
    void addCourseQuiz(Canvas::Quiz const&);
    void reset();

private:
    Ui::AvailableQuizzes *ui;
    QTreeWidgetItem* courseTreeItem(Canvas::Course const&);
};

#endif // AVAILABLE_QUIZZES_H
