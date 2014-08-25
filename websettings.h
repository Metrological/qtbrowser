#ifndef __WEBSETTINGS__
#define __WEBSETTINGS__

//REMARK: may depend on an updated QWebPreferences

#include <QSettings>

class WebView;
class WebSettings
{
public:
    enum Attribute // Similar to QWebSettings and QWebPreferences
    {
        AcceleratedCompositing,       // Only WK1
        AutoLoadImages,
        Database,                     // Only WK1
        JavaScript,
        JavaScriptCanOpenWindow,      // Currently no WK2 support
        JavaScriptCanAccessClipBoard, // Currently no WK2 support
        LocalStorage,
        OfflineStorage,               // Synonym for Database
        OfflineWebApplicationCache,
        Plugins,
        PrivateBrowsing,
        SpatialNavigation,            // Currently no WK2 support
        TiledBackingStore,            // Only WK1
        WebAudio,                     // Only WK2
        WebGL,
        WebSecurity,
        MaxAttribute
    };

    enum Graphic
    {
        MissingImage,
        MissingPlugin,
        MaxGraphic
    };

    enum Storage
    {
        Cookie,
        Icon,
        Local,
        Offline,
        Persistent, // Wrapper
        WebApplicationCache,
        MaxStorage
    };

    WebSettings(const WebView& webview);
    virtual ~WebSettings();

    virtual void setRemoteInspector(unsigned int port = 0) = 0;
    virtual void setMaximumPagesInCache(unsigned int number = 1) = 0;

    virtual void setObjectCacheCapacities(unsigned int minDead = 0, unsigned int maxDead = 0, unsigned int total = 0) = 0;
    virtual void setOfflineWebApplicationCacheQuota(unsigned int size = 0) = 0;
    virtual void setOfflineStorageDefaultQuota(unsigned int size = 0) = 0;

    virtual void setStorage(Storage storage, const QString& path) = 0;
    virtual void setAttribute(Attribute attribute, bool enable = true) = 0;
    virtual void setGraphic(Graphic graphic, const QString& resource) = 0;

protected:
    WebSettings();
    WebSettings(const WebSettings&);
    WebSettings& operator=(const WebSettings&);
};

class WK1WebView;
class WK1WebSettings : public WebSettings
{
public:
    WK1WebSettings(const WK1WebView& webview);
    ~WK1WebSettings();

    void setRemoteInspector(unsigned int port = 0);
    void setMaximumPagesInCache(unsigned int number = 1);

    void setObjectCacheCapacities(unsigned int minDead = 0, unsigned int maxDead = 0, unsigned int total = 0);
    void setOfflineWebApplicationCacheQuota(unsigned int size = 0);
    void setOfflineStorageDefaultQuota(unsigned int size = 0);

    void setStorage(Storage storage, const QString& path);
    void setAttribute(Attribute attribute, bool enable);
    void setGraphic(Graphic graphic, const QString& resource);

private:
    WK1WebSettings(const WK1WebSettings&);
    WK1WebSettings& operator=(const WK1WebSettings&);

    const WK1WebView* webview;
};

#ifdef QT_BUILD_WITH_QML_API
class WK2WebView;
class WK2WebSettings : public WebSettings
{
public:
    WK2WebSettings(const WK2WebView& webview);
    ~WK2WebSettings();

    void setRemoteInspector(unsigned int port = 0);
    void setMaximumPagesInCache(unsigned int number = 1);

    void setObjectCacheCapacities(unsigned int minDead = 0, unsigned int maxDead = 0, unsigned int total = 0);
    void setOfflineWebApplicationCacheQuota(unsigned int size = 0);
    void setOfflineStorageDefaultQuota(unsigned int size = 0);
 
    void setStorage(Storage storage, const QString& path);
    void setAttribute(Attribute attribute, bool enable);
    void setGraphic(Graphic graphic, const QString& resource);

private:
    WK2WebSettings();
    WK2WebSettings(const WK2WebSettings&);
    WK2WebSettings& operator=(const WK2WebSettings&);

    const WK2WebView* webview;
};
#endif

#endif //__WEBSETTINGS__
