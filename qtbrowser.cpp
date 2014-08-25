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
#include <qglobal.h>

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QApplication>
#else
#include <QtWidgets>
#endif

#include <QNetworkProxy>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include <QDesktopWidget>
 
#include <QWebSettings>

#include <QDesktopServices>
#include <QPixmapCache>
#include <QSettings>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QStandardPaths>
#endif

#include "webview.h"
#include "sslhandler.h"

#include <QtWebCustomPaths.h>

void help(void) {
  printf("%s",
    " ------------------------------------------------------------------------------\n"
    " Usage: qtbrowser --url=http://www.example.org/                                \n"
    " ------------------------------------------------------------------------------\n"
    "  --help                         Print this help page and exit                 \n"
#ifdef QT_BUILD_WITH_QML_API
    "  --webkit=<version>             WebKit mode (1=WK1 (default), 2=WK2)          \n"
#endif
    "  --url=<url>                    The URL to view (http:...|file:...|...)       \n"
    "  --app-name=<name>              appName used in User-Agent; default is none   \n"
    "  --app-version=<version>        appVers used in User-Agent; default is none   \n"
    "  --user-agent=<string>          Set a custom User-Agent                       \n"
    "  --missing-image=<no|file>      Disable or change missing image icon          \n"
    "  --auto-load-images=<on|off>    Automatic image loading (default: on)         \n"
    "  --javascript=<on|off>          JavaScript execution (default: on)            \n"
    "  --private-browsing=<on|off>    Private browsing (default: off)               \n"
    "  --spatial-navigation=<on|off>  Spatial Navigation (default: off)             \n"
    "  --websecurity=<on|off>         WebSecurity (default: off)                    \n"
    "  --inspector=<port>             Inspector (default: disabled)                 \n"
    "  --max-cached-pages=<n>         Maximum pages in cache (default: 1)           \n"
    "  --pixmap-cache=<n>             Pixmap Cache size in MB (default: 20)         \n"
    "  --object-cache=<n,n,n>         Object Cache size in MB (default: 1,10,64)    \n"
    "  --http-proxy=<url>             Address for HTTP proxy server (default: none) \n"
    "  --transparent                  Make Qt background color transparent          \n"
    "  --full-viewport-update         Set rendering to full viewport updating       \n"
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#ifdef QT_BUILD_WITH_OPENGL
    "  --tiled-backing-store          Enable tiled backing store                    \n"
#endif
#endif
    "  --validate-ca=<on|off>         Validate Root CA certificates (default: on)   \n"
    "  --persistent-storage=<path>    Set icon database, local storage, offline     \n"
    "                                 storage and web-application cache path        \n"
    "  --cookie-storage=<path>        Set cookie storage path                       \n"
    " ------------------------------------------------------------------------------\n"
    " http://www.metrological.com - (c) 2014 Metrological - support@metrological.com\n"
    "");
}

void print_notice(void)
{
    printf("%s",
#ifdef QT_BUILD_WITH_QML_API
    "-------------------------------------------------------------------------------\n"
    "WARNING: This version of qtbrowser is not production ready. Not all features   \n"
    "         are currently supported in both WebKit modes (when enabled) . Many    \n"
    "         things might change without prior notice.                             \n"
    "-------------------------------------------------------------------------------\n"
#endif
    "");
}

