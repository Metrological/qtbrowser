#include "websettings.h"
#include "webview.h"

#ifdef QT_BUILD_WITH_QML_API
#include <QQmlProperty>
#include <QList>
#include <QVariant>
#endif

WebSettings::WebSettings()
{}

WebSettings::~WebSettings()
{}

WebSettings::WebSettings(const WebView& _webview_)
{
    Q_UNUSED(_webview_);
}

WK1WebSettings::WK1WebSettings()
{}

WK1WebSettings::~WK1WebSettings()
{
    webview = NULL;
}

WK1WebSettings::WK1WebSettings(const WK1WebView& _webview_) : WebSettings(dynamic_cast<const WebView&>(_webview_))
{
    webview = &_webview_;
    Q_ASSERT(NULL != webview);
}

void WK1WebSettings::setObjectCacheCapacities(unsigned int minDead, unsigned int maxDead, unsigned int total)
{
    QWebSettings* settings = QWebSettings::globalSettings();
    Q_ASSERT(NULL != settings);

    settings->setObjectCacheCapacities(minDead, maxDead, total);
}

void WK1WebSettings::setOfflineWebApplicationCacheQuota(unsigned int size)
{
    QWebSettings* settings = QWebSettings::globalSettings();
    Q_ASSERT(NULL != settings);

    settings->setOfflineWebApplicationCacheQuota(size);
}

void WK1WebSettings::setOfflineStorageDefaultQuota(unsigned int size)
{
    QWebSettings* settings = QWebSettings::globalSettings();
    Q_ASSERT(NULL != settings);
 
    settings->setOfflineStorageDefaultQuota(size);
}

void WK1WebSettings::setStorage(Storage storage, const QString& path)
{
    QWebSettings* settings = QWebSettings::globalSettings();
    Q_ASSERT(NULL != settings);
 
    QString qstorage;
    switch(storage)
    {
        case Cookie              : settings->enablePersistentCookieStorage(path); break;
        case Icon                : return; // Use Persistent 
        case Local               : return; // Use Persistent
        case Offline             : return; // Use Persistent
        case Persistent          : settings->enablePersistentStorage(path);       break;
        case WebApplicationCache : return; // Use Persistent
        default                  : return;
    };
}

void WK1WebSettings::setAttribute(Attribute attribute, bool enable)
{
    QWebSettings::WebAttribute qattribute;
    switch(attribute)
    {
        case AcceleratedCompositing       : qattribute = QWebSettings::AcceleratedCompositingEnabled;     break;
        case AutoLoadImages               : qattribute = QWebSettings::AutoLoadImages;                    break;
        case JavaScript                   : qattribute = QWebSettings::JavascriptEnabled;                 break;
        case JavaScriptCanOpenWindow      : qattribute = QWebSettings::JavascriptCanOpenWindows;          break;
        case JavaScriptCanAccessClipBoard : qattribute = QWebSettings::JavascriptCanAccessClipboard;      break;
        case Database                     :
        case LocalStorage                 : qattribute = QWebSettings::LocalStorageDatabaseEnabled;       break;
        case OfflineStorage               : qattribute = QWebSettings::OfflineStorageDatabaseEnabled;     break;
        case OfflineWebApplicationCache   : qattribute = QWebSettings::OfflineWebApplicationCacheEnabled; break;
        case Plugins                      : qattribute = QWebSettings::PluginsEnabled;                    break;
        case PrivateBrowsing              : qattribute = QWebSettings::PrivateBrowsingEnabled;            break;
        case SpatialNavigation            : qattribute = QWebSettings::SpatialNavigationEnabled;          break;
        case TiledBackingStore            : qattribute = QWebSettings::TiledBackingStoreEnabled;          break;
        case WebAudio                     : return;
        case WebGL                        : qattribute = QWebSettings::WebGLEnabled;                      break;
        case WebSecurity                  : qattribute = QWebSettings::WebSecurityEnabled;                break;
        default                           : return;
    };

    QWebSettings* settings = QWebSettings::globalSettings();
    Q_ASSERT(NULL != settings);

    settings->setAttribute(qattribute, enable);
}

