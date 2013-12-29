#include "widgets/qtextedit.hpp"

namespace qtCanvas {

    QTextEdit::QTextEdit(QWidget *parent) :
        ::QTextEdit(parent)
    {
    }


    void QTextEdit::focusOutEvent(QFocusEvent *e)
    {
        emit editingFinished();
        ::QTextEdit::focusOutEvent(e);
    }
}
