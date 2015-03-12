#include "glplugin.h"

#include <QPainter>
#include <stdio.h>
#include <QGraphicsScene>
#include <QImage>
#include <gibbon/external/MetrologicalEGL.h>
#include <nrdbase/config.h>
#include <gibbon/config.h>
#include <nrdbase/Configuration.h>
#include <nrdbase/Thread.h>

#include <GibbonConfiguration.h>
#include <GibbonApplication.h>
#include <Transform.h>

#include <nrdapp/AppLog.h>
#include <nrdapp/Version.h>
#include <nrdapp/AppThread.h>

#include <stdio.h>
#include <errno.h>

// using namespace std;
using namespace netflix;
// using namespace netflix::base;
using namespace netflix::application;
using namespace netflix::application::gibbon;

#include <stdlib.h>

class NRDProgram: public netflix::base::Thread
{
public:
    static NRDProgram* instance() {
        if (!m_instance) {
            m_instance = new NRDProgram();
        }
        return m_instance;
    }

private:
    NRDProgram () : netflix::base::Thread(&THREAD_GIBBON_MAIN)
    {
    }

    virtual ~NRDProgram()
    {
    }

    virtual void Run()
    {
      netflix::base::Thread::Adopt(&THREAD_GIBBON_MAIN);

      netflix::application::gibbon::GibbonApplication::setArgs(argc, argv);

      if (netflix::application::gibbon::GibbonConfiguration::init(argc, argv)) {
      {
        gibbon_oem_event(GibbonOEM_Init);

        int ret;
        {
          netflix::application::gibbon::GibbonApplication app;

          ret = app.exec() ? 0 : 1;

          gibbon_oem_event(GibbonOEM_Cleanup);
        }
      }

      netflix::base::Thread::Disown();
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
