#ifndef __NRDPLUGIN__
#define __NRDPLUGIN__

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

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

protected:
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);

private:
    QOpenGLContext* m_context;
    QOffscreenSurface* m_surface;
};

#endif // __NRDPLUGIN__
