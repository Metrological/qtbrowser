#ifndef __WEBVIEW__
#define __WEBVIEW__

#include "webpage.h"
#include "websettings.h"

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

class WebSettings;
class WebView
{
public:
    enum ViewportUpdateMode {FullViewport = 0};

    virtual ~WebView();

    virtual bool initialize(void) = 0;

    virtual WebPage& page(void) = 0 ;

    virtual WebSettings& settings(void) = 0;

    virtual void load(const QUrl& url) = 0;

    virtual void resize(const QSize& size) = 0;

    virtual void show(void) = 0;
    virtual void hide(void) = 0;
    virtual void setVisible(bool enable = true) = 0;

    virtual void setFocus(bool enable = true) = 0;

    virtual void setViewportUpdateMode(WebView::ViewportUpdateMode mode = WebView::FullViewport) = 0;
    virtual void setTransparentBackground(bool enable = true) = 0;

protected:
    WebView();
    WebView(const WebView&);
    WebView& operator=(const WebView&);
};

class WK1WebSettings;
class WK1WebView : public WebView
{
friend class WK1WebSettings;

private:
    WK1WebSettings _settings_;

public:
    WK1WebView();
    virtual ~WK1WebView();

    bool initialize(void);

    WebPage& page(void);

    WebSettings& settings(void);

    void load(const QUrl& url);

    void resize(const QSize& size);

    void show(void);
    void hide(void);
    void setVisible(bool enable);

    void setFocus(bool enable);

    void setViewportUpdateMode(WebView::ViewportUpdateMode mode);
    void setTransparentBackground(bool enable);

private:
    WK1WebView(const WK1WebView&);
    WK1WebView& operator=(const WK1WebView&);

    GraphicsWebView g_webview;
    GraphicsView g_view;
    QGraphicsScene g_scene;
#ifdef QT_BUILD_WITH_OPENGL
    QGLWidget g_viewport;
#endif

    WebPage _page_;
};

#ifdef QT_BUILD_WITH_QML_API
class WK2WebSettings;
class WK2WebView : public WebView
{
friend class WK2WebSettings;

private:
    WK2WebSettings _settings_;

public:
    WK2WebView();
    virtual ~WK2WebView();

    bool initialize(void);

    WebPage& page(void);

    WebSettings& settings(void);

    void load(const QUrl& url);

    void resize(const QSize &);

    void show(void);
    void hide(void);
    void setVisible(bool enable);

    void setFocus(bool enable);

    void setViewportUpdateMode(WebView::ViewportUpdateMode mode);
    void setTransparentBackground(bool enable);

private:
    WK2WebView(const WK2WebView&);
    WK2WebView& operator=(const WK2WebView&);

    QQmlEngine q_engine;
    QQmlComponent q_component;

    QObject* q_webview; // QQuickWebView
    QQuickView q_view;  // A view, i.e. scenegraph, to display the contents

    WebPage _page_;
};
#endif
#endif // __WEBVIEW__
