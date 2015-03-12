#ifndef __GLPLUGIN__
#define __GLPLUGIN__

#include <QGraphicsWidget>
#include <QOpenGLContext>
#include <QOffscreenSurface>

class QPainter;

class NRDPlugin : public QGraphicsWidget
{
    Q_OBJECT
public:
    NRDPlugin();
    ~NRDPlugin();

protected:
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);

private:
    QOpenGLContext* m_context;
    QOffscreenSurface* m_surface;
};

#endif // __PLUGIN__
