#ifdef QT_BUILD_WITH_WEBDRIVER

// QWebDriver headers, being thirdparty headers and used as-is,
//       ignore the warning of certain parameters not being used
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <webdriver_server.h>
#include <webdriver_view_enumerator.h>
#include <webdriver_view_executor.h>
#include <webdriver_view_factory.h>
#include <webdriver_view_transitions.h>

#include <extension_qt/graphics_web_view_enumerator.h>
#include <extension_qt/graphics_web_view_executor.h>
#include <extension_qt/q_session_lifecycle_actions.h>
#include <extension_qt/q_view_runner.h>
#include <extension_qt/qwebviewext.h>
#include <extension_qt/web_view_creator.h>
#include <extension_qt/web_view_enumerator.h>
#include <extension_qt/web_view_executor.h>
#include <extension_qt/widget_view_creator.h>
#include <extension_qt/widget_view_enumerator.h>
#include <extension_qt/widget_view_executor.h>
#pragma GCC diagnostic pop


int startWebDriver()
{
    printf("Enabling QtWebDriver support\r\n\r\n");

    webdriver::ViewRunner::RegisterCustomRunner<webdriver::QViewRunner>();
    webdriver::SessionLifeCycleActions::RegisterCustomLifeCycleActions<webdriver::QSessionLifeCycleActions>();
    webdriver::ViewTransitionManager::SetURLTransitionAction(new webdriver::URLTransitionAction_CloseOldView());

    /* Configure widget views */
    webdriver::ViewCreator* widgetCreator = new webdriver::QWidgetViewCreator();
    widgetCreator->RegisterViewClass<QWidget>("QWidget");

    /* Configure web views */
    webdriver::ViewCreator* webCreator = new webdriver::QWebViewCreator();
    webCreator->RegisterViewClass<QWebViewExt>("QWebViewExt");
    webdriver::ViewFactory::GetInstance()->AddViewCreator(webCreator);

    /* Configure WebView support */
    webdriver::ViewEnumerator::AddViewEnumeratorImpl(new webdriver::WebViewEnumeratorImpl());
    webdriver::ViewCmdExecutorFactory::GetInstance()->AddViewCmdExecutorCreator(new webdriver::QWebViewCmdExecutorCreator());

    /* Configure GraphicsWebView support */
    webdriver::ViewEnumerator::AddViewEnumeratorImpl(new webdriver::GraphicsWebViewEnumeratorImpl());
    webdriver::ViewCmdExecutorFactory::GetInstance()->AddViewCmdExecutorCreator(new webdriver::GraphicsWebViewCmdExecutorCreator());

    /* Add widget creator last so that it deos not conflict with webview creator (QWebView is a subclass of QWidget)*/
    webdriver::ViewFactory::GetInstance()->AddViewCreator(widgetCreator);

    webdriver::ViewEnumerator::AddViewEnumeratorImpl(new webdriver::WidgetViewEnumeratorImpl());

    webdriver::ViewCmdExecutorFactory::GetInstance()->AddViewCmdExecutorCreator(new webdriver::QWidgetViewCmdExecutorCreator());

    //const QString& webdriverString = settings.value("WebDriver/cmdline").toString();
    //QString webdriverString("--config=/root/wdconfig.json");
    //QString webdriverString("--port=9542");
    QString webdriverString;
    CommandLine cmd_line(CommandLine::NO_PROGRAM);
    if (!webdriverString.isEmpty()) {
        const QStringList arguments(webdriverString.split(QRegExp("\\s+")));
        QStringListIterator iterator(arguments);

        while (iterator.hasNext()) {
            cmd_line.AppendArgNative(iterator.next().toStdString());
        }
    }

    if (webdriver::Server::GetInstance()->Configure(cmd_line) != 0) {
        printf("failed to configure WebDriver\n");
        return 1;
    }

    const int startError = webdriver::Server::GetInstance()->Start();
    if (startError) {
        printf("Error while starting server %d\n", startError);
        return startError;
    }

    return 0;
}
#endif  // QT_BUILD_WITH_WEBDRIVER


