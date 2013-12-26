#ifndef QUESTION_INDEX_HPP
#define QUESTION_INDEX_HPP

#include <QWidget>
#include <QListWidget>
#include <QScrollArea>
#include <canvas/resources/quiz.hpp>
#include <canvas/resources/quiz_question.hpp>
#include "widgets/question_widget.hpp"

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

    void render(Quiz::Questions const &, QScrollArea*);

private slots:
    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void updateSelection(QuestionWidget*);

private:
    Ui::QuestionIndex *ui;

    QListWidgetItem* renderQuestionEntry(QuizQuestion const *qq, QListWidget *view);
    QScrollArea *mScrollArea;
    bool mInternalSelectionUpdate;
};

#endif // QUESTION_INDEX_HPP
