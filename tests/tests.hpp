#include <QString>
#include <QtTest>
#include <canvas/canvas.hpp>

class Tests : public QObject
{
    Q_OBJECT

public:
    Tests();
    virtual ~Tests();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void createQuestionWidgets();
    void populatesMultipleChoiceQuestionAnswers();

private:
    Canvas::String loadFixture(QString const& path);
};
