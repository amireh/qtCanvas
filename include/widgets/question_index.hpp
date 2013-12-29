#ifndef QUESTION_INDEX_HPP
#define QUESTION_INDEX_HPP

#include <QWidget>
#include <QListWidget>
#include <QScrollArea>
#include <QToolButton>
#include <canvas/resources/quiz.hpp>
#include <canvas/resources/quiz_question.hpp>
#include <canvas/resources/quiz_submission.hpp>
#include <canvas/logger.hpp>
#include "widgets/question_widget.hpp"
#include "qtCanvas.hpp"

namespace Ui {
class QuestionIndex;
}

using Canvas::Quiz;
using Canvas::QuizQuestion;

class QuestionIndex : public QWidget, public Canvas::Logger
{
    Q_OBJECT

public:
    explicit QuestionIndex(QWidget *parent = 0);
    ~QuestionIndex();

    void render(Quiz::Questions const &, QScrollArea*, int presentationFlags = QuizPresentation::Normal);

signals:
    void questionFocused(QuizQuestion const*, QuestionWidget *);
private slots:
    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void selectFocusedQuestion(QuestionWidget*);
    void updateQuestionStatus(const QuizQuestion*);
    void markQuestion();
private:
    Ui::QuestionIndex *ui;

    QListWidgetItem* renderQuestionEntry(QuizQuestion const *qq, QListWidget *view);
    QScrollArea *mScrollArea;
    bool mInternalSelectionUpdate;

    void updateQuestionStatus(const Canvas::QuizQuestion *, QListWidgetItem*);
    void updateMarkButton(QuizQuestion const*);

    QuizQuestion *selectedQuestion();
};

#endif // QUESTION_INDEX_HPP
