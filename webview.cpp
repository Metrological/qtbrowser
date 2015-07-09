#include "webview.h"

//WK1
#include "graphicsview.h"
#include "graphicswebview.h"
#ifdef QT_BUILD_WITH_OPENGL
#include <QtOpenGL/QGLWidget>
#endif

//WK2
#ifdef QT_BUILD_WITH_QML_API
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlComponent>
#endif

#ifdef QT_BUILD_WITH_OPENGL
#include <QCoreApplication>
#endif

#ifdef QT_BUILD_WITH_QML_API
#include <QQmlProperty>
#include <QQuickItem>

#define QML_URL ""

#define QML_DATA        \
"\n"                    \
"import QtQuick 2.0\n"  \
"import QtWebKit 3.0\n" \
"import QtWebKit.experimental 1.0\n" \
"\n"                    \
"WebView {\n"           \
"    id : webView\n"    \
"    experimental.preferences.webAudioEnabled: true\n" \
"    experimental.alertDialog: Item {\n" \
"            Timer {\n" \
"                interval: 1\n" \
"                running: true\n" \
"                onTriggered: {\n" \
"                  console.log(\"JS Alert: \" + model.message);\n" \
"                }\n" \
"            }\n" \
"    }\n" \
"}"
#endif

// ----------------------------------------------------------------------------------------------------------------------------------
namespace WebKit1
// ----------------------------------------------------------------------------------------------------------------------------------
{

class WebView : public IWebView
{
public:
    WebView(const enum LogLevel setLevel) : 
#ifdef QT_BUILD_WITH_OPENGL
	m_viewport(),
#endif
        m_webview(),
        m_view(),
        m_scene(&m_view),
        m_webpage(setLevel)
    {
    }
    virtual ~WebView()
    {
    }

public:
    virtual bool initialize(void)
    {
        m_webview.setUrl(QUrl(""));

        m_webview.setPage(&m_webpage);

        m_scene.setItemIndexMethod(QGraphicsScene::NoIndex);
        m_scene.addItem(&m_webview);

#ifdef QT_BUILD_WITH_OPENGL
        m_view.setViewport(&m_viewport);
#endif

        m_view.setScene(&m_scene);
        m_view.setAttribute(Qt::WA_DeleteOnClose);
        m_view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        m_view.setAlignment(Qt::AlignTop | Qt::AlignHCenter);
        m_view.setFrameStyle(QFrame::NoFrame);
        m_view.showFullScreen();

#ifdef QT_BUILD_WITH_OPENGL
        QCoreApplication* app = QCoreApplication::instance();
        Q_ASSERT(NULL != app);
        app->installEventFilter(&m_view);
#endif

        return true;
    }

    virtual void destroy(void)
    {
#ifdef QT_BUILD_WITH_OPENGL
        QCoreApplication* app = QCoreApplication::instance();
        Q_ASSERT(NULL != app);
        app->removeEventFilter(&m_view);
#endif
    }

    void resize(const QSize& size)
    {
        m_view.resize(size);
        m_webview.resize(size);
    }

    void load(const QUrl& url)
    {
        m_webview.load(url);
    }

    void show(void)
    {
        m_webview.show();
    }

    void hide(void)
    {
        //No implementation
    }

    void setFocus(void)
    {
        m_webview.setFocus();
    }

    void setViewportUpdateMode(const enum ViewportUpdateMode mode)
    {
        switch(mode)
        {
            case FullViewport : m_view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
                                break;
            default           :;
        }
    }

    WebPage& page()
    {
        return (m_webpage);
    }

private:
    WebView(const WebView&);
    WebView& operator=(const WebView&);

private:
#ifdef QT_BUILD_WITH_OPENGL
    QGLWidget m_viewport;
#endif

    GraphicsWebView m_webview;
    GraphicsView m_view;
    QGraphicsScene m_scene;
    WebPage m_webpage;
};

} // namespace WebKit1
// ----------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------
namespace WebKit2 {
// ----------------------------------------------------------------------------------------------------------------------------------

#ifdef QT_BUILD_WITH_QML_API
class WebView : public IWebView
{
public:
    WebView(const enum LogLevel setLevel) :
        m_engine(),
        m_component(&m_engine),
        m_webview(NULL),
        m_view(&m_engine, NULL),
        m_webpage(setLevel)
    {
        //Creation from a local file should be instant. Therefore we keep it simple.
        m_component.setData(QByteArray(QML_DATA), QUrl(QML_URL));
        if (m_component.isError()) 
        {
            QList<QQmlError> errors = m_component.errors();
            for (int i = 0; i < errors.size(); ++i)
                qDebug() << errors.at(i).toString();
        }
        Q_ASSERT(QQmlComponent::Ready == m_component.status());

        m_webview = m_component.create();
        Q_ASSERT(NULL != m_webview);
    }
    virtual ~WebView()
    {
        delete m_webview;
    }

