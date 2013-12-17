#ifndef AVAILABLE_QUIZZES_H
#define AVAILABLE_QUIZZES_H

#include <QWidget>

namespace Ui {
class AvailableQuizzes;
}

class AvailableQuizzes : public QWidget
{
    Q_OBJECT

public:
    explicit AvailableQuizzes(QWidget *parent = 0);
    ~AvailableQuizzes();

private:
    Ui::AvailableQuizzes *ui;
};

#endif // AVAILABLE_QUIZZES_H
