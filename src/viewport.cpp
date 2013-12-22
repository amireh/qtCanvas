#include "include/viewport.h"
#include <QLayout>

Viewport* Viewport::gInstance = nullptr;

Viewport::Viewport()
    : Logger("Viewport"),
      mLayout(nullptr),
      mView(nullptr),
      errorMessageDialog(nullptr)
{
//    errorMessageDialog = new QErrorMessage(this);
}

Viewport::~Viewport() {
    if (errorMessageDialog) {
        delete errorMessageDialog;
        errorMessageDialog = nullptr;
    }

    if (mView) {
        delete mView;
        mView = nullptr;
    }
}


Viewport& Viewport::singleton() {
    if (!gInstance) {
        gInstance = new Viewport();
    }

    return *gInstance;
}

void Viewport::setLayout(QLayout* inLayout) {
    mLayout = inLayout;
    errorMessageDialog = new QErrorMessage(mLayout->parentWidget());
}

void Viewport::transition(const std::string &viewType)
{
    if (!views[viewType]) {
        throw std::runtime_error("No generator found for view " + viewType);
    }

    assertLayoutSet();

    ViewGenerator& generator = views[viewType];
    QView *view;

    if (mView) {
        detach(mView);
        delete mView;
        mView = nullptr;
    }

    view = generator();
    attach(view);

    mView = view;
}

void Viewport::attach(QView *view) {
    mLayout->addWidget(view);
    view->show();
    view->setup();

    debug() << "attaching view" << view;
}

void Viewport::detach(QView *view)
{
    debug() << "detaching view" << view;

    mLayout->removeWidget(view);
    view->cleanup();
    view->hide();
}

void Viewport::assertLayoutSet()
{
    if (!mLayout) {
        throw std::runtime_error("Viewport layout must be set before attaching views");
    }
}

void Viewport::setStatusBar(QStatusBar *inStatusBar)
{
    statusBar = inStatusBar;
}

void Viewport::setStatus(const QString &message)
{
    statusBar->showMessage(message);
}

void Viewport::registerView(const std::string &viewId, ViewGenerator generator)
{
    views[viewId] = generator;
}

QErrorMessage *Viewport::errorDialog()
{
    return errorMessageDialog;
}
