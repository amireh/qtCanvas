#ifndef QTCANVAS_QRADIOBUTTON_HPP
#define QTCANVAS_QRADIOBUTTON_HPP

#include <QWidget>
#include <QEvent>

class QLabel;
class QRadioButton;
class QHBoxLayout;

namespace qtCanvas {

class QRadioButton : public QWidget
{
    Q_OBJECT
public:
    explicit QRadioButton(QWidget *parent = 0);
    explicit QRadioButton(const QString &text, QWidget *parent=0);
    virtual ~QRadioButton();

    virtual void setText(const QString &text);
    virtual QHBoxLayout *cLayout() const;
    virtual ::QRadioButton *cButton() const;
    virtual QLabel *cLabel() const;
signals:

public slots:

protected:
    virtual bool eventFilter(QObject *, QEvent *);
    QHBoxLayout *mCustomLayout;
    QLabel *mText;
    ::QRadioButton *mButton;

    void buildCustomLayout();
    void buildButton();
    void buildTextHolder();
};

} // namespace qtCanvas

#endif // QTCANVAS_QRADIOBUTTON_HPP
