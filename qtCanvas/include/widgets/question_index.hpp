#ifndef QUESTION_INDEX_HPP
#define QUESTION_INDEX_HPP

#include <QWidget>
#include <QListWidget>
#include <canvas/resources/quiz.hpp>
#include <canvas/resources/quiz_question.hpp>

namespace Ui {
class QuestionIndex;
}

using Canvas::Quiz;
using Canvas::QuizQuestion;

class QuestionIndex : public QWidget
{
    Q_OBJECT

public:
    explicit QuestionIndex(QWidget *parent = 0);
    ~QuestionIndex();

    void render(Quiz::Questions const &);

private:
    Ui::QuestionIndex *ui;

    QListWidgetItem* renderQuestionEntry(QuizQuestion const *qq, QListWidget *view);
};

#endif // QUESTION_INDEX_HPP
