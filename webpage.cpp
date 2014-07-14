#include "webpage.h"

#include <QDebug>

void WebPage::javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& source)
{
    if (source.isEmpty()) {
        qDebug() << "[console]" << message.toUtf8().constData();
    } else {
        QString s = "[" + source + ":" + QString::number(lineNumber) + "]";
        qDebug() << "[console]" << s.toUtf8().constData() << message.toUtf8().constData();
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
    if (!url.isEmpty())
        return url.url();
    return QWebPage::userAgentForUrl(_url_);
}

void WebPage::setUserAgentForUrl(const QUrl& _url_, QString _agent_)
{
//TODO: use url
    Q_UNUSED(_url_);
    agent = _agent_;
}
