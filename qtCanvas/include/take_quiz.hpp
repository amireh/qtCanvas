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
#include <map>
#include <functional>

#include "question_renderer.hpp"

namespace Ui {
class TakeQuiz;
}

using Canvas::QuizQuestion;

typedef Canvas::QuizQuestions::MultipleChoice MultipleChoiceQuestion;

class TakeQuiz : public QView, public Canvas::Logger
{
    Q_OBJECT

public:
    explicit TakeQuiz(QWidget *parent = 0);
    ~TakeQuiz();

    virtual void setup();
    virtual void cleanup();

    virtual QuestionRenderer* generateRenderer(QuizQuestion *);

private slots:
    void submitQuiz();
    void markQuestion(bool);
    void saveAnswer(const QuizQuestion *qq);
private:
    typedef std::vector<QuestionRenderer*> Renderers;

    Ui::TakeQuiz *ui;
    Canvas::Quiz *mQuiz;
    Canvas::QuizSubmission *mQuizSubmission;

    Renderers mRenderers;

    void renderQuestions();
    QLayout* renderQuestionFrame(QuizQuestion*, QWidget*);
    QWidget * renderAnswerFrame(QWidget *qqWidget);

};

#endif // TAKE_QUIZ_HPP
