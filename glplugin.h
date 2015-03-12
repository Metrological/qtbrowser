#ifndef __GLPLUGIN__
#define __GLPLUGIN__

#include <QGraphicsWidget>
#include <QOpenGLContext>
#include <QOffscreenSurface>
#include <QWebFrame>

class QPainter;

class GLPlugin : public QGraphicsWidget
{
    Q_OBJECT
public:
    GLPlugin();
    ~GLPlugin();
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

public slots:
    void setNumber(int n);

signals:
    void numberSet(int n);

protected:
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);

private slots:
    void attachObject();

private:
    QOpenGLContext* m_context;
    QOffscreenSurface* m_surface;
    QWebFrame* m_frame;
};

#endif // __PLUGIN__
