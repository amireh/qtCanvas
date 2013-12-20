#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QWidget>
#include <QLayout>
#include <QStatusBar>
#include <map>
#include <canvas/logger.hpp>

class Viewport : public Canvas::Logger
{
public:
    virtual ~Viewport();
    static Viewport& singleton();

    void setLayout(QLayout*);
    void setView(QWidget*);
    void setStatusBar(QStatusBar*);
    void setStatus(QString const&);

    QStatusBar* getStatusBar() const;

    void transition(std::string const&);
    void registerView(std::string const&, QWidget*);

protected:
    QLayout *layout;
    QWidget *view;
    QStatusBar *statusBar;

    std::map<std::string, QWidget*> views;
private:
    Viewport();
    static Viewport* gInstance;
};

#endif // VIEWPORT_H
