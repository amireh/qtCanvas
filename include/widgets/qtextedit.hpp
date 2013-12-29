#ifndef QTEXTEDIT_HPP
#define QTEXTEDIT_HPP

#include <QTextEdit>

namespace qtCanvas {
    class QTextEdit : public ::QTextEdit
    {
        Q_OBJECT
    public:
        explicit QTextEdit(QWidget *parent = 0);

    signals:
        void editingFinished();

    protected:
        virtual void focusOutEvent(QFocusEvent *);

    };
}

#endif // QTEXTEDIT_HPP
