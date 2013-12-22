#include <QString>
#include <QtTest>
#include <canvas/canvas.hpp>
#include <canvas/file_manager.hpp>

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
    void testSomething();

private:
    Canvas::String loadFixture(Canvas::String const& path);
};