void WK1WebSettings::setGraphic(Graphic graphic, const QString& resource)
{
    QWebSettings::WebGraphic qgraphic;
    switch(graphic)
    {
        case MissingImage  : qgraphic = QWebSettings::MissingImageGraphic;  break;
        case MissingPlugin : qgraphic = QWebSettings::MissingPluginGraphic; break;
        default            : return;
    };

    QWebSettings* settings = QWebSettings::globalSettings();
    Q_ASSERT(NULL != settings);

//REMARK: empty reource results in NULL pixmap
//TODO: scale image to visible dimensions
    settings->setWebGraphic(qgraphic, QPixmap(resource));
}

void WK1WebSettings::setRemoteInspector(unsigned int port)
{
//TODO: do not automatically start/load inspector

    qDebug()<<__PRETTY_FUNCTION__<<" Currently not supported";

    if(0 < port)
    {
        QWebSettings* settings = QWebSettings::globalSettings();
        Q_ASSERT(NULL != settings);

/*
        settings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);

        webview->page();
        QWebPage& _page_ = dynamic_cast<QWebPage&>(webview->page());
        _page_.setProperty("_q_webInspectorServerPort", port);
*/
    }
}

void WK1WebSettings::setMaximumPagesInCache(unsigned int number)
{
    QWebSettings* settings = QWebSettings::globalSettings();
    Q_ASSERT(NULL != settings);

    settings->setMaximumPagesInCache(number);
}

#ifdef QT_BUILD_WITH_QML_API
WK2WebSettings::~WK2WebSettings()
{
    Q_ASSERT(NULL != webview);
    webview = NULL;
}

WK2WebSettings::WK2WebSettings(const WK2WebView& _webview_) : WebSettings(dynamic_cast<const WebView&>(_webview_))
{
    webview = &_webview_;
    Q_ASSERT(NULL != webview);
}

void WK2WebSettings::setObjectCacheCapacities(unsigned int minDead, unsigned int maxDead, unsigned int total)
{
    Q_ASSERT(NULL != webview);

    QQmlProperty webpreferences(webview->q_webview, "experimental.preferences");

    if(true == webpreferences.isValid())
    {
        QVariant variant = webpreferences.read();

        if(true == variant.canConvert(QMetaType::QObjectStar))
        {
            QObject* object = variant.value<QObject*>();
            Q_ASSERT(NULL != object);

            QQmlProperty property(object, "cacheCapacities");
            if(true == property.isValid())
            {
                QList<QVariant> capacities;
                capacities << minDead << maxDead << total;
                property.write(capacities);
            }
        }
    }
}

void WK2WebSettings::setOfflineWebApplicationCacheQuota(unsigned int size)
{
    Q_ASSERT(NULL != webview);

    QQmlProperty webpreferences(webview->q_webview, "experimental.preferences");

    if(true == webpreferences.isValid())
    {
        QVariant variant = webpreferences.read();

        if(true == variant.canConvert(QMetaType::QObjectStar))
        {
            QObject* object = variant.value<QObject*>();
            Q_ASSERT(NULL != object);

            QQmlProperty property(object, "webApplicationCacheQuota");
            if(true == property.isValid())
            {
                property.write(size);
            }
        }
    }
}

void WK2WebSettings::setOfflineStorageDefaultQuota(unsigned int size)
{
    Q_ASSERT(NULL != webview);

    QQmlProperty webpreferences(webview->q_webview, "experimental.preferences");

    if(true == webpreferences.isValid())
    {
        QVariant variant = webpreferences.read();

        if(true == variant.canConvert(QMetaType::QObjectStar))
        {
            QObject* object = variant.value<QObject*>();
            Q_ASSERT(NULL != object);

            QQmlProperty property(object, "offLineStorageQuota");
            if(true == property.isValid())
                property.write(size);
        }
    }
}

void WK2WebSettings::setStorage(Storage storage, const QString& path)
{
    Q_ASSERT(NULL != webview);

    QString qstorage;
    switch(storage)
    {
        case Cookie              : qstorage = "persistentCookieStoragePath"; break;
        case Icon                : return; // Use Persistent 
        case Offline             : return; // Use Persistent 
        case Local               : return; // Use Persistent 
        case Persistent          : qstorage = "persistentStoragePath";       break;
        case WebApplicationCache : return; // Use Persistent 
        default                  : return;
    };
 
    QQmlProperty webpreferences(webview->q_webview, "experimental.preferences");

    if(true == webpreferences.isValid())
    {
        QVariant variant = webpreferences.read();

        if(true == variant.canConvert(QMetaType::QObjectStar))
        {
            QObject* object = variant.value<QObject*>();
            Q_ASSERT(NULL != object);

            QQmlProperty property(object, qstorage);
            if(true == property.isValid())
                property.write(path); // Currently WK2 does not support setting/changing paths. QtWebCustomPaths are used.
        }
    }
}

