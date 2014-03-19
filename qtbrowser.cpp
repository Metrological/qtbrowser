#include <QtGui>

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QApplication>
#else
#include <QtWidgets>
#endif

#include <QNetworkProxy>

#include <QtOpenGL/QGLWidget>
#include <QGraphicsWebView>

#include <QWebSettings>

void help(void) {
  printf("%s",
    " ------------------------------------------------------------------------------\n"
    " Usage: qtbrowser --url=http://www.example.org/                                \n"
    " ------------------------------------------------------------------------------\n"
    "  --help                         Print this help page and exit                 \n"
    "  --url=<url>                    The URL to view (http:...|file:...|...)       \n"
    "  --app-name=<name>              appName used in User-Agent; default is none   \n"
    "  --app-version=<version>        appVers used in User-Agent; default is none   \n"
    "  --user-agent=<string>          Override the User-Agent header Qt would set   \n"
    "  --no-missing-image             Disable missing image icon in WebKit          \n"
    "  --auto-load-images=<on|off>    Automatic image loading (default: on)         \n"
    "  --http-proxy=<url>             Address for HTTP proxy server (default: none) \n"
    "  --insecure                     Ignore SSL/TLS certificate errors             \n"
    " ------------------------------------------------------------------------------\n"
    " http://www.metrological.com - (c) 2014 Metrological - support@metrological.com\n"
    "");
}

void webSettingAttribute(QWebSettings::WebAttribute option, const QString& value) {
    if (value == "on")
        QWebSettings::globalSettings()->setAttribute(option, true);
    else if (value == "off")
        QWebSettings::globalSettings()->setAttribute(option, false);
    else
        (void)0;
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QNetworkAccessManager manager;

    QSize size = QApplication::desktop()->screenGeometry().size();

    QWebSettings::globalSettings()->setAttribute(QWebSettings::AcceleratedCompositingEnabled, true);

    QGraphicsView g;
    g.setScene(new QGraphicsScene(&g));
    g.resize(size);
    g.scene()->setItemIndexMethod(QGraphicsScene::NoIndex);
    g.setAttribute(Qt::WA_DeleteOnClose);
    g.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    g.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    g.setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    g.setFrameStyle(QFrame::NoFrame);
    g.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    g.setViewport(new QGLWidget(QGLFormat(QGL::DirectRendering | QGL::DoubleBuffer | QGL::NoDepthBuffer | QGL::NoStencilBuffer | QGL::NoSampleBuffers)));
    g.showFullScreen();

    QGraphicsWebView view;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    view.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::HighQualityAntialiasing);
#endif
    view.resize(size);

    QWebSettings::globalSettings()->setAttribute(QWebSettings::LocalStorageEnabled, true);
    QWebSettings::globalSettings()->enablePersistentStorage("/tmp/qtbrowser");
    QWebSettings::globalSettings()->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, true);
    QWebSettings::globalSettings()->setOfflineWebApplicationCachePath("/tmp/qtbrowser");
    QWebSettings::globalSettings()->setOfflineWebApplicationCacheQuota(1024*1024*5);
    QWebSettings::globalSettings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    QWebSettings::globalSettings()->setWebGraphic(QWebSettings::MissingPluginGraphic, QPixmap());

    const char* argUrl = NULL;

    for (int ax = 1; ax < argc; ++ax) {
        size_t nlen;

        const char* s = argv[ax];
        const char* value;

        // boolean options
        if (strcmp("--help", s) == 0) {
            help();
            return EXIT_FAILURE;
        } else if (strcmp("--no-missing-image", s) == 0) {
            QWebSettings::globalSettings()->setWebGraphic(QWebSettings::MissingImageGraphic, QPixmap());
        }

        value = strchr(s, '=');
        nlen = value++ - s;

        // string options
        if (strncmp("--url", s, nlen) == 0) {
            argUrl = value;
        } else if (strncmp("--app-name", s, nlen) == 0) {
            a.setApplicationName(value);
        } else if (strncmp("--app-version", s, nlen) == 0) {
            a.setApplicationVersion(value);
        } else if (strncmp("--auto-load-images", s, nlen) == 0) {
            webSettingAttribute(QWebSettings::AutoLoadImages, value);
        } else if (strncmp("--javascript", s, nlen) == 0) {
            webSettingAttribute(QWebSettings::JavascriptEnabled, value);
        } else if (strncmp("--http-proxy", s, nlen) == 0) {
            QUrl p = QUrl::fromEncoded(value);
            QNetworkProxy proxy = QNetworkProxy(QNetworkProxy::HttpProxy, p.host(), p.port(80), p.userName(), p.password());
            manager.setProxy(proxy);
            view.page()->setNetworkAccessManager(&manager);
        }
    }

    view.load(QUrl((argUrl == NULL ? "http://www.google.com" : argUrl)));
    view.setFocus();
    view.show();

    g.scene()->addItem(&view);

    a.exec();
}
