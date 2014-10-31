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

#include "webview.h"
#include "sslhandler.h"

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

    QString path;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#else
    path = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#endif

    QPixmapCache::setCacheLimit(20 * 1024);

    QWebSettings* settings = QWebSettings::globalSettings();
    settings->setMaximumPagesInCache(1);
    settings->setObjectCacheCapacities(1*1024*1024, 10*1024*1024, 64*1024*1024);
#ifdef QT_BUILD_WITH_OPENGL
    settings->setAttribute(QWebSettings::AcceleratedCompositingEnabled, true);
#endif
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#ifdef QT_BUILD_WITH_OPENGL
    settings->setAttribute(QWebSettings::WebGLEnabled, true);
#endif
#endif
    settings->setAttribute(QWebSettings::WebAudioEnabled, true);
    settings->setAttribute(QWebSettings::PluginsEnabled, false);
    settings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    settings->setAttribute(QWebSettings::WebSecurityEnabled, false);
    settings->setAttribute(QWebSettings::LocalStorageEnabled, true);
    settings->enablePersistentStorage(path);
    settings->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, true);
    settings->setOfflineWebApplicationCacheQuota(1024*1024*5);
    settings->setOfflineStorageDefaultQuota(1024*1024*10);
    settings->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    settings->setAttribute(QWebSettings::JavascriptCanOpenWindows, false);
    settings->setAttribute(QWebSettings::JavascriptCanAccessClipboard, false);
    settings->setWebGraphic(QWebSettings::MissingPluginGraphic, QPixmap());

    QUrl url;

    int mode = 1;
    bool fullscreen = false;
    QString userAgent;
    QUrl proxyUrl;
    bool validateCa = true;
    unsigned int inspectorPort = 0;

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
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#ifdef QT_BUILD_WITH_OPENGL
        } else if (strcmp("--tiled-backing-store", s) == 0) {
            settings->setAttribute(QWebSettings::TiledBackingStoreEnabled, true);
#endif
#endif
        } else if (strcmp("--full-viewport-update", s) == 0) {
            fullscreen = true;
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
            userAgent = value;
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
        } else if (strncmp("--spatial-navigation", s, nlen) == 0) {
            webSettingAttribute(QWebSettings::SpatialNavigationEnabled, value);
        } else if (strncmp("--websecurity", s, nlen) == 0) {
            webSettingAttribute(QWebSettings::WebSecurityEnabled, value);
        } else if (strncmp("--inspector", s, nlen) == 0) {
            inspectorPort = (unsigned int)atoi(value);
        } else if (strncmp("--max-cached-pages", s, nlen) == 0) {
            settings->setMaximumPagesInCache((unsigned int)atoi(value));
        } else if (strncmp("--pixmap-cache", s, nlen) == 0) {
            QPixmapCache::setCacheLimit((unsigned int)atoi(value) * 1024);
        } else if (strncmp("--object-cache", s, nlen) == 0) {
            QStringList l = QString(value).split(",");
            if (l.length() == 3)
                settings->setObjectCacheCapacities(l.at(0).toInt()*1024*1024, l.at(1).toInt()*1024*1024, l.at(2).toInt()*1024*1024);
        } else if (strncmp("--http-proxy", s, nlen) == 0) {
            proxyUrl = QUrl::fromEncoded(value);
        } else if (strncmp("--ini", s, nlen) == 0) {
            QSettings ini(value, QSettings::IniFormat);
            url = QUrl(ini.value("Network/firstUrl", QApplication::applicationDirPath()).toString());
        } else if (strncmp("--validate-ca", s, nlen) == 0) {
            if (QString(value) == "off")
                validateCa = false;
        } else if (strncmp("--webkit", s, nlen) == 0) {
            mode = QString(value) == "2" ? 2 : 1;
#ifndef QT_BUILD_WITH_QML_API
            if (mode == 2) {
                printf("WebKit2 not supported in this build.\n");
                return 0;
            }
#endif
        }
    }

#ifdef QT_BUILD_WITH_QML_API
    WebView& webview = (mode==2) ? dynamic_cast<WebView&>(WK2WebView::instance()) : dynamic_cast<WebView&>(WK1WebView::instance());
#else
    WebView& webview = dynamic_cast<WebView&>(WK1WebView::instance());
#endif

    if (fullscreen)
        webview.setViewportUpdateMode(WebView::FullViewport);

    if (!userAgent.isEmpty())
        webview.setUserAgent(userAgent);

    QWebPage& page = dynamic_cast<QWebPage&>(webview.page());
    if (inspectorPort)
        page.setProperty("_q_webInspectorServerPort", inspectorPort);

    if (!proxyUrl.isEmpty()) {
        QNetworkAccessManager* manager = page.networkAccessManager();
        QNetworkProxy proxy = QNetworkProxy(QNetworkProxy::HttpProxy, proxyUrl.host(), proxyUrl.port(80), proxyUrl.userName(), proxyUrl.password());
        manager->setProxy(proxy);
     }

    if (!validateCa)
        QObject::connect(page.networkAccessManager(), SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError> &)), new SSLSlotHandler(), SLOT(sslError(QNetworkReply*, const QList<QSslError> &)));

    webview.initialize();

    webview.load(url.isEmpty() ? QUrl("http://www.google.com") : url);
    webview.resize(size);
    webview.setFocus();
    webview.show();

    return a.exec();

    webview.destroy();
}
