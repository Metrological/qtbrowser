#include "webview.h"

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

WebView* WebView::webview=NULL;

WebView::WebView()
{}

WebView& WebView::instance(void)
{
    return *webview;
}

bool WebView::initialize(void)
{
//    _size_ = QSize(0,0);
    return true;
}

WebView::~WebView()
{
    webview=NULL;
}

WebPage& WebView::page(void)
{
    return _page_;
}

void WebView::setUserAgent(const QString& value)
{
    m_userAgent = value;
}

WK1WebView::WK1WebView() : g_scene(&g_view)
{
}

WK1WebView& WK1WebView::instance(void)
{
    if (webview==NULL)
        webview = new WK1WebView();

    Q_ASSERT(NULL != webview);

    return dynamic_cast<WK1WebView&>(WebView::instance()); 
}

bool WK1WebView::initialize(void)
{
    g_webview.setUrl(QUrl(""));

    QWebPage* webPage = static_cast<QWebPage*>(&page());
    g_webview.setPage(webPage);

    g_scene.setItemIndexMethod(QGraphicsScene::NoIndex);
    g_scene.addItem(&g_webview);

#ifdef QT_BUILD_WITH_OPENGL
    g_view.setViewport(&g_viewport);
#endif

    g_view.setScene(&g_scene);
    g_view.setAttribute(Qt::WA_DeleteOnClose);
    g_view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    g_view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    g_view.setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    g_view.setFrameStyle(QFrame::NoFrame);
    g_view.showFullScreen();

#ifdef QT_BUILD_WITH_OPENGL
    QCoreApplication* app = QCoreApplication::instance();
    Q_ASSERT(NULL != app);
    app->installEventFilter(&g_view);
#endif

    return true;
}

WK1WebView::~WK1WebView()
{
#ifdef QT_BUILD_WITH_OPENGL
    QCoreApplication* app = QCoreApplication::instance();
    Q_ASSERT(NULL != app);
    app->removeEventFilter(&g_view);
#endif
}

void WK1WebView::destroy(void)
{
    delete dynamic_cast<WK1WebView*>(webview);
}

void WK1WebView::setViewportUpdateMode(WebView::ViewportUpdateMode mode)
{
    switch(mode)
    {
        case FullViewport :
            g_view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
            break;
        default           :;
    }
}

void WK1WebView::resize(const QSize& _size_)
{
    g_view.resize(_size_);
    g_webview.resize(_size_);
}

void WK1WebView::load(const QUrl& _url_)
{
    WebPage& webPage = page();
    if (!userAgent().isEmpty())
        webPage.setUserAgentForUrl(_url_, userAgent());
    g_webview.load(_url_);
}

void WK1WebView::show()
{
    g_webview.show();
}

void WK1WebView::hide()
{
    //No implementation
}

void WK1WebView::setFocus()
{
    g_webview.setFocus();
}

#ifdef QT_BUILD_WITH_QML_API
WK2WebView::WK2WebView() : q_component(&q_engine), q_webview(NULL), q_view(&q_engine, NULL)
{
    //Creation from a local file should be instant. Therefore we keep it simple.
    q_component.setData(QByteArray(QML_DATA), QUrl(QML_URL));
    if (q_component.isError()) {
        QList<QQmlError> errors = q_component.errors();
        for (int i = 0; i < errors.size(); ++i)
            qDebug() << errors.at(i).toString();
    }
    Q_ASSERT(QQmlComponent::Ready == q_component.status());

    q_webview = q_component.create();
    Q_ASSERT(NULL != q_webview);
}

WK2WebView& WK2WebView::instance(void)
{
    if (webview==NULL)
        webview = new WK2WebView();

    Q_ASSERT(NULL != webview);

    return dynamic_cast<WK2WebView&>(WebView::instance()); 
}

bool WK2WebView::initialize(void)
{
    q_view.setResizeMode(QQuickView::SizeViewToRootObject);

    Q_ASSERT(NULL != q_webview);
    QQuickItem* q_item=dynamic_cast<QQuickItem*>(q_webview);
    Q_ASSERT(NULL != q_item);

    //Add the qml item to the view
    q_view.setContent(QUrl(QML_URL), &q_component, q_item);

    return true;
}

WK2WebView::~WK2WebView()
{
    delete q_webview;
    q_webview=NULL;
}

void WK2WebView::destroy(void)
{
    delete dynamic_cast<WK2WebView*>(webview);
}

void WK2WebView::setViewportUpdateMode(WebView::ViewportUpdateMode mode)
{
    switch(mode)
    {
        case FullViewport :
            //No implementation
            break;
        default           :;
    }
}

void WK2WebView::resize(const QSize& _size_)
{
    Q_ASSERT(NULL != q_webview);

    QQmlProperty x(q_webview, "x");
    if(x.isValid())
        x.write(0);

    QQmlProperty y(q_webview, "y");
    if(y.isValid())
        y.write(0);

    QQmlProperty width(q_webview, "width");
    if(width.isValid())
        width.write(_size_.width());

    QQmlProperty height(q_webview, "height");
    if(height.isValid())
        height.write(_size_.height());
}

void WK2WebView::load(const QUrl& _url_)
{
    Q_ASSERT(NULL != q_webview);

    QQmlProperty property(q_webview, "url");
    if(property.isValid())
        property.write(_url_);

    QQmlProperty userAgentProperty(q_webview, "experimental.userAgent");
    if(userAgentProperty.isValid() && !userAgent().isEmpty())
        userAgentProperty.write(userAgent());
}

void WK2WebView::show()
{
    Q_ASSERT(NULL != q_webview);

    QQmlProperty property(q_webview, "visible");
    if(property.isValid())
        property.write(true);

    q_view.show();
}

void WK2WebView::hide()
{
    //No implementation
}

void WK2WebView::setFocus()
{
    Q_ASSERT(NULL != q_webview);

    QQmlProperty property(q_webview, "focus");
    if(property.isValid())
        property.write(true);
}

#endif
