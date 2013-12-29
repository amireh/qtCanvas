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
#include <QScrollArea>
#include <QScrollBar>

#include "include/qview.hpp"
#include "include/state.h"
#include "qtcanvas.hpp"
#include <canvas/resources/quiz.hpp>
#include <canvas/resources/quiz_submission.hpp>
#include <canvas/resources/quiz_question.hpp>
#include <canvas/resources/quiz_question_answer.hpp>
#include <canvas/resources/quiz_questions/multiple_choice.hpp>
#include <map>
#include <functional>

#include "question_renderer.hpp"
#include "widgets/question_index.hpp"
#include "widgets/question_widget.hpp"
#include "widgets/qclock.hpp"

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
    virtual ~TakeQuiz();

    virtual void setup();
    virtual void cleanup();

    virtual QuestionRenderer* generateRenderer(QuizQuestion *);

private slots:
    void submitQuiz();
    void saveAnswer(const QuizQuestion *qq);

protected:
    typedef std::vector<QuestionRenderer*> Renderers;

    Ui::TakeQuiz *ui;
    Canvas::Quiz *mQuiz;
    Canvas::QuizSubmission *mQuizSubmission;
    QuestionIndex *mQuestionIndex;
    Renderers mRenderers;
    int mPresentationFlags;

    void renderElapsedTimer();
    virtual void renderQuestions();
    QWidget *renderQuestion(QuizQuestion *question);
    QFrame *renderQuestionFrame(QuizQuestion*, QWidget*);
    QWidget * renderAnswerFrame(QWidget *qqWidget);

};

#endif // TAKE_QUIZ_HPP
