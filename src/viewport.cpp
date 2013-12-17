#include "include/viewport.h"
#include <QLayout>

Viewport* Viewport::gInstance = nullptr;

Viewport::Viewport()
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

void Viewport::setContent(QWidget *inWidget) {
    if (!layout) {
        throw "contentFrame must be set before assigning content";
    }

    content = inWidget;
    layout->addWidget(content->window());
}
