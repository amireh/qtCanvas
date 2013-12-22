#ifndef QVIEW_HPP
#define QVIEW_HPP

#include <QWidget>
#include <canvas/canvas.hpp>

class QView : public QWidget
{
    Q_OBJECT
public:
    explicit QView(QWidget *parent = 0);

    virtual void setup();
    virtual void cleanup();

protected:
    virtual void setStatus(QString const&);
//    virtual void setStatus(Canvas::String const&);
};

#endif // QVIEW_HPP
