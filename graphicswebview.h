#ifndef __GRAPHICSWEBVIEW__
#define __GRAPHICSWEBVIEW__

#include <QGraphicsWebView>
#include <QGraphicsSceneContextMenuEvent>

class GraphicsWebView : public QGraphicsWebView
{
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* ev);
};

#endif
