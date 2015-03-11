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

protected:
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);

private:
    QOpenGLContext* m_context;
    QOffscreenSurface* m_surface;
};

#endif // __PLUGIN__
