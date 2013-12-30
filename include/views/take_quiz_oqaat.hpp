#ifndef TAKE_QUIZ_OQAAT_HPP
#define TAKE_QUIZ_OQAAT_HPP

#include "views/take_quiz.hpp"

/**
 * @class TakeQuizOQAAT
 * @brief
 * A take-quiz view that renders a single question at a time, referred to as
 * the focused question. It also allows the student to navigate between
 * questions if the Quiz allows it.
 *
 * See Canvas::Quiz::cantGoBack() and Canvas::Quiz::isOQAAT().
 */
class TakeQuizOQAAT : public TakeQuiz
{
    Q_OBJECT
public:

    TakeQuizOQAAT(QWidget *parent = 0);
    virtual ~TakeQuizOQAAT();

    /**
     * @brief setup
     * Invoke TakeQuiz::setup() and then add the OQAAT navigation buttons.
     */
    virtual void setup();

private slots:
    /**
     * @brief prevQuestion
     * Focus the closest un-answered question before the focused one.
     */
    void prevQuestion();

    /**
     * @brief nextQuestion
     * Focus the closest un-answered question after the focused one.
     */
    void nextQuestion();

    /**
     * @brief focusQuestion
     * Acceptor for the signal emitted by the QuestionIndex when the student
     * manually focuses a question from the list.
     *
     * Delegates to focusQuestion() with no broadcast.
     */
    void requestFocusQuestion(const QuizQuestion *question, QuestionWidget *);

protected:
    /**
     * @brief clearLayout
     * Remove all QWidgets and spacer items in the given layout.
     *
     * @param[in] layout
     * The layout to clear, defaults to TakeQuizOQAAT::questionLayout();
     */
    void clearLayout(QVBoxLayout * = nullptr);

    /**
     * @override
     * @brief renderQuestions
     *
     * Focuses the first un-answered question after rendering their widgets.
     */
    virtual void renderQuestions();

    /**
     * @override
     * @brief renderActions
     * Attach a "Next" question button to the questionLayout(), and if the Quiz
     * allows it, attach a "Previous" question button as well.
     */
    virtual void renderActions(QHBoxLayout *);

    virtual void updateActionState();

    /**
     * @brief questionLayout
     * Convenience method for casting / retrieving the question layout.
     *
     * @return The question viewport layout.
     */
    virtual QVBoxLayout *questionLayout();

    /**
     * @brief focusQuestion
     * Clear the layout of the currently focused question, if any, then attach
     * the given question to the viewport.
     *
     * @param[in] question
     * The question to focus.
     * @param[in] broadcast
     * When true, QuestionWidget::focused() will be emitted so that other
     * interested components may update.
     */
    void focusQuestion(QuizQuestion *question, bool broadcast = true);

    /**
     * @brief locatePrevQuestion
     * Convenience method for locating the closest un-answered question before
     * the focused one.
     *
     * @param[in,out] cursor
     * An integer that will contain the index of the located question, if any.
     *
     * @return
     * The located question, or nullptr otherwise.
     */
    QuizQuestion *locatePrevQuestion(int* cursor = nullptr) const;

    /**
     * @brief locateNextQuestion
     * Convenience method for locating the closest un-answered question after
     * the focused one.
     *
     * @param[in,out] cursor
     * An integer that will contain the index of the located question, if any.
     *
     * @return
     * The located question, or nullptr otherwise.
     */
    QuizQuestion *locateNextQuestion(int* cursor = nullptr) const;

    /**
     * @brief locateNextQuestion
     * Convenience method for locating the closest un-answered question after
     * the focused one.
     *
     * @param[in] forward
     * Whether the direction of the search should be forward, or backward from
     * the given position.
     *
     * @param[in] position
     * The position to start the search from.
     *
     * @param[in,out] cursor
     * An integer that will contain the index of the located question, if any.
     *
     * @return
     * The located question, or nullptr otherwise.
     */
    QuizQuestion *locateQuestion(bool forward, int position, int *cursor = nullptr) const;

    virtual bool isFocusable(QuizQuestion const*) const;

    /**
     * @property mCursor
     * Index of the focused question.
     */
    int mCursor;

    /**
     * @property mQuestion
     * The focused question.
     */
    QuizQuestion *mQuestion;

    /**
     * @property mSinkWidget
     * Questions that are out of focus are "sinked" into this invisible widget.
     */
    QWidget *mSinkWidget;

    /**
     * @property mSinkLayout
     * A dummy layout for the sink widget.
     */
    QLayout *mSinkLayout;

    QPushButton *mNextButton;
    QPushButton *mPrevButton;
};
#endif // TAKE_QUIZ_OQAAT_HPP
