#include "webpage.h"

#include <QDebug>

WebPage::WebPage(LogLevel setLevel) : logLevel(setLevel)
{
}

void WebPage::javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& source)
{
    if (logLevel != LOGGING_NONE) {
        if (source.isEmpty()) {
            qDebug() << "[console]" << message.toUtf8().constData();
        } else {
            QString adaptedSource (source);

            if (logLevel != LOGGING_EXTENDED) {
                int lastIndex = source.lastIndexOf('/');

                if (lastIndex > 0)
                {
                    adaptedSource = source.mid(lastIndex + 1);
                }
            }

            QString s = "[" + adaptedSource + ":" + QString::number(lineNumber) + "]";
            qDebug() << "[console]" << s.toUtf8().constData() << message.toUtf8().constData();
        }
    }
}

void WebPage::javaScriptAlert(QWebFrame*, const QString& message)
{
    qDebug() << "[alert]  " << message.toUtf8().constData();
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
