#include "webview.h"

#include <QApplication>

#ifdef QT_BUILD_WITH_QML_API
#include <QQmlProperty>
#include <QQuickItem>

#define QML_URL ""

#define QML_DATA        \
"\n"                    \
"import QtQuick 2.0\n"  \
"import QtWebKit 3.0\n" \
"import QtWebKit.experimental 1.0\n"\
"WebView {\n"           \
"    id : webView\n"    \
"}"
#endif

WebView::WebView()
{}

WebView::WebView(const WebView&)
{}

WebView& WebView::operator=(const WebView&)
{}

WebView::~WebView()
{
}

WK1WebView::WK1WebView() : g_scene(&g_view)
{
}

bool WK1WebView::initialize(void)
{
//TODO: initialize settings

    g_webview.setUrl(QUrl(""));

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

WebPage& WK1WebView::page(void)
{
    return _page_;
}

WebSettings& WK1WebView::settings()
{
    return _settings_;
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
    g_webview.load(_url_);
}

void WK1WebView::show()
{
    g_webview.show();
}

void WK1WebView::hide()
{
    // No implementation
}

void WK1WebView::setVisible(bool enable)
{
    Q_UNUSED(enable);
    // No implementation
}

void WK1WebView::setFocus(bool enable)
{
    Q_UNUSED(enable);

    g_webview.setFocus();
}

void WK1WebView::setTransparentBackground(bool enable)
{
   Q_UNUSED(enable)

    QPalette palette;
    palette.setBrush(QPalette::Active, QPalette::Window, Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Base, Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::Window, Qt::SolidPattern);
    palette.setBrush(QPalette::Inactive, QPalette::Base, Qt::SolidPattern);
    palette.setColor(QPalette::Active, QPalette::Window, Qt::transparent);
    palette.setColor(QPalette::Active, QPalette::Base, Qt::transparent);
    palette.setColor(QPalette::Inactive, QPalette::Window, Qt::transparent);
    palette.setColor(QPalette::Inactive, QPalette::Base, Qt::transparent);

    QApplication* app = dynamic_cast<QApplication*>(QCoreApplication::instance());
    Q_ASSERT(NULL != app);

    app->setPalette(palette);
}

#ifdef QT_BUILD_WITH_QML_API
WK2WebView::WK2WebView() : _settings_(*this), q_component(&q_engine), q_webview(NULL), q_view(&q_engine, NULL)
{
    // Creation from a local file should be instant. Therefore we keep it simple.
    q_component.setData(QByteArray(QML_DATA), QUrl(QML_URL));

    q_webview = q_component.create();
    Q_ASSERT(QQmlComponent::Ready == q_component.status());

    Q_ASSERT(NULL != q_webview);
}

bool WK2WebView::initialize(void)
{
//TODO: initialize settings

    Q_ASSERT(NULL != q_webview);

    QQuickItem* q_item=dynamic_cast<QQuickItem*>(q_webview);
    Q_ASSERT(NULL != q_item);

    q_view.setResizeMode(QQuickView::SizeViewToRootObject);

    // Add the qml item to the view
    q_view.setContent(QUrl(QML_URL), &q_component, q_item);

    return true;
}

WK2WebView::~WK2WebView()
{
    delete q_webview;
    q_webview = NULL;
}

WebPage& WK2WebView::page(void)
{
    return _page_;
}

WebSettings& WK2WebView::settings()
{
    return _settings_;
}

void WK2WebView::setViewportUpdateMode(WebView::ViewportUpdateMode mode)
{
    switch(mode)
    {
        case FullViewport :
            // No implementation
            break;
        default           :;
    }
}

void WK2WebView::resize(const QSize& _size_)
{
    Q_ASSERT(NULL != q_webview);

    QQmlProperty x(q_webview, "x");
    if(true == x.isValid())
        x.write(0);

    QQmlProperty y(q_webview, "y");
    if(true == y.isValid())
        y.write(0);

    QQmlProperty width(q_webview, "width");
    if(true == width.isValid())
        width.write(_size_.width());

    QQmlProperty height(q_webview, "height");
    if(true == height.isValid())
        height.write(_size_.height());
}

void WK2WebView::load(const QUrl& _url_)
{
    Q_ASSERT(NULL != q_webview);

    QQmlProperty property(q_webview, "url");
    if(true == property.isValid())
        property.write(_url_);
}

void WK2WebView::show()
{
    setVisible(true);
    q_view.show();
}

void WK2WebView::hide()
{
    setVisible(false);
    q_view.hide();
}

void WK2WebView::setVisible(bool enable)
{
    Q_ASSERT(NULL != q_webview);

    QQmlProperty property(q_webview, "visible");
    if(true == property.isValid())
        property.write(enable);
}

void WK2WebView::setFocus(bool enable)
{
    Q_ASSERT(NULL != q_webview);

    QQmlProperty property(q_webview, "focus");
    if(true == property.isValid())
        property.write(enable);
}

void WK2WebView::setTransparentBackground(bool enable)
{
    Q_ASSERT(NULL != q_webview);

    QQmlProperty property(q_webview, "experimental.transparentBackground");

    if(true == property.isValid())
        property.write(enable);
}
#endif
