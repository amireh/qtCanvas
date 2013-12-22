#include "include/viewport.h"
#include <QLayout>

Viewport* Viewport::gInstance = nullptr;

Viewport::Viewport() : Logger("Viewport"), errorMessageDialog(nullptr)
{
//    errorMessageDialog = new QErrorMessage(this);
}

Viewport::~Viewport() {
    if (errorMessageDialog) {
        delete errorMessageDialog;
        errorMessageDialog = nullptr;
    }
}


Viewport& Viewport::singleton() {
    if (!gInstance) {
        gInstance = new Viewport();
    }

    return *gInstance;
}

void Viewport::setLayout(QLayout* inLayout) {
    layout = inLayout;
    errorMessageDialog = new QErrorMessage(layout->parentWidget());
}

void Viewport::setView(QView *inWidget) {
    if (!layout) {
        throw "contentFrame must be set before assigning content";
    }

    if (view) {
        debug() << "detaching view" << view;
        layout->removeWidget(view);
        view->cleanup();
        view->hide();
    }

    view = inWidget;
    layout->addWidget(view);
    view->show();
    view->setup();

    debug() << "attaching view" << view;
}

void Viewport::setStatusBar(QStatusBar *inStatusBar)
{
    statusBar = inStatusBar;
}

void Viewport::setStatus(const QString &message)
{
    statusBar->showMessage(message);
}

void Viewport::transition(const std::string &viewId)
{
    QView* view = views[viewId];

    if (!view) {
        throw "no such view to transition to" + viewId;
    }

    setView(view);
}

void Viewport::registerView(const std::string &viewId, QView *view)
{
    views[viewId] = view;
}

QErrorMessage *Viewport::errorDialog()
{
    return errorMessageDialog;
}
