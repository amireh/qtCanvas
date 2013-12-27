#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QWidget>
#include <QLayout>
#include <QStatusBar>
#include <QToolBar>
#include <QErrorMessage>
#include <map>
#include <canvas/logger.hpp>
#include "include/qview.hpp"

class Viewport : public Canvas::Logger
{
public:
    typedef std::function<QView*()> ViewGenerator;
    typedef std::function<QDialog*()> DialogGenerator;

    virtual ~Viewport();
    static Viewport& singleton();

    void setLayout(QLayout*);
    void setStatusBar(QStatusBar*);
    void setToolBar(QToolBar*);
    void setStatus(QString const&);

    QToolBar* getToolBar() const;
    QStatusBar* getStatusBar() const;

    /**
     * @brief registerView
     * Add a new view generator to the view registry.
     *
     * Views are allocated on-demand, when a request to transition to them is
     * received by the Viewport. So, you don't have to explicitly manage the
     * view objects.
     */
    void registerView(QString const&, ViewGenerator);

    /**
     * @brief transition
     * Hide the current view (if any) and show the specified one.
     */
    void transition(QString const&);

    /**
     * @brief registerDialog
     * Add a new dialog generator to the dialog registry.
     *
     * Like views, dialog instances are handled internally by the Viewport.
     */
    void registerDialog(QString const&, DialogGenerator);

    /**
     * @brief showDialog
     * Queue a request to display a certain dialog.
     *
     * @throw std::invalid_argument
     * If no dialog generator was found for the request dialog.
     *
     * @return The return value of QDialog::exec()
     */
    int showDialog(QString const&);

    QErrorMessage* errorDialog();

protected:
    typedef std::map<QString, ViewGenerator> ViewGenerators;
    typedef std::map<QString, DialogGenerator> DialogGenerators;

    void attach(QView*);
    void detach(QView*);
    void assertLayoutSet();

    QLayout *mLayout;
    QView *mView;
    QDialog *mDialog;
    QStatusBar *mStatusBar;
    QToolBar *mToolBar;
    QErrorMessage *errorMessageDialog;
    ViewGenerators mViews;
    DialogGenerators mDialogs;
private:
    Viewport();
    static Viewport* gInstance;
};

#endif // VIEWPORT_H
