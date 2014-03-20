/**
 *      qtbrowser
 *      Copyright (c) 2014 Metrological
 *      http://www.metrological.com
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with qtbrowser; see the file LICENSE.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 **/
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

class GraphicsWebView : public QGraphicsWebView {
public:

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* ev) {
        if (ev != NULL)
            ev->ignore();
    }
};

class WebPage : public QWebPage {
public:

protected:
    void javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& source) {
        if (source.isEmpty()) {
            qDebug() << "[console]" << message.toUtf8().constData();
        } else {
            QString s = "[" + source + ":" + QString::number(lineNumber) + "]";
            qDebug() << "[console]" << s.toUtf8().constData() << message.toUtf8().constData();
        }
    }

    void javaScriptAlert(QWebFrame*, const QString& message) {
        qDebug() << "[alert]  " << message.toUtf8().constData();
    }

    bool shouldInterruptJavaScript() {
        return false;
    }
};

void help(void) {
  printf("%s",
    " ------------------------------------------------------------------------------\n"
    " Usage: qtbrowser --url=http://www.example.org/                                \n"
    " ------------------------------------------------------------------------------\n"
    "  --help                         Print this help page and exit                 \n"
    "  --url=<url>                    The URL to view (http:...|file:...|...)       \n"
    "  --app-name=<name>              appName used in User-Agent; default is none   \n"
    "  --app-version=<version>        appVers used in User-Agent; default is none   \n"
    "  --missing-image=<no|file>      Disable or change missing image icon          \n"
    "  --auto-load-images=<on|off>    Automatic image loading (default: on)         \n"
    "  --javascript=<on|off>          JavaScript execution (default: on)            \n"
    "  --private-browsing=<on|off>    Private browsing (default: off)               \n"
    "  --spetial-navigation=<on|off>  Spatial Navigation (default: off)             \n"
    "  --websecurity=<on|off>         WebSecurity (default: off)                    \n"
    "  --inspector=<port>             Inspector (default: disabled)                 \n"
    "  --pixmap-cache=<n>             Pixmap Cache size in MB (default: 20)         \n"
    "  --object-cache=<n,n,n>         Object Cache size in MB (default: 1,10,64)    \n"
    "  --http-proxy=<url>             Address for HTTP proxy server (default: none) \n"
    "  --transparent                  Make Qt background color transparent          \n"
    " ------------------------------------------------------------------------------\n"
    " http://www.metrological.com - (c) 2014 Metrological - support@metrological.com\n"
    "");
}

