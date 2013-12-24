#ifndef TAKE_QUIZ_HPP
#define TAKE_QUIZ_HPP

#include <QRadioButton>
#include <QLabel>
#include <QGroupBox>
#include <QButtonGroup>
#include <QAbstractButton>
#include <QMessageBox>
#include <QToolButton>
#include <QCheckBox>

#include "include/qview.hpp"
#include "include/state.h"
#include <canvas/resources/quiz.hpp>
#include <canvas/resources/quiz_submission.hpp>
#include <canvas/resources/quiz_question.hpp>
#include <canvas/resources/quiz_question_answer.hpp>
#include <canvas/resources/quiz_questions/multiple_choice.hpp>
#include <canvas/resources/quiz_submission_entry.hpp>
#include <canvas/resources/quiz_submission_answers/multiple_choice.hpp>
#include <map>
#include <functional>

namespace Ui {
class TakeQuiz;
}

using Canvas::QuizQuestion;

typedef Canvas::QuizQuestions::MultipleChoice MultipleChoiceQuestion;
typedef Canvas::QuizSubmissionAnswers::MultipleChoice MultipleChoiceAnswer;

class TakeQuiz : public QView, public Canvas::Logger
{
    Q_OBJECT

public:
    explicit TakeQuiz(QWidget *parent = 0);
    ~TakeQuiz();

    virtual void setup();
    virtual void cleanup();

private slots:
    void chooseMultipleChoiceAnswer(QAbstractButton*);
    void submitQuiz();
    void markQuestion(bool);
private:
    QTCANVAS_EXPORTS

    typedef void (TakeQuiz::*QuestionRenderer)(QuizQuestion*, QWidget*, QLayout*);

    Ui::TakeQuiz *ui;
    Canvas::Quiz *mQuiz;
    Canvas::QuizSubmission *mQuizSubmission;

    std::map<Canvas::String, QuestionRenderer> mRenderers;

    void renderQuestions();
    QLayout* renderQuestion(QuizQuestion*, QWidget*);
    void renderMultipleChoiceQuestion(QuizQuestion*, QWidget*, QLayout*);
};

#endif // TAKE_QUIZ_HPP
