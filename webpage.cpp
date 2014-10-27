#include "webpage.h"

#include <QDebug>

WebPage::WebPage()
{
    userAgents["http://youtube.com/tv/"] = "QtBrowser/1.0 (Linux armv6 ; U; en), RaspberryPi/1.0 (RaspberryPi, B, Wired)";
    userAgents["http://qual-e.appspot.com/"] = "QtBrowser/1.0 (Linux armv6 ; U; en), RaspberryPi/1.0 (RaspberryPi, B, Wired)";

}
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
    return userAgents.value(_url_.url(), QWebPage::userAgentForUrl(_url_));
}

void WebPage::setUserAgentForUrl(const QUrl& _url_, QString _agent_)
{
    userAgents[_url_.url()] = _agent_;
}
