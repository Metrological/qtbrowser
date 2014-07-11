#include "graphicsview.h"

#include <QApplication>
#include <QWindow>
#include <QExposeEvent>

GraphicsView::GraphicsView()
{
}

GraphicsView::GraphicsView(QWidget* parent = 0) : QGraphicsView(parent)
{
}

bool GraphicsView::eventFilter(QObject* obj, QEvent* ev)
{
#ifdef QT_BUILD_WITH_OPENGL
    // forward the update events from the widgetWindow and "sink" them
    if (obj == dynamic_cast<QObject*>(QApplication::focusWindow()) && ev->type() == QEvent::Expose)
    {
        viewport()->update((dynamic_cast<QExposeEvent*>(ev))->region());

        return true;
    }
#endif
    return QGraphicsView::eventFilter(obj, ev);
}
