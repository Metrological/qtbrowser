#ifndef __WEBPAGE__
#define __WEBPAGE__

#include <QString>
#include <QWebPage>
#include <QWebFrame>
#include <QUrl>

enum LogLevel
{
    LOGGING_NONE,
    LOGGING_SHORT,
    LOGGING_EXTENDED
};

class WebPage : public QWebPage
{
public:
    WebPage(LogLevel logLevel);
    void setDefaultUserAgent(QString agent);

protected:
    void javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& source);
    void javaScriptAlert(QWebFrame*, const QString& message);
    bool shouldInterruptJavaScript();
    QString userAgentForUrl(const QUrl& url) const;

private:
    QString defaultUserAgent;
    QHash<QString, QString> userAgents;
    LogLevel logLevel;
};

#endif // __WEBPAGE__
