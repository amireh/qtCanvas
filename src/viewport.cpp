#include "include/viewport.h"
#include <QLayout>

Viewport* Viewport::gInstance = nullptr;

Viewport::Viewport()
    : Logger("Viewport"),
      mLayout(nullptr),
      mView(nullptr),
      errorMessageDialog(nullptr)
{
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

void Viewport::transition(const QString &viewType)
{
    ViewGenerators::iterator locator = mViews.find(viewType);
    if (locator == mViews.end()) {
        throw std::runtime_error("No generator found for view " + viewType.toStdString());
    }

    assertLayoutSet();

    ViewGenerator& generator = locator->second;

    if (mView) {
        detach(mView);
        delete mView;
        mView = nullptr;
    }

    mView = generator();
    attach(mView);
}

void Viewport::registerDialog(const QString &dialogId, Viewport::DialogGenerator generator)
{
    mDialogGenerators.insert(std::make_pair(dialogId, generator));
}

void Viewport::attach(QView *view) {
    mLayout->addWidget(view);
    view->show();
    view->setup();

//    if (mMainWindow) {
//        QTimer::singleShot(1, this, SLOT(fitToContents()));
//    }

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
    mStatusBar = inStatusBar;
}

void Viewport::setToolBar(QToolBar *toolBar)
{
    mToolBar = toolBar;
}

void Viewport::setStatus(const QString &message)
{
    mStatusBar->showMessage(message);
}

void Viewport::setMainWindow(QMainWindow *mainWindow)
{
    mMainWindow = mainWindow;
}

QToolBar *Viewport::getToolBar() const
{
    return mToolBar;
}

void Viewport::registerView(const QString &viewId, ViewGenerator generator)
{
    mViews.insert(std::make_pair(viewId, generator));
}

int Viewport::showDialog(const QString &id)
{
    DialogGenerators::const_iterator dialogGenerator = mDialogGenerators.find(id);

    if (dialogGenerator == mDialogGenerators.end()) {
        throw std::invalid_argument("Can not create dialogs of type " + id.toStdString());
    }

    QDialog *dialog = dialogGenerator->second();

    connect(dialog, SIGNAL(finished(int)), dialog, SLOT(deleteLater()));

    return dialog->exec();
}

QErrorMessage *Viewport::errorDialog()
{
    return errorMessageDialog;
}

void Viewport::fitToContents()
{
    assert(mMainWindow);

    if (mMainWindow->isMaximized()) {
        warn() << "Window is maximized, will not attempt to fit.";
        return;
    }

    QSize hint = mMainWindow->sizeHint();

    mMainWindow->centralWidget()->resize(hint);
    mMainWindow->resize(hint);
}
