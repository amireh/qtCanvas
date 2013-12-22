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
    virtual ~Viewport();
    static Viewport& singleton();

    void setLayout(QLayout*);
    void setView(QView*);
    void setStatusBar(QStatusBar*);
    void setStatus(QString const&);

    QStatusBar* getStatusBar() const;

    void transition(std::string const&);
    void registerView(std::string const&, QView*);

    QErrorMessage* errorDialog();

protected:
    QLayout *layout;
    QView *view;
    QStatusBar *statusBar;
    QErrorMessage *errorMessageDialog;

    std::map<std::string, QView*> views;
private:
    Viewport();
    static Viewport* gInstance;
};

#endif // VIEWPORT_H
