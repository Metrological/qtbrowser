#ifndef __WEBVIEW__
#define __WEBVIEW__

#include "webpage.h"

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

class WebView
{
public:
    enum ViewportUpdateMode {FullViewport = 0};

    static WebView& instance(void);
    virtual ~WebView();

    virtual bool initialize(void);
    virtual void destroy(void) = 0;

    void setUserAgent(const QString&);
    QString userAgent() const { return m_userAgent; }

    WebPage& page(void);

    virtual void load(const QUrl& url) = 0;

    virtual void resize(const QSize &) = 0;

    virtual void show(void) = 0;
    virtual void hide(void) = 0;

    virtual void setFocus(void) = 0;

    virtual void setViewportUpdateMode(WebView::ViewportUpdateMode) = 0;

protected:
    WebView();
    static WebView* webview;

private:
    // Disallow copy and assign.
    WebView(const WebView&);
    WebView& operator=(const WebView&);

    QSize _size_;
    WebPage _page_;
    mutable QString m_userAgent;
};

class WK1WebView : public WebView
{
public:
    static WK1WebView& instance(void);
    virtual ~WK1WebView();

    bool initialize(void);
    void destroy(void);

    void resize(const QSize &);

    void load(const QUrl& url);

    void show(void);
    void hide(void);

    void setFocus(void);

    void setViewportUpdateMode(WebView::ViewportUpdateMode mode);

private:
    WK1WebView();
    WK1WebView(const WK1WebView&);
    WK1WebView& operator=(const WK1WebView&);

    GraphicsWebView g_webview;
    GraphicsView g_view;
    QGraphicsScene g_scene;
#ifdef QT_BUILD_WITH_OPENGL
    QGLWidget g_viewport;
#endif
};

#ifdef QT_BUILD_WITH_QML_API
class WK2WebView : public WebView
{
public:
    static WK2WebView& instance(void);
    virtual ~WK2WebView();

    bool initialize(void);
    void destroy(void);

    void resize(const QSize &);

    void load(const QUrl& url);

    void show(void);
    void hide(void);

    void setFocus(void);

    void setViewportUpdateMode(WebView::ViewportUpdateMode mode);

private:
    WK2WebView();
    WK2WebView(const WK2WebView&);
    WK2WebView& operator=(const WK2WebView&);

    QQmlEngine q_engine;
    QQmlComponent q_component;

    QObject* q_webview; //QQuickWebView
    QQuickView q_view;  //A view, i.e. scenegraph, to display the contents
};
#endif
#endif // __WEBVIEW__
