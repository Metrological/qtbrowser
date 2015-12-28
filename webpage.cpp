#include "webpage.h"

#include <SysLog.h>


WebPage::WebPage(LogLevel setLevel) : logLevel(setLevel)
{
}

void WebPage::javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& source)
{
    if (logLevel != LOGGING_NONE) {
        if (source.isEmpty()) {
            WTF::sysLogF("[console] %s", message.toUtf8().constData());
        } else {
            QString adaptedSource (source);

            if (logLevel != LOGGING_EXTENDED) {
                int lastIndex = source.lastIndexOf('/');

                if (lastIndex > 0)
                {
                    adaptedSource = source.mid(lastIndex + 1);
                }
            }

            QString s = "[console] [" + adaptedSource + ":" + QString::number(lineNumber) + "] " + message;
            WTF::sysLogF("%s", s.toUtf8().constData());
        }
    }
}

void WebPage::javaScriptAlert(QWebFrame*, const QString& message)
{
    WTF::sysLogF("[console] [alert] %s", message.toUtf8().constData());
}

bool WebPage::shouldInterruptJavaScript()
{
    return false;
}

QString WebPage::userAgentForUrl(const QUrl& _url_) const
{
    QString url = _url_.url();
    if (userAgents.contains(url))
        return userAgents[url];

    if (!defaultUserAgent.isEmpty())
        return defaultUserAgent;

    return QWebPage::userAgentForUrl(_url_);
}

void WebPage::setDefaultUserAgent(QString _agent_)
{
    defaultUserAgent = _agent_;
}
