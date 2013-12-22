#include "tests.hpp"
#include "include/take_quiz.hpp"

Tests::Tests()
{
}

Tests::~Tests()
{

}

void Tests::initTestCase()
{
}

void Tests::cleanupTestCase()
{
}

Canvas::String Tests::loadFixture(Canvas::String const& path) {
  Canvas::String out;
  Canvas::FileManager::singleton().load_file("../tests/fixture/" + path, out);
  return out;
}

void Tests::createQuestionWidgets()
{
    TakeQuiz view;
    Canvas::Course course(1);
    Canvas::Quiz quiz(1, &course);

    quiz.deserialize(loadFixture("quiz.json"));
    quiz.loadQuestions(loadFixture("quiz_questions.json"));

    State::singleton().setActiveQuiz(&quiz);

    QVERIFY(quiz.questions().size() > 0);
    QVERIFY(quiz.questions().front()->userData<QWidget>("QWidget") == NULL);

    view.setup();
    QVERIFY(quiz.questions().front()->userData<QWidget>("QWidget") != NULL);

    view.cleanup();
    QVERIFY(quiz.questions().front()->userData<QWidget>("QWidget") == NULL);
}


void Tests::testSomething()
{
    QVERIFY(false);
}

QTEST_MAIN(Tests)