int main(int argc, char *argv[]) {
    print_notice();

    QApplication a(argc, argv);

    QSize size = QApplication::desktop()->screenGeometry().size();

    QString path;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#else
    path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#endif

    int mode = 1; 
    for (int ax = 1; ax < argc; ++ax) {

       size_t nlen;

       const char* s = argv[ax];
       const char* value;

       value = strchr(s, '=');
       nlen = value++ - s;

#ifdef QT_BUILD_WITH_QML_API
        if (strncmp("--webkit", s, nlen) == 0) {
            if (strncmp("2", value, 1) == 0)
                mode = 2;
        } else if (strncmp("--persistent-storage", s, nlen) == 0) {
            QtWebCustomPaths& custompaths = QtWebCustomPaths::instance();
            custompaths.setPath(QtWebCustomPaths::PersistentStorage, value);
         } else if (strncmp("--cookie-storage", s, nlen) == 0) {
            QtWebCustomPaths& custompaths = QtWebCustomPaths::instance();
            custompaths.setPath(QtWebCustomPaths::CookieStorage, value);
        }
#endif
    }

    WebView& webview = (mode==2) ? dynamic_cast<WebView&>(*new WK2WebView) : dynamic_cast<WebView&>(*new WK1WebView);

#ifdef QT_BUILD_WITH_OPENGL
    webview.settings().setAttribute(WebSettings::AcceleratedCompositing, true);
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#ifdef QT_BUILD_WITH_OPENGL
    webview.settings().setAttribute(WebSettings::WebGL, true);
#endif
#endif
    webview.settings().setAttribute(WebSettings::LocalStorage, true);

    webview.settings().setStorage(WebSettings::Persistent, path); // Ineffective for WK2
    webview.settings().setStorage(WebSettings::Cookie, path); // Ineffective for WK2

    webview.settings().setMaximumPagesInCache(1);
    webview.settings().setObjectCacheCapacities(1*1024*1024, 10*1024*1024, 64*1024*1024);
    webview.settings().setOfflineWebApplicationCacheQuota(1024*1024*5);
    webview.settings().setOfflineStorageDefaultQuota(1024*1024*10);

    webview.settings().setAttribute(WebSettings::OfflineWebApplicationCache, true);
    webview.settings().setAttribute(WebSettings::OfflineStorage, true);
    webview.settings().setAttribute(WebSettings::JavaScriptCanOpenWindow, false);
    webview.settings().setAttribute(WebSettings::JavaScriptCanAccessClipBoard, false);

    QString resource(""); 
    webview.settings().setGraphic(WebSettings::MissingPlugin, resource);

    QPixmapCache::setCacheLimit(20 * 1024);

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
            webview.setTransparentBackground();
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#ifdef QT_BUILD_WITH_OPENGL
        } else if (strcmp("--tiled-backing-store", s) == 0) {
            webview.settings().setAttribute(WebSettings::TiledBackingStore, true);
#endif
#endif
        } else if (strcmp("--full-viewport-update", s) == 0) {
            webview.setViewportUpdateMode(WebView::FullViewport);
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
        } else if (strncmp("--user-agent", s, nlen) == 0) {
//WK1
           WebPage& page = webview.page();
            if(!url.isEmpty())
                page.setUserAgentForUrl(url, value);
        } else if (strncmp("--missing-image", s, nlen) == 0) {
            QString resource(value);
            webview.settings().setGraphic(WebSettings::MissingImage, resource);
        } else if (strncmp("--auto-load-images", s, nlen) == 0) {
            webview.settings().setAttribute(WebSettings::AutoLoadImages, (QString(value)=="on" ? true: false));
        } else if (strncmp("--javascript", s, nlen) == 0) {
            webview.settings().setAttribute(WebSettings::JavaScript, (QString(value)=="on" ? true : false));
        } else if (strncmp("--private-browsing", s, nlen) == 0) {
            webview.settings().setAttribute(WebSettings::PrivateBrowsing, (QString(value)=="on" ? true : false));
        } else if (strncmp("--spatial-navigation", s, nlen) == 0) {
            webview.settings().setAttribute(WebSettings::SpatialNavigation, (QString(value)=="on" ? true : false));
        } else if (strncmp("--websecurity", s, nlen) == 0) {
            webview.settings().setAttribute(WebSettings::WebSecurity, ((QString(value)=="on") ? true : false));
        } else if (strncmp("--inspector", s, nlen) == 0) {
            webview.settings().setRemoteInspector((unsigned int)atoi(value));
        } else if (strncmp("--max-cached-pages", s, nlen) == 0) {
            webview.settings().setMaximumPagesInCache(atoi(value));
        } else if (strncmp("--pixmap-cache", s, nlen) == 0) {
            QPixmapCache::setCacheLimit((unsigned int)atoi(value) * 1024);
        } else if (strncmp("--object-cache", s, nlen) == 0) {
            QStringList l = QString(value).split(",");
            if (l.length() == 3)
                webview.settings().setObjectCacheCapacities(l.at(0).toInt()*1024*1024, l.at(1).toInt()*1024*1024, l.at(2).toInt()*1024*1024);
        } else if (strncmp("--http-proxy", s, nlen) == 0) {
//WK1 
            QUrl p = QUrl::fromEncoded(value);
            QWebPage& page = dynamic_cast<QWebPage&> ( webview.page() );
            QNetworkAccessManager* manager = page.networkAccessManager();
            QNetworkProxy proxy = QNetworkProxy(QNetworkProxy::HttpProxy, p.host(), p.port(80), p.userName(), p.password());
            manager->setProxy(proxy);
        } else if (strncmp("--ini", s, nlen) == 0) {
            QSettings ini(value, QSettings::IniFormat);
            url = QUrl(ini.value("Network/firstUrl", QApplication::applicationDirPath()).toString());
        } else if (strncmp("--validate-ca", s, nlen) == 0) {
//WK1
            if (QString(value) == "off") {
                QWebPage& page = dynamic_cast<QWebPage&> ( webview.page() );
                QObject::connect(page.networkAccessManager(), SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError> &)), new SSLSlotHandler(), SLOT(sslError(QNetworkReply*, const QList<QSslError> &)));
            }
        } else if (strncmp("--persistent-storage", s, nlen) == 0) {
          QString path(value);
          webview.settings().setStorage(WebSettings::Persistent, path); // Ineffective for WK2
        } else if (strncmp("--cookie-storage", s, nlen) == 0) {
          QString path(value);
          webview.settings().setStorage(WebSettings::Cookie, path); // Ineffective for WK2
        }
    }

    webview.initialize();

    webview.load(url.isEmpty() ? QUrl("http://www.google.com") : url);
    webview.resize(size);
    webview.setFocus();
    webview.show();

    return a.exec();

    delete &webview;
}
