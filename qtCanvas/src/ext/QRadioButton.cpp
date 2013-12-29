#include "include/ext/QRadioButton.hpp"
#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>

namespace qtCanvas {

QRadioButton::QRadioButton(QWidget *parent) :
    ::QWidget(parent)
{
    buildCustomLayout();
    buildButton();
    buildTextHolder();
}

QRadioButton::QRadioButton(const QString &text, QWidget *parent) :
    ::QWidget(parent)
{
    buildCustomLayout();
    buildButton();
    buildTextHolder();
    setText(text);
}

QRadioButton::~QRadioButton()
{
}

void QRadioButton::setText(const QString &text)
{
    mText->setText(text);
}

QHBoxLayout *QRadioButton::cLayout() const
{
    return mCustomLayout;
}

::QRadioButton *QRadioButton::cButton() const
{
    return mButton;
}

QLabel *QRadioButton::cLabel() const
{
    return mText;
}

bool QRadioButton::eventFilter( QObject * o, QEvent * e ) {
    if ( e->type() == QEvent::MouseButtonRelease && qobject_cast<QLabel*>( o ) ) {
//        mButton->setChecked(true);
        mButton->click();
        return true;
    }

    return QWidget::eventFilter( o, e );
}

void QRadioButton::buildCustomLayout()
{
    mCustomLayout = new QHBoxLayout(this);
    mCustomLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mCustomLayout->setContentsMargins(0,0,0,0);
    mCustomLayout->setMargin(0);
}

void QRadioButton::buildButton()
{
    mButton = new ::QRadioButton(this);
    mCustomLayout->addWidget(mButton);
}

void QRadioButton::buildTextHolder()
{
    mText = new QLabel(this);
    mText->setWordWrap(true);
    mText->setAlignment(Qt::AlignLeft | Qt::AlignTop);
//    mText->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    mText->installEventFilter(this);

    mCustomLayout->addWidget(mText);
//    mCustomLayout->addStretch();
}


} // namespace qtCanvas
