#include "include/qview.hpp"
#include "include/viewport.h"

QView::QView(QWidget *parent) :
    QWidget(parent)
{
}

void QView::setup()
{
}

void QView::cleanup()
{
}

void QView::setStatus(const QString &status)
{
    Viewport::singleton().setStatus(status);
}

//void QView::setStatus(const Canvas::String &status)
//{
//    setStatus(QString::fromStdString(status));
//}
