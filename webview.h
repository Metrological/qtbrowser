#ifndef __WEBVIEW__
#define __WEBVIEW__

#include "webpage.h"

enum WebKitType
{
    WEBKIT_1,
    WEBKIT_2
};

enum ViewportUpdateMode 
{
    FullViewport = 0
};

struct IWebView
{
    virtual ~IWebView() {}

    static IWebView* instance(const enum WebKitType type, const enum LogLevel setLevel);
    
    virtual bool initialize(void) = 0;
    virtual void destroy(void) = 0;

    virtual void load(const QUrl& url) = 0;
 
    virtual void resize(const QSize &) = 0;

    virtual void show(void) = 0;
    virtual void hide(void) = 0;

    virtual void setFocus(void) = 0;

    virtual void setViewportUpdateMode(const enum ViewportUpdateMode) = 0;

    virtual WebPage& page() =0;
};


#endif // __WEBVIEW__
