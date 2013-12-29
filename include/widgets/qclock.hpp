#ifndef QCLOCK_HPP
#define QCLOCK_HPP

#include <QLabel>
#include <QTimer>
#include <QTime>

class QClock : public QLabel
{
    Q_OBJECT

public:
    explicit QClock(QWidget *parent = 0);
    virtual ~QClock();

private slots:
    void tick();
private:
    QTime mStartTime;
};

#endif // QCLOCK_HPP
