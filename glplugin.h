#ifndef __GLPLUGIN__
#define __GLPLUGIN__

#include <QGraphicsWidget>

class QPainter;

class GLPlugin : public QGraphicsWidget
{
    Q_OBJECT
public:
    GLPlugin();
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
};

#endif // __PLUGIN__
