#include "include/viewport.h"
#include <QLayout>

Viewport* Viewport::gInstance = nullptr;

Viewport::Viewport() : Logger("Viewport")
{
}

Viewport::~Viewport() {
}


Viewport& Viewport::singleton() {
    if (!gInstance) {
        gInstance = new Viewport();
    }

    return *gInstance;
}

void Viewport::setLayout(QLayout* inLayout) {
    layout = inLayout;
}

void Viewport::setView(QWidget *inWidget) {
    if (!layout) {
        throw "contentFrame must be set before assigning content";
    }

    if (view) {
        debug() << "detaching view" << view;
        layout->removeWidget(view);
        view->hide();
    }

    view = inWidget;
    layout->addWidget(view);
    view->show();

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
    QWidget* view = views[viewId];

    if (!view) {
        throw "no such view to transition to" + viewId;
    }

    setView(view);
}

void Viewport::registerView(const std::string &viewId, QWidget *view)
{
    views[viewId] = view;
}
