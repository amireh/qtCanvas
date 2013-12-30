#ifndef QVIEW_HPP
#define QVIEW_HPP

#include <QWidget>

class QView : public QWidget
{
    Q_OBJECT
public:
    explicit QView(QWidget *parent = 0);

    virtual void setup();
    virtual void cleanup();

protected:
    virtual void setStatus(QString const&);
    virtual void reportError(QString const& message, QString type = "");
};

#endif // QVIEW_HPP
