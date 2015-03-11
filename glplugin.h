#ifndef __GLPLUGIN__
#define __GLPLUGIN__

#include <QGraphicsWidget>
#include <QOpenGLContext>
#include <QOffscreenSurface>

class QPainter;

class GLPlugin : public QGraphicsWidget
{
    Q_OBJECT
public:
    GLPlugin();
    ~GLPlugin();
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

private:
    QOpenGLContext* m_context;
    QOffscreenSurface* m_surface;
};

#endif // __PLUGIN__
