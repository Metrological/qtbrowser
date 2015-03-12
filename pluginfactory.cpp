#include "pluginfactory.h"
#include "nrdplugin.h"
#include <stdio.h>

PluginFactory::PluginFactory(QObject* parent)
    : QWebPluginFactory(parent)
{
    QWebPluginFactory::MimeType mimeType;
    mimeType.name = "application/x-qt-plugin";
    mimeType.description = "External gl application";

    QWebPluginFactory::Plugin plugin;
    plugin.name = "GL application viewer";
    plugin.description = "GL application viewer";
    plugin.mimeTypes = QList<MimeType>() << mimeType;

    m_plugins << plugin;
}

QList<QWebPluginFactory::Plugin> PluginFactory::plugins() const
{
    return m_plugins;
}

QObject* PluginFactory::create(const QString & mimeType, const QUrl & url, const QStringList & argumentNames, const QStringList & argumentValues) const
{
    if (mimeType == "application/x-qt-plugin") {
        NRDPlugin *plugin = new NRDPlugin();
        return plugin;
    }

    return 0;
}