    bool initialize(void)
    {
        m_view.setResizeMode(QQuickView::SizeViewToRootObject);

        //Add the qml item to the view
        m_view.setContent(QUrl(QML_URL), &m_component, static_cast<QQuickItem*>(m_webview));

    return true;
    }

    void destroy(void)
    {
    }

    void resize(const QSize& size)
    {
        Q_ASSERT(NULL != m_webview);

        QQmlProperty x(m_webview, "x");
        if(x.isValid())
            x.write(0);

        QQmlProperty y(m_webview, "y");
        if(y.isValid())
            y.write(0);

        QQmlProperty width(m_webview, "width");
        if(width.isValid())
            width.write(size.width());

        QQmlProperty height(m_webview, "height");
        if(height.isValid())
            height.write(size.height());
    }

    void load(const QUrl& url)
    {
        Q_ASSERT(NULL != m_webview);

        QQmlProperty property(m_webview, "url");
        if(property.isValid())
            property.write(url);
    }

    void show(void)
    {
        Q_ASSERT(NULL != m_webview);

        QQmlProperty property(m_webview, "visible");
        if(property.isValid())
            property.write(true);

        m_view.show();
    }

    void hide(void)
    {
        //No implementation
    }

    void setFocus(void)
    {
        Q_ASSERT(NULL != m_webview);

        QQmlProperty property(m_webview, "focus");
        if(property.isValid())
            property.write(true);
    }

    void setViewportUpdateMode(const enum ViewportUpdateMode mode)
    {
        switch(mode)
        {
            case FullViewport : //No implementation
                                break;
            default           :;
        }
    }

    WebPage& page()
    {
        return (m_webpage);
    }

private:
    WebView(const WebView&);
    WebView& operator=(const WebView&);

private:
    QQmlEngine m_engine;
    QQmlComponent m_component;

    QObject* m_webview; //QQuickWebView
    QQuickView m_view;  //A view, i.e. scenegraph, to display the contents
    WebPage m_webpage;
};
#endif

} // namespace WebKit2
// ----------------------------------------------------------------------------------------------------------------------------------


/* static */ IWebView* IWebView::instance(const enum WebKitType type, const enum LogLevel setLevel)
{
    static IWebView* myWebView = NULL;
    // static enum WebKitType myWebType = WEBKIT_1;

    // Q_ASSERT((NULL == myWebView) || (type == myWebType));

    if (myWebView == NULL)
    {
        if (type == WEBKIT_1)
        {
            // myWebType = WEBKIT_1;
            myWebView = new WebKit1::WebView(setLevel);
        }
        else
        {
            // myWebType = WEBKIT_2;

#ifdef QT_BUILD_WITH_QML_API
            myWebView = new WebKit2::WebView(setLevel);
#else
            Q_ASSERT (type != WEBKIT_2);
#endif
        }
    }

    Q_ASSERT (NULL != myWebView);

    return (myWebView);
}