void WK2WebSettings::setAttribute(Attribute attribute, bool enable)
{
    Q_ASSERT(NULL != webview);

    QString qattribute;
    switch(attribute)
    {
        case AcceleratedCompositing     : return;
        case AutoLoadImages             : qattribute = "autoLoadImages";                    break;
        case JavaScript                 : qattribute = "javaScriptEnabled";                 break;
/*
        case JavaScriptWindow           : qattribute = "";                                  break;
        case JavaScriptClipBoard        : qattribute = "";                                  break;
*/
        case LocalStorage               : qattribute = "localStorageEnabled";               break;
/*
        case Database                   :
        case Offline                    : qattribute = "offLineStorage";                    break;
*/
        case OfflineWebApplicationCache : qattribute = "offLineWebApplicationCacheEnabled"; break;
        case Plugins                    : qattribute = "pluginsEnabled";                    break;
        case PrivateBrowsing            : qattribute = "privateBrowsingEnabled";            break;
/*
        case SpatialNavigation          : qattribute = "";                                  break;
*/
        case TiledBackingStore          : return;
        case WebAudio                   : qattribute = "webAudioEnabled";                   break;
        case WebGL                      : qattribute = "webGLEnabled";                      break;
        case WebSecurity                : qattribute = "webSecurityEnabled";                break;
        default                         : return;
    }; 

    QQmlProperty webpreferences(webview->q_webview, "experimental.preferences");

    if(true == webpreferences.isValid())
    {
        QVariant variant = webpreferences.read();

        if(true == variant.canConvert(QMetaType::QObjectStar))
        {
            QObject* object = variant.value<QObject*>();
            Q_ASSERT(NULL != object);

            QQmlProperty property(object, qattribute);
            if(true == property.isValid())
                property.write(enable);
        }
    }
}

void WK2WebSettings::setGraphic(Graphic graphic, const QString& resource)
{
    Q_ASSERT(NULL != webview);

    QString qgraphic;
    switch(graphic)
    {
        case MissingImage  : qgraphic = "missingImage";
        case MissingPlugin : qgraphic = "missingPlugin";
        default : return;
    };
 
    QQmlProperty webpreferences(webview->q_webview, "experimental.preferences");

    if(true == webpreferences.isValid())
    {
        QVariant variant = webpreferences.read();

        if(true == variant.canConvert(QMetaType::QObjectStar))
        {
            QObject* object = variant.value<QObject*>();
            Q_ASSERT(NULL != object);

            QQmlProperty property(object, qgraphic);
            if(true == property.isValid())
            {
                QPixmap pixmap(resource);
                property.write(pixmap);
            }
        }
    }
}

void WK2WebSettings::setRemoteInspector(unsigned int port)
{
    Q_ASSERT(NULL != webview);

    Q_UNUSED(port);

    //No implementation
    qWarning("%s Function not implemented\n",__PRETTY_FUNCTION__);

/*
//TODO: do not automatically start/load inspector
    if(0 < port)
    {
        QQmlProperty webpreferences(q_webview, "experimental.preferences");

        if(true == webpreferences.isValid())
        {
            QVariant variant = webpreferences.read();

            if(true == variant.canConvert(QMetaType::QObjectStar))
            {
                QObject* object = variant.value<QObject*>();
                Q_ASSERT(NULL != object);

                QQmlProperty property(object, "developerExtrasEnabled");
                if(true == property.isValid())
                {
                    property.write(true);
                    qputenv("QTWEBKIT_INSPECTOR_SERVER", value);
                }
            }
        }
    }
*/
}

void WK2WebSettings::setMaximumPagesInCache(unsigned int number)
{
    Q_ASSERT(NULL != webview);

    QQmlProperty webpreferences(webview->q_webview, "experimental.preferences");

    if(true == webpreferences.isValid())
    {
        QVariant variant = webpreferences.read();

        if(true == variant.canConvert(QMetaType::QObjectStar))
        {
            QObject* object = variant.value<QObject*>();
            Q_ASSERT(NULL != object);

            QQmlProperty property(object, "maxCachedPages");
            if(true == property.isValid())
                property.write(number);
        }
    }
}
#endif // QT_BUILD_WITH_QML_API
