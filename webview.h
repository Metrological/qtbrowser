#ifndef __WEBVIEW__
#define __WEBVIEW__

#include "webpage.h"

//WK1 ?
#include "graphicswebview.h"

//WK1
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsWebView>
#include <QGraphicsSceneContextMenuEvent>

//WK2
#ifdef QT_BUILD_WITH_QML_API
#include <QQuickView>
#endif


class WebView
{
public:
    enum ViewportUpdateMode {FullViewport = 0};

    static WebView& instance(void);
    virtual ~WebView();

    WebPage& page(void);

    virtual void load(const QUrl& url) = 0;

    virtual void resize(const QSize &) = 0;

    virtual void show(void) = 0;
    virtual void hide(void) = 0;

    virtual void setFocus(void) = 0;

    virtual void setViewportUpdateMode(WebView::ViewportUpdateMode) = 0;

    virtual void destroy(void) = 0;

protected:
    WebView();
    WebView(const WebView&);
    WebView& operator=(const WebView&);

    static WebView* webview;

private:
    QSize _size_;
    WebPage _page_;
};

class WK1WebView : public WebView
{
public:
    static WK1WebView& instance(void);
    virtual ~WK1WebView();

    void resize(const QSize &);

    void load(const QUrl& url);

    void show(void);
    void hide(void);

    void setFocus(void);

    void setViewportUpdateMode(WebView::ViewportUpdateMode mode);

    void destroy(void);

private:
    WK1WebView();
    WK1WebView(const WK1WebView&);
    WK1WebView& operator=(const WK1WebView&);

    QGraphicsView g_view;
    GraphicsWebView g_webview;
};

class WK2WebView : public WebView
{
public:
    static WK2WebView& instance(void);
    virtual ~WK2WebView();

    void resize(const QSize &);

    void load(const QUrl& url);

    void show(void);
    void hide(void);

    void setFocus(void);

    void setViewportUpdateMode(WebView::ViewportUpdateMode mode);

    void destroy(void);

private:
    WK2WebView();
    WK2WebView(const WK2WebView&);
    WK2WebView& operator=(const WK2WebView&);

    QQuickView q_view;
};
#endif // __WEBVIEW__
