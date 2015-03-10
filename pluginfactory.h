#ifndef __PLUGINFACTORY__
#define __PLUGINFACTORY__

#include <QWebPluginFactory>

class PluginFactory : public QWebPluginFactory
{
    Q_OBJECT
public:
    PluginFactory(QObject* parent = 0);
    QList<QWebPluginFactory::Plugin> plugins() const;
    QObject* create(const QString & mimeType, const QUrl & url, const QStringList & argumentNames, const QStringList & argumentValues) const;

private:
    QList<QWebPluginFactory::Plugin> m_plugins;
};

#endif // __PLUGINFACTORY__
