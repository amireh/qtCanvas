#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QWidget>
#include <QLayout>
#include <QStatusBar>
#include <QErrorMessage>
#include <map>
#include <canvas/logger.hpp>
#include "include/qview.hpp"

class Viewport : public Canvas::Logger
{
public:
    typedef std::function<QView*()> ViewGenerator;

    virtual ~Viewport();
    static Viewport& singleton();

    void setLayout(QLayout*);
    void setStatusBar(QStatusBar*);
    void setStatus(QString const&);

    QStatusBar* getStatusBar() const;

    void transition(std::string const&);
    void registerView(std::string const&, ViewGenerator);

    QErrorMessage* errorDialog();

protected:
    void attach(QView*);
    void detach(QView*);
    void assertLayoutSet();

    QLayout *mLayout;
    QView *mView;
    QStatusBar *statusBar;
    QErrorMessage *errorMessageDialog;

    std::map<std::string, ViewGenerator> views;
private:
    Viewport();
    static Viewport* gInstance;
};

#endif // VIEWPORT_H
