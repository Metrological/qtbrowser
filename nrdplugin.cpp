#include "glplugin.h"
#include <QPainter>
#include <stdio.h>
#include <QGraphicsScene>
#include <QImage>
#include <gibbon/external/MetrologicalEGL.h>

class NRDProgram {
public:
    static NRDProgram* instance() {
        if (!m_instance) {
            m_instance = new NRDProgram();
        }
        return m_instance;
    }

private:
    NRDProgram()
    {
    }

    static NRDProgram *m_instance;
};
NRDProgram* NRDProgram::m_instance = NULL;

NRDPlugin::NRDPlugin(
    : QGraphicsWidget(0)
    , m_context(0)
    , m_surface(0)
{
    // needed in order to get keyboard events
    setFlag(QGraphicsItem::ItemIsFocusable, true);
}

NRDPlugin::~NRDPlugin()
{
    delete m_context;
    delete m_surface;
}

/* virtual */ void NRDPlugin::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    // we don't really need our custom gl context, but it's safer
    // to use a new one to avoid messing with the rendering of the page content
    QOpenGLContext *prevContext = QOpenGLContext::currentContext();
    if (m_context == NULL) {
        m_surface = new QOffscreenSurface();
        m_surface->create();
        m_context = new QOpenGLContext();
        m_context->setShareContext(prevContext);
        m_context->create();

	// Time to create the app, we are now sure that we have the context
	NRDPlugin::instance();
    }
    // m_context->makeCurrent(m_surface);

    // prevContext->makeCurrent(prevContext->surface());
}

void NRDPlugin::keyPressEvent(QKeyEvent* event)
{
    printf("press event\n");
}

void NRDPlugin::keyReleaseEvent(QKeyEvent* event)
{
    printf("release event\n");
}

QVariant NRDPlugin::itemChange(GraphicsItemChange change, const QVariant& value)
{
    // when the widget gets visible and inside a scene it grabs the keyboard to get keyboard events
    // the grab will be released automatically when the widget gets hidden or out of the scene
    if (change == QGraphicsItem::ItemVisibleHasChanged || change == QGraphicsItem::ItemSceneHasChanged) {
        if (isVisible() && scene())
            grabKeyboard();
    }
   return QGraphicsItem::itemChange(change, value);
}
