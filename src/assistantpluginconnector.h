#ifndef ASSISTANTPLUGINCONNECTOR_H
#define ASSISTANTPLUGINCONNECTOR_H

#include <QQmlContext>
#include "plugininterface.h"
#include "settings.h"

class KU_Assistant_PluginConnector : public KU::PLUGIN::PluginConnector
{
    Q_OBJECT

public:
    KU_Assistant_PluginConnector(QObject* parent = nullptr);
    void setup();

    Q_INVOKABLE void listen();
    Q_INVOKABLE QStringList languages() const;
};

#endif // ASSISTANTPLUGINCONNECTOR_H
