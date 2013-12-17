#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QWidget>
#include <QLayout>

class Viewport
{
public:
    virtual ~Viewport();
    static Viewport& singleton();

    void setLayout(QLayout*);
    void setContent(QWidget*);

protected:
    QLayout *layout;
    QWidget *content;

private:
    Viewport();
    static Viewport* gInstance;
};

#endif // VIEWPORT_H
