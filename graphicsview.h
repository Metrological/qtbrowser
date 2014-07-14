#ifndef __GRAPHICSVIEW__
#define GRAPHICSVIEW__

#include <QGraphicsView>
#include <QWidget>
#include <QObject>
#include <QEvent>

class GraphicsView : public QGraphicsView
{
public:
    GraphicsView();
    GraphicsView(QWidget* parent);

protected:
    bool eventFilter(QObject* obj, QEvent* ev);
};
#endif //__GRAPHICSVIEW__
