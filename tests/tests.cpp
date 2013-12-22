#include "tests.hpp"
#include "include/take_quiz.hpp"
#include "include/mainwindow.h"
#include "include/type_exports.hpp"

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
    view.renderQuestions();
    QVERIFY(quiz.questions().front()->userData<QWidget>("QWidget") != NULL);

    view.cleanup();
    QVERIFY(quiz.questions().front()->userData<QWidget>("QWidget") == NULL);
}

int main(int argc, char *argv[])
{
    int rc;

    QTCANVAS_EXPORT_TYPES;

    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    Canvas::init(argc, argv);
    Canvas::Logger::unmute();

    Tests tc;

    MainWindow mainWindow;
//    mainWindow.show();

    QTEST_DISABLE_KEYPAD_NAVIGATION

    rc = QTest::qExec(&tc, argc, argv);

    Canvas::free();

    return rc;
}
