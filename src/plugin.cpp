#include "plugin.h"
#include <QQmlEngine>

QString KU_Assistant_Plugin::name() const
{
    return "Assistant";
}

QString KU_Assistant_Plugin::id() const
{
    return "karunit_assistant";
}

KU::PLUGIN::PluginVersion KU_Assistant_Plugin::version() const
{
    return {1, 0, 0};
}

QString KU_Assistant_Plugin::license() const
{
    return "LGPL";
}

QString KU_Assistant_Plugin::icon() const
{
    return QString();
}

bool KU_Assistant_Plugin::initialize()
{
    this->getPluginConnector()->setup();

    qmlRegisterSingletonInstance("KarunitPlugins", 1, 0, "KUPAssistantPluginConnector", this->pluginConnector);

    return true;
}

bool KU_Assistant_Plugin::stop()
{
    return true;
}

bool KU_Assistant_Plugin::loadSettings()
{
    return true;
}

bool KU_Assistant_Plugin::saveSettings()
{
    return KU::Settings::instance()->status() == QSettings::NoError;
}

KU_Assistant_PluginConnector* KU_Assistant_Plugin::getPluginConnector()
{
    if (this->pluginConnector == nullptr)
        this->pluginConnector = new KU_Assistant_PluginConnector;
    return qobject_cast<KU_Assistant_PluginConnector*>(this->pluginConnector);
}
