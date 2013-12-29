#include "include/widgets/qclock.hpp"

QClock::QClock(QWidget *parent) :
    QLabel(parent),
    mStartTime()
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
    timer->start(1000);
    mStartTime.setHMS(0,0,0,0);
}

QClock::~QClock()
{
}

void QClock::tick()
{
    mStartTime = mStartTime.addSecs(1);
    setText(mStartTime.toString("hh:mm:ss"));
}