void webSettingAttribute(QWebSettings::WebAttribute option, const QString& value) {
    if (value == "on")
        QWebSettings::globalSettings()->setAttribute(option, true);
    else if (value == "off")
        QWebSettings::globalSettings()->setAttribute(option, false);
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QSize size = QApplication::desktop()->screenGeometry().size();

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
    g.setViewport(new QGLWidget());
    g.showFullScreen();

    QPixmapCache::setCacheLimit(20 * 1024);

    WebPage page;
    GraphicsWebView view;
    view.setPage(&page);
    view.resize(size);

    QWebSettings* settings = QWebSettings::globalSettings();
    settings->setObjectCacheCapacities(1*1024*1024, 10*1024*1024, 64*1024*1024);
    settings->setAttribute(QWebSettings::AcceleratedCompositingEnabled, true);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    settings->setAttribute(QWebSettings::WebGLEnabled, true);
#endif
    settings->setAttribute(QWebSettings::PluginsEnabled, false);
    settings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    settings->setAttribute(QWebSettings::WebSecurityEnabled, false);
    settings->setAttribute(QWebSettings::LocalStorageEnabled, true);
    settings->enablePersistentStorage("/tmp/qtbrowser");
    settings->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, true);
    settings->setOfflineWebApplicationCachePath("/tmp/qtbrowser");
    settings->setOfflineWebApplicationCacheQuota(1024*1024*5);
    settings->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    settings->setAttribute(QWebSettings::JavascriptCanOpenWindows, false);
    settings->setAttribute(QWebSettings::JavascriptCanAccessClipboard, false);
  //settings->setAttribute(QWebSettings::TiledBackingStoreEnabled, true);
  //settings->setAttribute(QWebSettings::FrameFlatteningEnable, true);
    settings->setWebGraphic(QWebSettings::MissingPluginGraphic, QPixmap());

    QUrl url;

    for (int ax = 1; ax < argc; ++ax) {
        size_t nlen;

        const char* s = argv[ax];
        const char* value;

        // boolean options
        if (strcmp("--help", s) == 0) {
            help();
            return 0;
        } else if (strcmp("--transparent", s) == 0) {
            QPalette palette;
            palette.setBrush(QPalette::Active, QPalette::Window, Qt::SolidPattern);
            palette.setBrush(QPalette::Active, QPalette::Base, Qt::SolidPattern);
            palette.setBrush(QPalette::Inactive, QPalette::Window, Qt::SolidPattern);
            palette.setBrush(QPalette::Inactive, QPalette::Base, Qt::SolidPattern);
            palette.setColor(QPalette::Active, QPalette::Window, Qt::transparent);
            palette.setColor(QPalette::Active, QPalette::Base, Qt::transparent);
            palette.setColor(QPalette::Inactive, QPalette::Window, Qt::transparent);
            palette.setColor(QPalette::Inactive, QPalette::Base, Qt::transparent);
            a.setPalette(palette);
        }

        value = strchr(s, '=');
        nlen = value++ - s;

        // string options
        if (strncmp("--url", s, nlen) == 0) {
            url = QUrl(value);
        } else if (strncmp("--app-name", s, nlen) == 0) {
            a.setApplicationName(value);
        } else if (strncmp("--app-version", s, nlen) == 0) {
            a.setApplicationVersion(value);
        } else if (strncmp("--missing-image", s, nlen) == 0) {
            if (strcmp(value, "no") == 0)
                settings->setWebGraphic(QWebSettings::MissingImageGraphic, QPixmap());
            else
                settings->setWebGraphic(QWebSettings::MissingImageGraphic, QPixmap(QString(value)));
        } else if (strncmp("--auto-load-images", s, nlen) == 0) {
            webSettingAttribute(QWebSettings::AutoLoadImages, value);
        } else if (strncmp("--javascript", s, nlen) == 0) {
            webSettingAttribute(QWebSettings::JavascriptEnabled, value);
        } else if (strncmp("--private-browsing", s, nlen) == 0) {
            webSettingAttribute(QWebSettings::PrivateBrowsingEnabled, value);
        } else if (strncmp("--spetial-navigation", s, nlen) == 0) {
            webSettingAttribute(QWebSettings::SpatialNavigationEnabled, value);
        } else if (strncmp("--websecurity", s, nlen) == 0) {
            webSettingAttribute(QWebSettings::WebSecurityEnabled, value);
        } else if (strncmp("--inspector", s, nlen) == 0) {
            page.setProperty("_q_webInspectorServerPort", (unsigned int)atoi(value));
        } else if (strncmp("--pixmap-cache", s, nlen) == 0) {
            QPixmapCache::setCacheLimit((unsigned int)atoi(value) * 1024);
        } else if (strncmp("--object-cache", s, nlen) == 0) {
            QStringList l = QString(value).split(",");
            if (l.length() == 3)
                settings->setObjectCacheCapacities(l.at(0).toInt()*1024*1024, l.at(1).toInt()*1024*1024, l.at(2).toInt()*1024*1024);
        } else if (strncmp("--http-proxy", s, nlen) == 0) {
            QUrl p = QUrl::fromEncoded(value);
            QNetworkProxy proxy = QNetworkProxy(QNetworkProxy::HttpProxy, p.host(), p.port(80), p.userName(), p.password());
            QNetworkAccessManager manager;
            manager.setProxy(proxy);
            page.setNetworkAccessManager(&manager);
        } else if (strncmp("--ini", s, nlen) == 0) {
            QSettings ini(value, QSettings::IniFormat);
            url = QUrl(ini.value("Network/firstUrl", QApplication::applicationDirPath()).toString());
        }
    }

    view.load(url.isEmpty() ? QUrl("http://www.google.com") : url);
    view.setFocus();
    view.show();

    g.scene()->addItem(&view);

    return a.exec();
}
